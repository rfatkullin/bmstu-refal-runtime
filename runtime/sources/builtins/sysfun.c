#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>

#include <gc/gc.h>
#include <vmachine.h>
#include <defines/gc_macros.h>
#include <builtins/char_vector.h>
#include <builtins/builtins.h>
#include <defines/errors_str.h>
#include <builtins/unicode_io.h>
#include <allocators/data_alloc.h>
#include <allocators/vterm_alloc.h>
#include <defines/data_struct_sizes.h>

static struct char_vector_t _chVec;
static char _ch;
static FILE* _readFile;
static struct lterm_t* _fragLTerm;

static void chReadStr(allocate_result* res);
static void chReadNum(allocate_result* res);
static void chReadIdent(allocate_result* res);
static struct lterm_t* chReadExpr(allocate_result* res);
static struct lterm_t* chReadBrackets(allocate_result* res);
static void chSetActualFragLTerm(allocate_result* res);

struct func_result_t Sysfun(int entryStatus)
{
    gcInitBuiltinEnv();

    if (_memMngr.vterms[BUILTIN_FRAG->offset].tag != V_INT_NUM_TAG)
        FMT_PRINT_AND_EXIT(BAD_ARG, "Sysfun");

    char* fileName = vtermsToChars(BUILTIN_FRAG->offset + 1, BUILTIN_FRAG->length - 1, 0);

//    printf("Open file: %s\n", fileName);

    struct lterm_t* res = 0;
    allocate_result gcRes = GC_OK;
    allocate_result prevGCRes = GC_OK;

    constructCharVector(&_chVec);

    do
    {
        _fragLTerm = 0;

        _readFile = fopen(fileName, "r");

        if (!_readFile)
            FMT_PRINT_AND_EXIT(FILE_OPEN_ERROR, "Sysfun", fileName, "r", strerror(errno));

        clearCharVector(&_chVec);

        res = chReadExpr(&gcRes);
        fclose(_readFile);

        if (gcRes != GC_NEED_CLEAN)
            break;

        if (prevGCRes == GC_NEED_CLEAN)
            PRINT_AND_EXIT(GC_MEMORY_OVERFLOW_MSG);

        collectGarbage();
        prevGCRes = GC_NEED_CLEAN;

    } while (1);

    delCharVec(&_chVec);

    free(fileName);

    return (struct func_result_t){.status = OK_RESULT, .fieldChain = res, .callChain = 0};
}

static struct lterm_t* chReadExpr(allocate_result* res)
{    
    struct lterm_t* expr = 0;
    struct lterm_t* currItem = 0;

    CHECK_ALLOCATION_RETURN(expr, chAllocateSimpleChainLTerm(res), *res);

    _ch = fgetc(_readFile);

    while (_ch != EOF)
    {
        currItem = 0;

        if (_ch == '(')
        {
            if (_fragLTerm)
            {
                ADD_TO_CHAIN(expr, _fragLTerm);
                _fragLTerm = 0;
            }

            CHECK_ALLOCATION_RETURN(currItem, chReadBrackets(res), *res);

            ADD_TO_CHAIN(expr, currItem);
        }        
        else if (isdigit(_ch))
        {
            CHECK_VOID_RETURN(chReadNum(res), *res);
        }
        else if (isalpha(_ch) || _ch == '"')
        {
            CHECK_VOID_RETURN(chReadIdent(res), *res);
        }
        else if (isspace(_ch))
        {
            _ch = fgetc(_readFile);
        }
        else if (_ch == ')')
        {
            break;
        }
        else
        {
            CHECK_VOID_RETURN(chReadStr(res), *res);
        }
    }

    if (_fragLTerm)
    {
        ADD_TO_CHAIN(expr, _fragLTerm);
        _fragLTerm = 0;
    }

    return expr;
}

static struct lterm_t* chReadBrackets(allocate_result* res)
{
    GC_DATA_HEAP_CHECK_RETURN(CHAIN_LTERM_SIZE, *res);
    struct lterm_t* brackets = allocateSimpleChain();
    brackets->tag = L_TERM_CHAIN_KEEPER_TAG;

    CHECK_ALLOCATION_RETURN(brackets->chain, chReadExpr(res), *res);

    if (_ch != ')')
        FMT_PRINT_AND_EXIT(BAD_ARG, "Sysfun");

    _ch = fgetc(_readFile);

    return brackets;
}

static void chReadNum(allocate_result* res)
{
    uint32_t resNum = 0;

    while (_ch != EOF && isdigit(_ch))
    {
        resNum = resNum * 10 + (_ch - '0');
        _ch = fgetc(_readFile);
    }

    if (!isspace(_ch))
        FMT_PRINT_AND_EXIT(BAD_ARG, "Sysfun");

    mpz_t num;
    mpz_init_set_ui(num, resNum);
    uint32_t numb = 8 * sizeof(uint8_t);
    uint64_t length = (mpz_sizeinbase (num, 2) + numb - 1) / numb;

    GC_VTERM_HEAP_CHECK_VOID_RETURN(1, *res);
    GC_DATA_HEAP_CHECK_VOID_RETURN(VINT_STRUCT_SIZE(length), *res);

    struct vint_t* intNum = allocateIntStruct(length);
    mpz_export(intNum->bytes, &length, 1, sizeof(uint8_t), 1, 0, num);
    mpz_clear(num);    

    CHECK_VOID_RETURN(chSetActualFragLTerm(res), *res);

    allocateIntNumVTerm(intNum);
    _fragLTerm->fragment->length++;
}

static void chSetActualFragLTerm(allocate_result* res)
{
    if (!_fragLTerm)
    {
        GC_DATA_HEAP_CHECK_VOID_RETURN(FRAGMENT_LTERM_SIZE(1), *res);
        _fragLTerm = allocateFragmentLTerm(1);
        _fragLTerm->fragment->offset = _memMngr.vtermsOffset;
    }
}

static void chReadIdent(allocate_result* res)
{
    int quoted = 0;

    if (_ch == '"')
    {
        quoted = 1;
        _ch = fgetc(_readFile);        
    }    

    while (_ch != EOF)
    {
        if (quoted && _ch == '"')
            break;

        if (!quoted && !(isdigit(_ch) || isalpha(_ch) || _ch == '_'))
            break;

        addToCharVector(&_chVec, _ch);
        _ch = fgetc(_readFile);
    }

    if (quoted)
    {
        if (_ch != '"' && _ch != EOF)
            FMT_PRINT_AND_EXIT(BAD_ARG, "Sysfun");

        _ch = fgetc(_readFile);
    }

    if (_ch != EOF && !isspace(_ch))
        FMT_PRINT_AND_EXIT(BAD_ARG, "Sysfun");

    GC_VTERM_HEAP_CHECK_VOID_RETURN(1, *res);
    GC_DATA_HEAP_CHECK_VOID_RETURN(VSTRING_SIZE(_chVec.size) + FRAGMENT_LTERM_SIZE(1), *res);

    struct vstring_t* ident = allocateVString(_chVec.size);

    uint32_t ind = 0;
    for (ind = 0; ind < _chVec.size; ++ind)
        ident->head[ind] = _chVec.buff[ind];

    clearCharVector(&_chVec);

    CHECK_VOID_RETURN(chSetActualFragLTerm(res), *res);
    _fragLTerm->fragment->length++;
    allocateIdentVTerm(ident);
}

static void chReadStr(allocate_result* res)
{
    int quoted = 0;

    if (_ch == '\'')
    {
        quoted = 1;
        _ch = fgetc(_readFile);
    }

    while (_ch != EOF)
    {
        if (quoted && _ch == '\'')
            break;

        if (!quoted && (isspace(_ch) || isdigit(_ch) || isalpha(_ch)))
            break;

        addToCharVector(&_chVec, _ch);
        _ch = fgetc(_readFile);
    }

    if (quoted)
    {
        if (_ch != '\'' && _ch != EOF)
            FMT_PRINT_AND_EXIT(BAD_ARG, "Sysfun");

        _ch = fgetc(_readFile);
    }        

    GC_VTERM_HEAP_CHECK_VOID_RETURN(_chVec.size, *res);
    GC_DATA_HEAP_CHECK_VOID_RETURN(FRAGMENT_LTERM_SIZE(1), *res);

    struct lterm_t* fragLTerm = allocateFragmentLTerm(1);
    fragLTerm->fragment->offset = _memMngr.vtermsOffset;
    fragLTerm->fragment->length = _chVec.size;

    CHECK_VOID_RETURN(chSetActualFragLTerm(res), *res);
    _fragLTerm->fragment->length += _chVec.size;

    uint32_t ind = 0;
    for (ind = 0; ind < _chVec.size; ++ind)
        allocateSymbolVTerm(_chVec.buff[ind]);

    clearCharVector(&_chVec);
}
