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

static void printRange(FILE* file, struct fragment_t* frag);
static void printSymbol(FILE* file, struct v_term* term);
static void printUnicodeChar(uint32_t ch);
static void printIntNumber(FILE* file, struct v_int* intNum);
static uint64_t copySymbols(uint64_t first, uint64_t length);
static char* getFileName();
static char* assemblyFileName(struct fragment_t* frag);
static void openFile(struct fragment_t* frag, uint8_t mode, uint8_t descr);
static uint64_t calcNeedBytesCount(struct fragment_t* frag);
static uint8_t getOpenMode(struct fragment_t* frag);
static uint8_t getDescr(struct fragment_t* frag);
static void openDefaultFile(uint8_t descr, uint8_t mode);
static void openFileWithName(char* fileName, uint8_t mode, uint8_t descr);
static void _put(struct lterm_t* fieldOfView);
static struct func_result_t _get(FILE* file);
static getFileDescr(struct v_int* bigInt, uint8_t* descr);
static struct func_result_t switchCase(uint32_t op(uint32_t ch), struct lterm_t* fieldOfView);

struct func_result_t Card(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView, int firstCall)
{
    return _get(stdin);
}

struct func_result_t Get(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView, int firstCall)
{
    struct fragment_t* frag = gcGetAssembliedChain(fieldOfView)->fragment;

    if (frag->length != 1)
        PRINT_AND_EXIT(GET_WRONG_ARG_NUM);

    uint8_t descr = getDescr(frag);

    if (!descr)
        return _get(stdin);

    if (!files[descr].file)
        openDefaultFile(descr, READ_MODE);

    return _get(files[descr].file);
}

struct func_result_t Prout(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView, int firstCall)
{
    struct lterm_t* currExpr = gcGetAssembliedChain(fieldOfView);

    printRange(stdout, currExpr->fragment);

	return (struct func_result_t){.status = OK_RESULT, .fieldChain = 0, .callChain = 0};
}

struct func_result_t Print(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView, int firstCall)
{
    struct lterm_t* currExpr = gcGetAssembliedChain(fieldOfView);

    printRange(stdout, currExpr->fragment);

    return (struct func_result_t){.status = OK_RESULT, .fieldChain = fieldOfView, .callChain = 0};
}

struct func_result_t Open(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView, int firstCall)
{
    struct fragment_t* frag = gcGetAssembliedChain(fieldOfView)->fragment;

    if (frag->length < 2)
        PRINT_AND_EXIT(TOO_FEW_ARGUMENTS);

    if (memMngr.vterms[frag->offset].tag != V_CHAR_TAG)
        PRINT_AND_EXIT(BAD_FILE_OPEN_MODE);

    uint8_t mode = getOpenMode(frag);
    uint8_t descr = getDescr(frag);

    openFile(frag, mode, descr);

    return (struct func_result_t){.status = OK_RESULT, .fieldChain = 0, .callChain = 0};
}

struct func_result_t Put(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView, int firstCall)
{
    _put(fieldOfView);

    return (struct func_result_t){.status = OK_RESULT, .fieldChain = fieldOfView, .callChain = 0};
}

struct func_result_t Putout(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView, int firstCall)
{
    _put(fieldOfView);

    return (struct func_result_t){.status = OK_RESULT, .fieldChain = 0, .callChain = 0};
}

struct func_result_t Arg(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView, int firstCall)
{
    struct fragment_t* frag = gcGetAssembliedChain(fieldOfView)->fragment;

    if (frag->length != 1)
        PRINT_AND_EXIT(ARG_WRONG_ARG_NUM);

    // TO FIX: CHECK ON INT!

    int argNum = ConvertToInt(memMngr.vterms[frag->offset].intNum);

    if (argNum < 0 || argNum >= refalProgramArgsCount)
        FMT_PRINT_AND_EXIT(BAD_PROGRAM_ARG_NUM, refalProgramArgsCount, argNum, refalProgramArgsCount);

    checkAndCleanData(BUILTINS_RESULT_SIZE);

    struct lterm_t* chain = allocateBuiltinsResult(refalProgramArgs[argNum].offset, refalProgramArgs[argNum].length);

    return (struct func_result_t){.status = OK_RESULT, .fieldChain = chain, .callChain = 0};
}

// TO FIX: Убрать malloc.
uint64_t initArgsData(uint64_t offset, int argc, char** argv)
{
    refalProgramArgsCount = argc;
    refalProgramArgs = (struct fragment_t*) malloc(argc * sizeof(struct fragment_t));

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

struct func_result_t Upper(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView, int firstCall)
{
    return switchCase(toUpperCase, fieldOfView);
}

struct func_result_t Lower(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView, int firstCall)
{
    return switchCase(toLowerCase, fieldOfView);
}

struct func_result_t Symb(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView, int firstCall)
{
    struct fragment_t* frag = gcGetAssembliedChain(fieldOfView)->fragment;

    if (frag->length != 1)
        PRINT_AND_EXIT(SYMB_BAD_ARG);

    if (memMngr.vterms[frag->offset].tag != V_INT_NUM_TAG)
        PRINT_AND_EXIT(SYMB_BAD_ARG);

    struct v_int* intNum = memMngr.vterms[frag->offset].intNum;
    mpz_t num;
    mpz_init(num);
    mpz_import(num, intNum->length, 1, sizeof(uint8_t), 1, 0, intNum->bytes);

    char ch;
    uint64_t size = gmp_snprintf(&ch, 1, "%Zd", num);

    checkAndCleanTermsAndData(size, BUILTINS_RESULT_SIZE + size + 1);  // +1 for 0 character!

    // No need to update memMngr.dataOffset, because using only in this function.
    char* buff = (char*)(memMngr.data + memMngr.dataOffset);

    gmp_sprintf (buff, "%Zd", num);
    mpz_clear(num);

    uint64_t firstOffset = memMngr.vtermsOffset;
    uint64_t i = 0;

    if (intNum->sign)
        allocateSymbolVTerm('-');

    for (i = 0; i < size; ++i)
        allocateSymbolVTerm(buff[i]);

    struct lterm_t* res = allocateBuiltinsResult(firstOffset, memMngr.vtermsOffset - firstOffset);

    return (struct func_result_t){.status = OK_RESULT, .fieldChain = res, .callChain = 0};
}

struct func_result_t Numb(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView, int firstCall)
{
    assembledFrageInBuiltins = gcGetAssembliedChain(fieldOfView);
    struct fragment_t* frag = assembledFrageInBuiltins->fragment;

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

    //TO FIX: Дублирование кода. arithmetics.c
    uint32_t numb = 8 * sizeof(uint8_t);
    uint64_t length = (mpz_sizeinbase (num, 2) + numb - 1) / numb;

    checkAndCleanTermsAndData(1, VINT_STRUCT_SIZE(length) + BUILTINS_RESULT_SIZE);

    struct v_int* intNum = allocateIntStruct(length);

    mpz_export(intNum->bytes, &length, 1, sizeof(uint8_t), 1, 0, num);
    intNum->sign = sign;

    uint64_t offset = allocateIntNumVTerm(intNum);

    mpz_clear(num);

    struct lterm_t* res = allocateBuiltinsResult(offset, 1);

    assembledFrageInBuiltins = 0;

    return (struct func_result_t){.status = OK_RESULT, .fieldChain = res, .callChain = 0};
}

struct func_result_t Lenw(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView, int firstCall)
{
    assembledFrageInBuiltins = gcGetAssembliedChain(fieldOfView);
    struct fragment_t* frag = assembledFrageInBuiltins->fragment;

    mpz_t num;
    mpz_t helper;

    mpz_init_set_ui(num, 0);
    mpz_init_set_ui(helper, UINT32_MAX);

    mpz_add_ui(helper, helper, 1);

    uint64_t div = (uint64_t)UINT32_MAX + 1;

    mpz_addmul_ui(num, helper, frag->length / div);
    mpz_add_ui(num, num, frag->length % div);

    //TO FIX: Дублирование кода. arithmetics.c
    uint32_t numb = 8 * sizeof(uint8_t);
    uint64_t length = (mpz_sizeinbase (num, 2) + numb - 1) / numb;

    checkAndCleanTermsAndData(1, VINT_STRUCT_SIZE(length) + BUILTINS_RESULT_SIZE);

    struct v_int* intNum = allocateIntStruct(length);

    mpz_export(intNum->bytes, &length, 1, sizeof(uint8_t), 1, 0, num);
    intNum->sign = 0;

    uint64_t offset = allocateIntNumVTerm(intNum);

    mpz_clear(num);
    mpz_clear(helper);

    struct lterm_t* res = allocateBuiltinsResult(offset, 1);

    CONCAT_CHAINS(res, fieldOfView);

    assembledFrageInBuiltins = 0;

    return (struct func_result_t){.status = OK_RESULT, .fieldChain = res, .callChain = 0};
}

static struct func_result_t switchCase(uint32_t op(uint32_t ch), struct lterm_t* fieldOfView)
{
    assembledFrageInBuiltins = gcGetAssembliedChain(fieldOfView);

    checkAndCleanData(CHAIN_LTERM_SIZE(1));

    struct lterm_t* chainTerm = allocateChainLTerm(1);

    ADD_TO_CHAIN(chainTerm->chain, assembledFrageInBuiltins);

    struct fragment_t* frag = assembledFrageInBuiltins->fragment;
    uint64_t i =0;
    for (i = 0; i < frag->length; ++i)
        memMngr.vterms[frag->offset + i].ch = op(memMngr.vterms[frag->offset + i].ch);

    assembledFrageInBuiltins = 0;

    return (struct func_result_t){.status = OK_RESULT, .fieldChain = chainTerm->chain, .callChain = 0};
}

static struct func_result_t _get(FILE* file)
{
    uint64_t firstOffset = memMngr.vtermsOffset;
    uint64_t currOffset =  memMngr.vtermsOffset;
    struct lterm_t* mainChain = 0;

    checkAndCleanData(BUILTINS_RESULT_SIZE);

    uint32_t ch;
    while(1)
    {
        ch = readUTF8Char(file);
        if ( ch == '\n' || ch == 0)
            break;

        if (checkAndCleanVTerms(1))
        {
            uint64_t length = currOffset - firstOffset + 1;
            strictCheckVTermsOverflow(length + 1);
            firstOffset = copySymbols(firstOffset, currOffset - firstOffset);
            currOffset = memMngr.vtermsOffset;
        }
        allocateSymbolVTerm(ch);
        currOffset++;        
    }

    if (ch == '\n')
    {
        mainChain = allocateBuiltinsResult(firstOffset, memMngr.vtermsOffset - firstOffset);
    }
    else
    {        
        gcAllocateUInt8VTerm(0);
        mainChain = allocateBuiltinsResult(firstOffset, 1);
    }

    return (struct func_result_t){.status = OK_RESULT, .fieldChain = mainChain, .callChain = 0};
}

static void _put(struct lterm_t* fieldOfView)
{
    struct fragment_t* frag = gcGetAssembliedChain(fieldOfView)->fragment;

    if (frag->length < 1)
        PRINT_AND_EXIT(TOO_FEW_ARGUMENTS);

    uint8_t descr = getDescr(frag);

    if (!descr)
        printRange(stdout, frag);
    else if (!files[descr].file)
        openDefaultFile(descr, WRITE_MODE);

    printRange(files[descr].file, frag);
}

static uint8_t getOpenMode(struct fragment_t* frag)
{
    uint8_t mode;

    switch (memMngr.vterms[frag->offset].ch)
    {
        case 'w':
        case 'W':            
            mode = WRITE_MODE;
            break;

        case 'r':
        case 'R':            
            mode = READ_MODE;
           break;

        default:
            PRINT_AND_EXIT(BAD_FILE_OPEN_MODE);
    }

    frag->offset++;
    frag->length--;

    return mode;
}

static uint8_t getDescr(struct fragment_t* frag)
{
    if (memMngr.vterms[frag->offset].tag != V_INT_NUM_TAG)
        FMT_PRINT_AND_EXIT(BAD_DESCR, MAX_FILE_DESCR);

    uint8_t descr = 0;
    if (!getFileDescr(memMngr.vterms[frag->offset].intNum, &descr))
        FMT_PRINT_AND_EXIT(BAD_DESCR, MAX_FILE_DESCR);

    frag->offset++;
    frag->length--;

    return descr;
}

static getFileDescr(struct v_int* bigInt, uint8_t* descr)
{
    if (bigInt->sign || bigInt->length > 1)
        return 0;

    *descr = *bigInt->bytes;

    return *descr < MAX_FILE_DESCR;
}

static void openFile(struct fragment_t* frag, uint8_t mode, uint8_t descr)
{
    char* fileName = 0;
    fileName = assemblyFileName(frag);

    if (!fileName)
    {
        openDefaultFile(descr, mode);
        return;
    }

    openFileWithName(fileName, mode, descr);

    free(fileName);
}

static void openDefaultFile(uint8_t descr, uint8_t mode)
{
    char* fileName = (char*)malloc(PATTERN_FILE_NAME_LENGHT);

    snprintf(fileName, PATTERN_FILE_NAME_LENGHT, FILE_NAME_PATTERN, descr);

    openFileWithName(fileName, mode, descr);

    free(fileName);
}

static void openFileWithName(char* fileName, uint8_t mode, uint8_t descr)
{
    if (files[descr].file)
        FMT_PRINT_AND_EXIT(DESCR_ALREADY_IN_USE, descr);

    if (descr == 0)
        FMT_PRINT_AND_EXIT(TRY_TO_TAKE_TERMINAL_DESCR, MAX_FILE_DESCR);

    files[descr].file = fopen(fileName, modeStr[mode]);

    if (!files[descr].file)
        FMT_PRINT_AND_EXIT(FILE_OPEN_ERROR, fileName, modeStr[mode], strerror(errno));

    files[descr].mode = mode;
}

static char* assemblyFileName(struct fragment_t* frag)
{
    uint64_t needBytesCount = calcNeedBytesCount(frag);
    char* fileName = (char*)malloc(needBytesCount + 1); // +1 for 0 character.
    char* pointer = fileName;

    uint64_t i = 0;
    for (i = 0; i < frag->length; ++i)
    {
        struct v_term* term = memMngr.vterms + frag->offset + i;

        switch (term->tag)
        {
            case V_BRACKET_CLOSE_TAG:
                *pointer++ = '(';
                break;

            case V_BRACKET_OPEN_TAG:
                *pointer++ = ')';
                break;

            case V_CHAR_TAG:
                pointer = writeAsUTF8ToBuff(term->ch, pointer);
                break;

            case V_IDENT_TAG:
                pointer = vStringToCharArr(term->str, pointer);
                break;

            case V_CLOSURE_TAG:
                pointer = vStringToCharArr(term->closure->ident, pointer);
                break;

            case V_INT_NUM_TAG:
                pointer = vIntToCharArr(term->intNum, pointer);
                break;

            case V_DOUBLE_NUM_TAG:
                pointer += sprintf(pointer, "%f", term->doubleNum);
                break;

            default:
                PRINT_AND_EXIT(BAD_VTERM);
        }
    }

    *pointer++ = 0;

    return fileName;
}

static uint64_t calcNeedBytesCount(struct fragment_t* frag)
{
    uint64_t needBytesCount = 0;
    uint64_t i = 0;
    for (i = 0; i < frag->length; ++i)
    {
        struct v_term* term = memMngr.vterms + frag->offset + i;

        switch (term->tag)
        {
            case V_BRACKET_CLOSE_TAG:
            case V_BRACKET_OPEN_TAG:
                ++needBytesCount;
                break;

            case V_CHAR_TAG:
                needBytesCount += sizeof(uint32_t);
                break;

            case V_IDENT_TAG:
                needBytesCount += calcBytesForVStringCharArr(term->str);
                break;

            case V_CLOSURE_TAG:
                needBytesCount += calcBytesForVStringCharArr(term->closure->ident);
                break;

            case V_INT_NUM_TAG:
                needBytesCount += calcBytesForIntCharArr(term->intNum);
                break;

            case V_DOUBLE_NUM_TAG:
                needBytesCount += snprintf(0, 0, "%f", term->doubleNum);
                break;
        }
    }

    return needBytesCount;
}

static uint64_t copySymbols(uint64_t first, uint64_t length)
{
    uint64_t firstOffset = memMngr.vtermsOffset;

    uint64_t i = 0;
    for (i = 0; i < length; ++i)
        allocateSymbolVTerm(memMngr.vterms[first + i].ch);

    return firstOffset;
}

static void printRange(FILE* file, struct fragment_t* frag)
{
	int i = 0;
	struct v_term* currTerm = memMngr.vterms + frag->offset;

	for (i = 0; i < frag->length; ++i)	
        printSymbol(file, currTerm + i);

    fprintf(file, "\n");
}

static void printSymbol(FILE* file, struct v_term* term)
{
	switch (term->tag)
	{
	case V_CHAR_TAG:
        printUTF32(file, term->ch);
		break;
	case V_IDENT_TAG:
        printUStr(file, term->str);
        fprintf(file, " ");
		break;
	case V_INT_NUM_TAG:
        printIntNumber(file, term->intNum);
		break;
    case V_DOUBLE_NUM_TAG:
        fprintf(file, "%lf ", term->doubleNum);
		break;
    case V_CLOSURE_TAG:
        printUStr(file, term->closure->ident);
		break;
    case V_BRACKET_OPEN_TAG:
        fprintf(file, "%c", '(' );
		break;
    case V_BRACKET_CLOSE_TAG:
        fprintf(file, "%c", ')' );
        break;
	}
}

/// Проверка на равенство двух строк. 1 - успех, 0 - неудача.
int ustrEq(struct v_string* a, struct v_string* b)
{
    if (!a || !b)
        return 0;

    if (a->length != b->length)
        return 0;

    uint64_t i = 0;
    for (i = 0; i < a->length; ++i)
    {
        if (a->head[i] != b->head[i])
            return 0;
    }

    return 1;
}

/// Проверка на равенство двух чисел. 1 - успех, 0 - неудача.
int intCmp(struct v_int* a, struct v_int* b)
{
    if (a->length == 1 && b->length == 1 &&
        a->bytes[0] == 0 && b->bytes[0] == 0)
        return 0;

    int invert = 1;

    if (a->sign > b->sign)
        return -1;

    if (a->sign < b->sign)
        return 1;

    if (a->sign)
        invert = -1;

    if (a->length > b->length)
        return 1 * invert;

    if (a->length < b->length)
        return -1 * invert;

    uint64_t i = 0;
    for (i = 0; i < a->length; ++i)
    {
        if (a->bytes[i] > b->bytes[i])
            return 1 * invert;

        if (a->bytes[i] < b->bytes[i])
            return -1 * invert;
    }

    return 0;
}

void printUStr(FILE* file, struct v_string* str)
{
    if (!str)
        return;

    uint64_t i = 0;

    for (i = 0; i < str->length; ++i)
        printUTF32(file, str->head[i]);
}


static void printIntNumber(FILE* file, struct v_int* intNum)
{
    mpz_t num;
    mpz_init(num);

    mpz_import(num, intNum->length, 1, sizeof(uint8_t), 1, 0, intNum->bytes);

    if (intNum->sign)
        mpz_mul_si(num, num, -1);

    gmp_printf("%Zd ", num);

    mpz_clear(num);
}
