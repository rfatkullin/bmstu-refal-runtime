#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <helpers.h>
#include <vmachine.h>
#include <to_string.h>
#include <builtins/builtins.h>
#include <builtins/unicode_io.h>
#include <allocators/data_alloc.h>
#include <allocators/vterm_alloc.h>
#include <builtins/case_map_table.h>

static struct func_result_t gcSwitchCase(uint32_t op(uint32_t ch), struct lterm_t* fieldOfView);

struct func_result_t Arg(int entryStatus)
{
    struct fragment_t* frag = gcGetAssembliedChain(_currFuncCall->fieldOfView)->fragment;

    if (frag->length != 1)
        PRINT_AND_EXIT(ARG_FUNC_BAD_ARG_NUM);

    if (memMngr.vterms[frag->offset].tag != V_INT_NUM_TAG)
        PRINT_AND_EXIT(ARG_FUNC_BAD_ARG_NUM);

    int argNum = ConvertToInt(memMngr.vterms[frag->offset].intNum);

    if (argNum < 0 || argNum >= refalProgramArgsCount)
        FMT_PRINT_AND_EXIT(BAD_PROGRAM_ARG_NUM, refalProgramArgsCount, argNum, refalProgramArgsCount);

    checkAndCleanHeaps(0, BUILTINS_RESULT_SIZE);

    struct lterm_t* chain = allocateBuiltinsResult(refalProgramArgs[argNum].offset, refalProgramArgs[argNum].length);

    return (struct func_result_t){.status = OK_RESULT, .fieldChain = chain, .callChain = 0};
}

uint64_t initArgsData(uint64_t offset, int argc, char** argv)
{
    refalProgramArgsCount = argc;
    refalProgramArgs = (struct fragment_t*)malloc(argc * sizeof(struct fragment_t));

    int i = 0;
    for (i = 0; i < argc; ++i)
    {
        refalProgramArgs[i].offset = offset;
        char* curr = argv[i];

        while (*curr)
        {
            uint32_t ch;
            curr = readUTF8CharFromStr(curr, &ch);
            memMngr.vterms[offset++] = (struct v_term){.tag = V_CHAR_TAG, .ch = ch};
            refalProgramArgs[i].length++;
        }
    }

    return offset;
}

struct func_result_t Upper(int entryStatus)
{
    return gcSwitchCase(toUpperCase, _currFuncCall->fieldOfView);
}

struct func_result_t Lower(int entryStatus)
{
    return gcSwitchCase(toLowerCase, _currFuncCall->fieldOfView);
}

struct func_result_t Symb(int entryStatus)
{
    struct fragment_t* frag = gcGetAssembliedChain(_currFuncCall->fieldOfView)->fragment;

    if (frag->length != 1)
        PRINT_AND_EXIT(SYMB_BAD_ARG);

    if (memMngr.vterms[frag->offset].tag != V_INT_NUM_TAG)
        PRINT_AND_EXIT(SYMB_BAD_ARG);

    mpz_t num;
    uint64_t size = calcBytesForIntCharArr(memMngr.vterms[frag->offset].intNum, &num);
    checkAndCleanHeaps(size, MAX(BUILTINS_RESULT_SIZE, size + 1)); // +1 for 0 character!

    // No need to update memMngr.dataOffset, because using only in this function.
    char* buff = (char*)(memMngr.data + memMngr.dataOffset);

    gmp_sprintf (buff, "%Zd", num);
    mpz_clear(num);

    uint64_t firstOffset = memMngr.vtermsOffset;
    uint64_t i = 0;
    for (i = 0; i < size; ++i)
        allocateSymbolVTerm(buff[i]);

    struct lterm_t* res = allocateBuiltinsResult(firstOffset, memMngr.vtermsOffset - firstOffset);

    return (struct func_result_t){.status = OK_RESULT, .fieldChain = res, .callChain = 0};
}

struct func_result_t Numb(int entryStatus)
{
    struct fragment_t* frag = gcGetAssembliedChain(_currFuncCall->fieldOfView)->fragment;

    if (frag->length == 0)
        PRINT_AND_EXIT(NUMB_BAD_ARG);

    int sign = 0;
    if (memMngr.vterms[frag->offset].tag == V_CHAR_TAG && memMngr.vterms[frag->offset].ch == '-')
    {
        sign = 1;
        frag->offset++;
        frag->length--;
    }

    mpz_t num;
    mpz_init_set_si(num, 0);

    uint64_t i = 0;
    for (i = 0; i < frag->length; ++i)
    {
        if (memMngr.vterms[frag->offset + i].tag != V_CHAR_TAG)
            PRINT_AND_EXIT(NUMB_BAD_ARG);

        if (memMngr.vterms[frag->offset + i].ch < '0' || memMngr.vterms[frag->offset + i].ch > '9')        
            PRINT_AND_EXIT(NUMB_BAD_ARG);        

        mpz_mul_ui(num, num, 10);
        mpz_add_ui(num, num, memMngr.vterms[frag->offset + i].ch -  '0');
    }

    if (sign)
        mpz_neg(num, num);

    struct lterm_t* res = gcConstructIntNumBuiltinResult(num);

    mpz_clear(num);

    return (struct func_result_t){.status = OK_RESULT, .fieldChain = res, .callChain = 0};
}

// TO FIX: Using fieldOfView after gc! Fix: global var func_call_t
struct func_result_t Lenw(int entryStatus)
{
    struct fragment_t* frag = gcGetAssembliedChain(_currFuncCall->fieldOfView)->fragment;

    mpz_t num;
    mpz_t helper;

    mpz_init_set_ui(num, 0);
    mpz_init_set_ui(helper, UINT32_MAX);

    mpz_add_ui(helper, helper, 1);

    uint64_t div = (uint64_t)UINT32_MAX + 1;

    mpz_addmul_ui(num, helper, frag->length / div);
    mpz_add_ui(num, num, frag->length % div);

    struct lterm_t* res = gcConstructIntNumBuiltinResult(num);

    mpz_clear(num);
    mpz_clear(helper);

    CONCAT_CHAINS(res, _currFuncCall->fieldOfView);

    return (struct func_result_t){.status = OK_RESULT, .fieldChain = res, .callChain = 0};
}

static struct func_result_t gcSwitchCase(uint32_t op(uint32_t ch), struct lterm_t* fieldOfView)
{
    assembledFragInBuiltins = gcGetAssembliedChain(fieldOfView);

    checkAndCleanHeaps(0, CHAIN_LTERM_SIZE);

    struct lterm_t* chainTerm = allocateSimpleChain();

    ADD_TO_CHAIN(chainTerm, assembledFragInBuiltins);

    struct fragment_t* frag = assembledFragInBuiltins->fragment;
    uint64_t i =0;
    for (i = 0; i < frag->length; ++i)
        memMngr.vterms[frag->offset + i].ch = op(memMngr.vterms[frag->offset + i].ch);

    assembledFragInBuiltins = 0;

    return (struct func_result_t){.status = OK_RESULT, .fieldChain = chainTerm, .callChain = 0};
}