#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>

#include <vmachine.h>
#include <defines/gc_macros.h>
#include <builtins/builtins.h>
#include <defines/errors_str.h>
#include <builtins/unicode_io.h>
#include <allocators/data_alloc.h>
#include <allocators/vterm_alloc.h>
#include <defines/data_struct_sizes.h>

static char _ch;
static FILE* _readFile;

static struct lterm_t* readStr();
static struct lterm_t* readNum();
static struct lterm_t* readExpr();
static struct lterm_t* readIdent();
static struct lterm_t* readBrackets();
static char* addChar(char* buff, int* buffSize, int* buffCapacity, char ch);

struct func_result_t Sysfun(int entryStatus)
{
    gcInitBuiltinEnv();

    if (_memMngr.vterms[BUILTIN_FRAG->offset].tag != V_INT_NUM_TAG)
        FMT_PRINT_AND_EXIT(BAD_ARG, "Sysfun");

    char* fileName = vtermsToChars(BUILTIN_FRAG->offset + 1, BUILTIN_FRAG->length - 1, 0);
    _readFile = fopen(fileName, "r");
    free(fileName);

    if (!_readFile)
        FMT_PRINT_AND_EXIT(FILE_OPEN_ERROR, "Sysfun", fileName, "r", strerror(errno));

    struct lterm_t* res = readExpr();

    fclose(_readFile);

    return (struct func_result_t){.status = OK_RESULT, .fieldChain = res, .callChain = 0};
}

static struct lterm_t* readExpr()
{
    struct lterm_t* expr = allocateSimpleChain();
    struct lterm_t* currItem = 0;

    _ch = fgetc(_readFile);

    while (_ch != EOF)
    {
        currItem = 0;

        if (_ch == '(')
        {
            currItem = readBrackets();
        }        
        else if (isdigit(_ch))
        {
            currItem = readNum();
        }
        else if (isalpha(_ch) || _ch == '"')
        {
            currItem = readIdent();
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
            currItem = readStr();
        }

        if (currItem)
            ADD_TO_CHAIN(expr, currItem);
    }

    return expr;
}

static struct lterm_t* readBrackets()
{
    struct lterm_t* brackets = allocateSimpleChain();
    brackets->tag = L_TERM_CHAIN_KEEPER_TAG;

    brackets->chain = readExpr();

    if (_ch != ')')
        FMT_PRINT_AND_EXIT(BAD_ARG, "Sysfun");

    _ch = fgetc(_readFile);

    return brackets;
}

static struct lterm_t* readNum()
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

    struct lterm_t* fragLTerm = allocateFragmentLTerm(1);
    fragLTerm->fragment->offset = _memMngr.vtermsOffset;
    fragLTerm->fragment->length = 1;

    struct vint_t* intNum = allocateIntStruct(length);
    mpz_export(intNum->bytes, &length, 1, sizeof(uint8_t), 1, 0, num);
    mpz_clear(num);

    allocateIntNumVTerm(intNum);

    return fragLTerm;
}

static struct lterm_t* readIdent()
{
    char* buff = (char*) malloc(1);
    int buffSize = 0;
    int buffCapacity = 1;
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

        if (!quoted && !(isdigit(_ch) || isalpha(_ch)))
            break;

        buff = addChar(buff, &buffSize, &buffCapacity, _ch);
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

    struct vstring_t* ident = allocateVString(buffSize);

    uint32_t ind = 0;
    for (ind = 0; ind < buffSize; ++ind)
        ident->head[ind] = buff[ind];


    free(buff);

    struct lterm_t* fragLTerm = allocateFragmentLTerm(1);
    fragLTerm->fragment->offset = allocateIdentVTerm(ident);
    fragLTerm->fragment->length = 1;

    return fragLTerm;
}

static struct lterm_t* readStr()
{
    char* buff = (char*) malloc(1);
    int buffSize = 0;
    int buffCapacity = 1;
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

        buff = addChar(buff, &buffSize, &buffCapacity, _ch);
        _ch = fgetc(_readFile);
    }

    if (quoted)
    {
        if (_ch != '\'' && _ch != EOF)
            FMT_PRINT_AND_EXIT(BAD_ARG, "Sysfun");

        _ch = fgetc(_readFile);
    }

    struct lterm_t* fragLTerm = allocateFragmentLTerm(1);
    fragLTerm->fragment->offset = _memMngr.vtermsOffset;
    fragLTerm->fragment->length = buffSize;

    uint32_t ind = 0;
    for (ind = 0; ind < buffSize; ++ind)
        allocateSymbolVTerm(buff[ind]);

    free(buff);

    return fragLTerm;
}

static char* addChar(char* buff, int* buffSize, int* buffCapacity, char ch)
{
    if (*buffSize + 1 > *buffCapacity)
    {
        char* tmp = (char*) malloc(2 * (*buffCapacity));
        memcpy(tmp, buff, *buffCapacity);
        (*buffCapacity) *= 2;
        free(buff);
        buff = tmp;
    }

    buff[*buffSize] = ch;
    ++(*buffSize);

    return buff;
}
