#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <inttypes.h>

#include <helpers.h>
#include <vmachine.h>
#include <to_string.h>
#include <builtins/builtins.h>
#include <builtins/unicode_io.h>
#include <allocators/data_alloc.h>
#include <allocators/vterm_alloc.h>
#include <builtins/case_map_table.h>
#include <defines/data_struct_sizes.h>

static struct func_result_t gcSwitchCase(uint32_t op(uint32_t ch));

// TO FIX: Нет проверки на переполнение памяти. Можно все инициализировать с помощью initEnvData.
void gcInitBuiltin()
{
    CURR_FUNC_CALL->env->locals = 0;
    CURR_FUNC_CALL->env->localsCount = 0;

    CURR_FUNC_CALL->env->params = 0;
    CURR_FUNC_CALL->env->paramsCount = 0;

    CURR_FUNC_CALL->env->fovsCount = 1;

    CURR_FUNC_CALL->env->assembled = (uint64_t*)(_memMngr.data + _memMngr.dataOffset);
    _memMngr.dataOffset += sizeof(uint64_t);

    CURR_FUNC_CALL->env->stretchVarsNumber = (int*)(_memMngr.data + _memMngr.dataOffset);
    _memMngr.dataOffset += sizeof(int);

    memset(CURR_FUNC_CALL->env->assembled, 0, CURR_FUNC_CALL->env->fovsCount * sizeof(struct lterm_t*));
    memset(CURR_FUNC_CALL->env->stretchVarsNumber, 0, CURR_FUNC_CALL->env->fovsCount * sizeof(int));

    ASSEMBLY_FIELD(0, CURR_FUNC_CALL->fieldOfView);
}

struct func_result_t Arg(int entryStatus)
{
    gcInitBuiltin();

    if (BUILTIN_FRAG->length != 1)
        PRINT_AND_EXIT(ARG_FUNC_BAD_ARG_NUM);

    if (_memMngr.vterms[BUILTIN_FRAG->offset].tag != V_INT_NUM_TAG)
        PRINT_AND_EXIT(ARG_FUNC_BAD_ARG_NUM);

    int argNum = ConvertToInt(_memMngr.vterms[BUILTIN_FRAG->offset].intNum);

    if (argNum < 1 || argNum >= refalProgramArgsCount)
        FMT_PRINT_AND_EXIT(BAD_PROGRAM_ARG_NUM, refalProgramArgsCount, argNum, refalProgramArgsCount);

    checkAndCleanHeaps(0, BUILTINS_RESULT_SIZE);

    struct lterm_t* chain = allocateBuiltinsResult(refalProgramArgs[argNum].offset, refalProgramArgs[argNum].length);

    return (struct func_result_t){.status = OK_RESULT, .fieldChain = chain, .callChain = 0};
}

uint64_t initArgsData(uint64_t offset, int argc, char** argv)
{
    refalProgramArgsCount = argc;
    refalProgramArgs = (struct fragment_t*)malloc(argc * sizeof(struct fragment_t));

    int argIndex = 1;

    int i = 0;
    for (i = 1; i < argc; ++i)
    {
        if (argv[i][0] == '-')
        {
            refalProgramArgsCount--;
            continue;
        }

        refalProgramArgs[argIndex].offset = offset;
        char* curr = argv[i];

        while (*curr)
        {
            uint32_t ch;
            curr = readUTF8CharFromStr(curr, &ch);
            _memMngr.vterms[offset++] = (struct vterm_t){.tag = V_CHAR_TAG, .ch = ch};
            refalProgramArgs[argIndex].length++;
        }

        ++argIndex;
    }

    return offset;
}

struct func_result_t Upper(int entryStatus)
{
    return gcSwitchCase(toUpperCase);
}

struct func_result_t Lower(int entryStatus)
{
    return gcSwitchCase(toLowerCase);
}

struct func_result_t Symb(int entryStatus)
{
    gcInitBuiltin();

    if (BUILTIN_FRAG->length != 1)
        PRINT_AND_EXIT(SYMB_BAD_ARG);

    if (_memMngr.vterms[BUILTIN_FRAG->offset].tag != V_INT_NUM_TAG)
        PRINT_AND_EXIT(SYMB_BAD_ARG);

    mpz_t num;
    uint64_t size = calcBytesForIntCharArr(_memMngr.vterms[BUILTIN_FRAG->offset].intNum, &num);
    checkAndCleanHeaps(size, MAX(BUILTINS_RESULT_SIZE, size + 1)); // +1 for 0 character!

    // No need to update memMngr.dataOffset, because using only in this function.
    char* buff = (char*)(_memMngr.data + _memMngr.dataOffset);

    gmp_sprintf (buff, "%Zd", num);
    mpz_clear(num);

    uint64_t firstOffset = _memMngr.vtermsOffset;
    uint64_t i = 0;
    for (i = 0; i < size; ++i)
        allocateSymbolVTerm(buff[i]);

    struct lterm_t* res = allocateBuiltinsResult(firstOffset, _memMngr.vtermsOffset - firstOffset);

    return (struct func_result_t){.status = OK_RESULT, .fieldChain = res, .callChain = 0};
}

struct func_result_t Numb(int entryStatus)
{
    gcInitBuiltin();

    if (BUILTIN_FRAG->length == 0)
        PRINT_AND_EXIT(NUMB_BAD_ARG);

    int sign = 0;
    if (_memMngr.vterms[BUILTIN_FRAG->offset].tag == V_CHAR_TAG && _memMngr.vterms[BUILTIN_FRAG->offset].ch == '-')
    {
        sign = 1;
        BUILTIN_FRAG->offset++;
        BUILTIN_FRAG->length--;
    }

    mpz_t num;
    mpz_init_set_si(num, 0);

    uint64_t i = 0;
    for (i = 0; i < BUILTIN_FRAG->length; ++i)
    {
        if (_memMngr.vterms[BUILTIN_FRAG->offset + i].tag != V_CHAR_TAG)
            PRINT_AND_EXIT(NUMB_BAD_ARG);

        if (_memMngr.vterms[BUILTIN_FRAG->offset + i].ch < '0' || _memMngr.vterms[BUILTIN_FRAG->offset + i].ch > '9')
            PRINT_AND_EXIT(NUMB_BAD_ARG);        

        mpz_mul_ui(num, num, 10);
        mpz_add_ui(num, num, _memMngr.vterms[BUILTIN_FRAG->offset + i].ch -  '0');
    }

    if (sign)
        mpz_neg(num, num);

    struct lterm_t* res = gcConstructIntNumBuiltinResult(num);

    mpz_clear(num);

    return (struct func_result_t){.status = OK_RESULT, .fieldChain = res, .callChain = 0};
}

struct func_result_t Lenw(int entryStatus)
{
    gcInitBuiltin();

    mpz_t num;
    mpz_t helper;

    mpz_init_set_ui(num, 0);
    mpz_init_set_ui(helper, UINT32_MAX);

    mpz_add_ui(helper, helper, 1);

    uint64_t div = (uint64_t)UINT32_MAX + 1;

    mpz_addmul_ui(num, helper, BUILTIN_FRAG->length / div);
    mpz_add_ui(num, num, BUILTIN_FRAG->length % div);

    struct lterm_t* res = gcConstructIntNumBuiltinResult(num);

    mpz_clear(num);
    mpz_clear(helper);

    CONCAT_CHAINS(res, CURR_FUNC_CALL->fieldOfView);

    return (struct func_result_t){.status = OK_RESULT, .fieldChain = res, .callChain = 0};
}

static struct func_result_t gcSwitchCase(uint32_t op(uint32_t ch))
{
    gcInitBuiltin();

    checkAndCleanHeaps(0, BUILTINS_RESULT_SIZE);

    struct lterm_t* chainTerm = allocateBuiltinsResult(BUILTIN_FRAG->offset, BUILTIN_FRAG->length);
    struct fragment_t* frag = chainTerm->next->fragment;

    uint64_t i =0;
    for (i = 0; i < frag->length; ++i)
        _memMngr.vterms[frag->offset + i].ch = op(_memMngr.vterms[frag->offset + i].ch);

    return (struct func_result_t){.status = OK_RESULT, .fieldChain = chainTerm, .callChain = 0};
}
