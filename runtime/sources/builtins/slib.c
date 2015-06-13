#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include <helpers.h>
#include <vmachine.h>
#include <defines/gc_macros.h>
#include <builtins/builtins.h>
#include <builtins/unicode_io.h>
#include <allocators/data_alloc.h>
#include <allocators/vterm_alloc.h>
#include <builtins/case_mapping.h>
#include <defines/data_struct_sizes.h>

static uint32_t getCharFromInt(struct vint_t* numData);
static void recApplyChr(uint64_t offset, uint64_t length);
static struct func_result_t gcSwitchCase(uint32_t op(uint32_t ch));
static int getCharInfo(uint32_t ch, uint32_t* first, uint32_t* add);
static struct func_result_t gcGetPart(int first, const char* funcName);
static struct vint_t* chGetNumFromChar(uint32_t ch, allocate_result* res);
static void chRecApplyOrd(uint64_t offset, uint64_t length, allocate_result* res);
static int getFragmentFirstVTermType(struct fragment_t* frag, uint32_t* first, uint32_t* add);
static void recApplyCaseMappingOp(uint64_t offset, uint64_t length, uint32_t op(uint32_t ch));

void initBuiltins()
{
    mpz_init_set_ui(_step, 0);

    mpz_init_set_ui(MaxCharCodePoint, 2);

    mpz_pow_ui(MaxCharCodePoint, MaxCharCodePoint, 32);
}

void gcInitBuiltinEnv()
{
    checkAndCleanHeaps(0, ENV_SIZE(0, 1, 0));

    initEnvData(CURR_FUNC_CALL->env, 0, 1, 0);

    ASSEMBLY_FIELD(0, CURR_FUNC_CALL->fieldOfView);
}

struct func_result_t Mu(int entryStatus)
{
    checkAndCleanHeaps(0, 2 * CHAIN_LTERM_SIZE + FUNC_CALL_LTERM_SIZE);

    struct lterm_t* chain = allocateSimpleChain();
    struct lterm_t* funcCallChain = allocateSimpleChain();
    struct lterm_t* funcTerm = allocateFuncCallLTerm();

    funcTerm->funcCall->failEntryPoint = -1;
    funcTerm->funcCall->fieldOfView = CURR_FUNC_CALL->fieldOfView;

    ADD_TO_CHAIN(chain, funcTerm);
    ADD_TO_CHAIN(funcCallChain, funcTerm);

    return (struct func_result_t){.status = OK_RESULT, .fieldChain = chain, .callChain = funcCallChain};
}

struct func_result_t Arg(int entryStatus)
{
    gcInitBuiltinEnv();

    if (BUILTIN_FRAG->length != 1)
        FMT_PRINT_AND_EXIT(ARG_FUNC_BAD_ARG_NUM, "Arg");

    if (_memMngr.vterms[BUILTIN_FRAG->offset].tag != V_INT_NUM_TAG)
        FMT_PRINT_AND_EXIT(ARG_FUNC_BAD_ARG_NUM, "Arg");

    int argNum = ConvertToInt(_memMngr.vterms[BUILTIN_FRAG->offset].intNum);

    if (argNum < 1)
        FMT_PRINT_AND_EXIT(BAD_PROGRAM_ARG_NUM, "Arg", _refalProgramArgsCount, argNum, _refalProgramArgsCount);

    checkAndCleanHeaps(0, BUILTINS_RESULT_SIZE);

    struct lterm_t* chain = 0;

    if (argNum >= _refalProgramArgsCount)
        chain = allocateBuiltinsResult(0, 0);
    else
        chain = allocateBuiltinsResult(_refalProgramArgs[argNum].offset, _refalProgramArgs[argNum].length);

    return (struct func_result_t){.status = OK_RESULT, .fieldChain = chain, .callChain = 0};
}

uint64_t initArgsData(uint64_t offset, int argc, char** argv)
{
    _refalProgramArgsCount = argc;
    _refalProgramArgs = (struct fragment_t*)malloc(argc * sizeof(struct fragment_t));
    memset(_refalProgramArgs, 0, argc * sizeof(struct fragment_t));

    int argIndex = 1;

    int i = 0;
    for (i = 1; i < argc; ++i)
    {
        if (argv[i][0] == '-')
        {
            _refalProgramArgsCount--;
            continue;
        }

        _refalProgramArgs[argIndex].offset = offset;
        char* curr = argv[i];

        while (*curr)
        {
            uint32_t ch;
            curr = readUTF8CharFromStr(curr, &ch);
            _memMngr.vterms[offset++] = (struct vterm_t){.tag = V_CHAR_TAG, .ch = ch};
            _refalProgramArgs[argIndex].length++;
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
    gcInitBuiltinEnv();

    if (BUILTIN_FRAG->length != 1)
        FMT_PRINT_AND_EXIT(SYMB_BAD_ARG, "Symb");

    if (_memMngr.vterms[BUILTIN_FRAG->offset].tag != V_INT_NUM_TAG)
        FMT_PRINT_AND_EXIT(SYMB_BAD_ARG, "Symb");

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
    gcInitBuiltinEnv();

    if (BUILTIN_FRAG->length == 0)
        FMT_PRINT_AND_EXIT(NUMB_BAD_ARG, "Numb");

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
            FMT_PRINT_AND_EXIT(NUMB_BAD_ARG, "Numb");

        if (_memMngr.vterms[BUILTIN_FRAG->offset + i].ch < '0' || _memMngr.vterms[BUILTIN_FRAG->offset + i].ch > '9')
            FMT_PRINT_AND_EXIT(NUMB_BAD_ARG, "Numb");

        mpz_mul_ui(num, num, 10);
        mpz_add_ui(num, num, _memMngr.vterms[BUILTIN_FRAG->offset + i].ch -  '0');
    }

    if (sign)
        mpz_neg(num, num);

    struct lterm_t* res = gcConstructSingleIntNumBuiltinResult(num);

    mpz_clear(num);

    return (struct func_result_t){.status = OK_RESULT, .fieldChain = res, .callChain = 0};
}

struct func_result_t Lenw(int entryStatus)
{
    gcInitBuiltinEnv();

    mpz_t num;
    mpz_t helper;

    mpz_init_set_ui(num, 0);
    mpz_init_set_ui(helper, UINT32_MAX);

    mpz_add_ui(helper, helper, 1);

    uint64_t div = (uint64_t)UINT32_MAX + 1;

    mpz_addmul_ui(num, helper, BUILTIN_FRAG->length / div);
    mpz_add_ui(num, num, BUILTIN_FRAG->length % div);

    struct lterm_t* res = gcConstructSingleIntNumBuiltinResult(num);

    mpz_clear(num);
    mpz_clear(helper);

    CONCAT_CHAINS(res, CURR_FUNC_CALL->fieldOfView);

    return (struct func_result_t){.status = OK_RESULT, .fieldChain = res, .callChain = 0};
}

struct func_result_t Step(int entryStatus)
{
    gcInitBuiltinEnv();

    struct lterm_t* res = gcConstructSingleIntNumBuiltinResult(_step);

    return (struct func_result_t){.status = OK_RESULT, .fieldChain = res, .callChain = 0};
}

struct func_result_t Type(int entryStatus)
{
    int res = 0;
    uint32_t first = 'O';
    uint32_t add = 'O';

    checkAndCleanHeaps(2, BUILTINS_RESULT_SIZE);

    if (CURR_FUNC_CALL->fieldOfView->next == CURR_FUNC_CALL->fieldOfView)
    {
        first = '*';
    }
    else
    {
        switch (CURR_FUNC_CALL->fieldOfView->next->tag)
        {
        case L_TERM_FRAGMENT_TAG:
            res = getFragmentFirstVTermType(CURR_FUNC_CALL->fieldOfView->next->fragment, &first, &add);
            break;

        case L_TERM_CHAIN_KEEPER_TAG:
            first = 'B';
            break;

        case L_TERM_FUNC_CALL:
            first = 'F';
            break;
        }
    }

    uint64_t offset = allocateSymbolVTerm(first);

    if (res)
        allocateSymbolVTerm(add);
    else
        allocateUInt8VTerm(0);

    struct lterm_t* headChain = allocateBuiltinsResult(offset, 2);

    CONCAT_CHAINS(headChain, CURR_FUNC_CALL->fieldOfView);

    return (struct func_result_t){.status = OK_RESULT, .fieldChain = headChain, .callChain = 0};
}

static int getFragmentFirstVTermType(struct fragment_t* frag, uint32_t* first, uint32_t* add)
{
    int res = 0;
    *first = '*';

    if (frag->length == 0)
        return res;

    switch (_memMngr.vterms[frag->offset].tag)
    {
    case V_CHAR_TAG:
    {
        return getCharInfo(_memMngr.vterms[frag->offset].ch, first, add);
    }
    case V_IDENT_TAG:
    case V_CLOSURE_TAG:
        *first = 'W';
        *add = 'i';
        res = 1;
        break;

    case V_INT_NUM_TAG:
        *first = 'N';
        break;

    case V_BRACKETS_TAG:
        res = 'B';
        break;

    default:
        res = 'O';
    }

    return res;
}

static int getCharInfo(uint32_t ch, uint32_t* first, uint32_t* add)
{
    if ((ch >=0 && ch <= 32) || (ch >= 127 && ch <= 159))
    {
        *first = 'O';

        if (isUpperCase(ch))
            *add = 'u';
        else
            *add = 'l';

        return 1;
    }

    if (ch >= '0' && ch <= '9')
    {
        *first = 'D';
        return 0;
    }

    if (ch >= 'a' && ch <= 'z')
    {
        *first = 'L';
        *add = 'l';

        return 1;
    }

    if (ch >= 'A' && ch <= 'Z')
    {
        *first = 'L';
        *add = 'u';

        return 1;
    }


    *first = 'P';

    if (isUpperCase(ch))
        *add = 'u';
    else
        *add = 'l';

    return 1;
}

struct func_result_t Chr(int entryStatus)
{
    gcInitBuiltinEnv();

    struct lterm_t* chainTerm = allocateBuiltinsResult(BUILTIN_FRAG->offset, BUILTIN_FRAG->length);

    recApplyChr(BUILTIN_FRAG->offset, BUILTIN_FRAG->length);

    return (struct func_result_t){.status = OK_RESULT, .fieldChain = chainTerm, .callChain = 0};
}

struct func_result_t Ord(int entryStatus)
{
    gcInitBuiltinEnv();

    struct lterm_t* chainTerm = allocateBuiltinsResult(BUILTIN_FRAG->offset, BUILTIN_FRAG->length);

    allocate_result res;

    DOUBLE_TRY_VOID(chRecApplyOrd(BUILTIN_FRAG->offset, BUILTIN_FRAG->length, &res), res);

    return (struct func_result_t){.status = OK_RESULT, .fieldChain = chainTerm, .callChain = 0};
}

struct func_result_t Time(int entryStatus)
{
    struct lterm_t* res = 0;
    time_t currTime;

    currTime = time(0);

    if (currTime != ((time_t)-1))
    {
        char* timeString = ctime(&currTime);

        if (timeString != 0)
        {
            int len = strlen(timeString) - 1; // не учитываем конечный символ '\n'

            checkAndCleanHeaps(len, BUILTINS_RESULT_SIZE);

            uint64_t offset = _memMngr.vtermsOffset;

            int i = 0;
            for (i = 0; i < len; ++i)
                allocateSymbolVTerm(timeString[i]);

            res = allocateBuiltinsResult(offset, len);
        }
    }

    return (struct func_result_t){.status = OK_RESULT, .fieldChain = res, .callChain = 0};
}

struct func_result_t GetEnv(int entryStatus)
{
    gcInitBuiltinEnv();

    struct lterm_t* res = 0;
    char* name = vtermsToChars(BUILTIN_FRAG->offset, BUILTIN_FRAG->length, 0);
    char* envValue = getenv(name);
    free(name);

    if (envValue)
    {
        int len = strlen(envValue);

        checkAndCleanHeaps(len, BUILTINS_RESULT_SIZE);

        uint64_t offset = _memMngr.vtermsOffset;

        int i = 0;
        for (i = 0; i < len; ++i)
            allocateSymbolVTerm(envValue[i]);

        res = allocateBuiltinsResult(offset, len);
    }

    return (struct func_result_t){.status = OK_RESULT, .fieldChain = res, .callChain = 0};
}

struct func_result_t System(int entryStatus)
{
    gcInitBuiltinEnv();

    char* cmdStr = vtermsToChars(BUILTIN_FRAG->offset, BUILTIN_FRAG->length, 0);
    int callRes = system(cmdStr) / 256;
    free(cmdStr);

    int addVTerms = 0;
    if (callRes < 0)
    {
        callRes *= -1;
        addVTerms = 2;
    }

    mpz_t num;
    mpz_init_set_ui(num, callRes);
    uint32_t numb = 8 * sizeof(uint8_t);
    uint64_t length = (mpz_sizeinbase (num, 2) + numb - 1) / numb;

    checkAndCleanHeaps(1 + addVTerms, VINT_STRUCT_SIZE(length) + BUILTINS_RESULT_SIZE);

    uint64_t offset = _memMngr.vtermsOffset;

    if (addVTerms)
    {
        allocateSymbolVTerm('-');
        allocateSymbolVTerm('-');
    }

    struct vint_t* intNum = allocateIntStruct(length);
    mpz_export(intNum->bytes, &length, 1, sizeof(uint8_t), 1, 0, num);
    mpz_clear(num);

    allocateIntNumVTerm(intNum);

    struct lterm_t* res = allocateBuiltinsResult(offset, 1 + addVTerms);

    return (struct func_result_t){.status = OK_RESULT, .fieldChain = res, .callChain = 0};
}

struct func_result_t First(int first)
{
    return gcGetPart(1, "First");
}

struct func_result_t Last(int first)
{
    return gcGetPart(0, "Last");
}

static struct func_result_t gcGetPart(int first, const char* funcName)
{
    gcInitBuiltinEnv();

    if (BUILTIN_FRAG->length == 0)
        FMT_PRINT_AND_EXIT(BAD_ARG, funcName);

    if (_memMngr.vterms[BUILTIN_FRAG->offset].tag != V_INT_NUM_TAG)
        FMT_PRINT_AND_EXIT(BAD_ARG, funcName);

    checkAndCleanHeaps(0, FRAGMENT_LTERM_SIZE +
                       2 * CHAIN_LTERM_SIZE +
                       CHAIN_KEEPER_LTERM_SIZE(1));

    struct vint_t* numData = _memMngr.vterms[BUILTIN_FRAG->offset].intNum;

    if (GET_INT_SIGN(numData))
        FMT_PRINT_AND_EXIT(BAD_ARG, funcName);

    mpz_t num;
    mpz_init(num);

    mpz_import(num, GET_INT_LENGTH(numData), 1, sizeof(uint8_t), 1, 0, numData->bytes);
    uint64_t length = mpz_get_ui(num);
    mpz_clear(num);

    struct lterm_t* firstLTerm = allocateFragmentLTerm(1);
    struct lterm_t* secondLTerm = allocateSimpleChain();
    struct lterm_t* bracket = allocateChainKeeperLTerm(1);
    struct lterm_t* res = allocateSimpleChain();

    secondLTerm->tag = L_TERM_FRAGMENT_TAG;
    secondLTerm->fragment = BUILTIN_FRAG;

    uint64_t fOffset = 0;
    uint64_t fLength = 0;
    uint64_t sOffset = 0;
    uint64_t sLength = 0;

    length = min(length, BUILTIN_FRAG->length - 1);

    if (first)
    {
        fOffset = BUILTIN_FRAG->offset + 1;
        fLength = length;

        sOffset = fOffset + fLength;
        sLength = BUILTIN_FRAG->length - 1 - length;
    }
    else
    {
        fOffset = BUILTIN_FRAG->offset + 1;
        fLength = BUILTIN_FRAG->length - 1 - length;

        sOffset = fOffset + fLength;
        sLength = length;
    }

    firstLTerm->fragment->offset = fOffset;
    firstLTerm->fragment->length = fLength;

    secondLTerm->fragment->offset = sOffset;
    secondLTerm->fragment->length = sLength;

    ADD_TO_CHAIN(bracket->chain, firstLTerm);
    ADD_TO_CHAIN(res, bracket);
    ADD_TO_CHAIN(res, secondLTerm);

    return (struct func_result_t){.status = OK_RESULT, .fieldChain = res, .callChain = 0};
}

static void chRecApplyOrd(uint64_t offset, uint64_t length, allocate_result* res)
{
    uint64_t i = 0;
    for (i = 0; i < length; ++i)
    {
        if (_memMngr.vterms[offset + i].tag == V_CHAR_TAG)
        {
            _memMngr.vterms[offset + i].tag = V_INT_NUM_TAG;
            _memMngr.vterms[offset + i].intNum = chGetNumFromChar(_memMngr.vterms[offset + i].ch, res);

            if (*res == GC_NEED_CLEAN)
                return;
        }
        else if (_memMngr.vterms[offset + i].tag == V_BRACKETS_TAG)
        {
            chRecApplyOrd(_memMngr.vterms[offset + i].brackets->offset,
                _memMngr.vterms[offset + i].brackets->length,
                res);

            if (*res == GC_NEED_CLEAN)
                return;
        }
    }
}

static struct vint_t* chGetNumFromChar(uint32_t ch, allocate_result* res)
{
    mpz_t num;

    mpz_init_set_ui(num, ch);

    uint32_t numb = 8 * sizeof(uint8_t);
    uint64_t length = (mpz_sizeinbase (num, 2) + numb - 1) / numb;
    struct vint_t* intNum = 0;

    CHECK_ALLOCATION_RETURN(intNum, chAllocateIntStruct(length, res), *res);

    mpz_export(intNum->bytes, &length, 1, sizeof(uint8_t), 1, 0, num);

    return intNum;
}

static void recApplyChr(uint64_t offset, uint64_t length)
{
    uint64_t i = 0;
    for (i = 0; i < length; ++i)
    {
        if (_memMngr.vterms[offset + i].tag == V_INT_NUM_TAG)
        {
            _memMngr.vterms[offset + i].tag = V_CHAR_TAG;
            _memMngr.vterms[offset + i].ch = getCharFromInt(_memMngr.vterms[offset + i].intNum);
        }
        else if (_memMngr.vterms[offset + i].tag == V_BRACKETS_TAG)
        {
            recApplyChr(_memMngr.vterms[offset + i].brackets->offset,
                    _memMngr.vterms[offset + i].brackets->length);
        }
    }
}

static uint32_t getCharFromInt(struct vint_t* numData)
{
    mpz_t num;

    mpz_init(num);

    mpz_import(num, GET_INT_LENGTH(numData), 1, sizeof(uint8_t), 1, 0, numData->bytes);

    if (GET_INT_SIGN(numData))
        mpz_neg(num, num);

    mpz_mod(num, num, MaxCharCodePoint);

    uint32_t res = mpz_get_ui(num);

    mpz_clear(num);

    return res;
}

static struct func_result_t gcSwitchCase(uint32_t op(uint32_t ch))
{
    gcInitBuiltinEnv();

    // 0 для vterm'ов, так как во время сборки уже
    // выделили для них память.
    checkAndCleanHeaps(0, BUILTINS_RESULT_SIZE);

    struct lterm_t* chainTerm = allocateBuiltinsResult(BUILTIN_FRAG->offset, BUILTIN_FRAG->length);    

    recApplyCaseMappingOp(BUILTIN_FRAG->offset, BUILTIN_FRAG->length, op);

    return (struct func_result_t){.status = OK_RESULT, .fieldChain = chainTerm, .callChain = 0};
}

static void recApplyCaseMappingOp(uint64_t offset, uint64_t length, uint32_t op(uint32_t ch))
{
    uint64_t i = 0;
    for (i = 0; i < length; ++i)
    {
        if (_memMngr.vterms[offset + i].tag == V_CHAR_TAG)
        {
            _memMngr.vterms[offset + i].ch = op(_memMngr.vterms[offset + i].ch);
        }
        else if (_memMngr.vterms[offset + i].tag == V_BRACKETS_TAG)
        {
            recApplyCaseMappingOp(_memMngr.vterms[offset + i].brackets->offset,
                    _memMngr.vterms[offset + i].brackets->length,
                    op);
        }
    }
}
