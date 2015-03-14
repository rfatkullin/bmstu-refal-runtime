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


#define N 256

static void printRange(struct fragment_t* frag);
static void printSymbol(struct v_term* term);
static void printUnicodeChar(uint32_t ch);
static void printIntNumber(struct v_int* num);
static uint64_t copySymbols(uint64_t first, uint64_t length);
static char* getFileName();
static char* assemblyFileName(struct fragment_t* frag);
static void openFile(struct fragment_t* frag, const char* modeStr, uint8_t mode);
static void findOutOpenMode(struct fragment_t* frag, const char** modeStr, uint8_t* mode);
static uint64_t calcNeedBytesCount(struct fragment_t* frag);

struct func_result_t Card(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView, int firstCall)
{
    uint64_t firstOffset = memMngr.vtermsOffset;
    uint64_t currOffset =  memMngr.vtermsOffset;
	struct lterm_t* mainChain = 0;

    checkAndCleanData(BUILTINS_RESULT_SIZE);

    uint32_t ch;
    while ((ch = readUTF8Char()) != '\n')
    {
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

	if (firstOffset != memMngr.vtermsOffset)
        mainChain = allocateBuiltinsResult(firstOffset, memMngr.vtermsOffset - firstOffset);

	return (struct func_result_t){.status = OK_RESULT, .fieldChain = mainChain, .callChain = 0};
}

struct func_result_t Prout(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView, int firstCall)
{
    struct lterm_t* currExpr = gcGetAssembliedChain(fieldOfView);

	printRange(currExpr->fragment);

	return (struct func_result_t){.status = OK_RESULT, .fieldChain = 0, .callChain = 0};
}

struct func_result_t Open(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView, int firstCall)
{
    struct fragment_t* frag = gcGetAssembliedChain(fieldOfView)->fragment;

    if (frag->length < 2)
        PRINT_AND_EXIT(TOO_FEW_ARGUMENTS);

    if (memMngr.vterms[frag->offset].tag != V_CHAR_TAG)
        PRINT_AND_EXIT(BAD_FILE_OPEN_MODE);

    const char* modeStr = 0;
    uint8_t mode = 0;

    findOutOpenMode(frag, &modeStr, &mode);
    openFile(frag, modeStr, mode);

    return (struct func_result_t){.status = OK_RESULT, .fieldChain = 0, .callChain = 0};
}

static void findOutOpenMode(struct fragment_t* frag, const char** modeStr, uint8_t* mode)
{
    switch (memMngr.vterms[frag->offset].tag)
    {
        case 'w':
        case 'W':
            *modeStr = WRITE_MODE_STR;
            *mode = WRITE_MODE;
            break;

        case 'r':
        case 'R':
            *modeStr = READ_MODE_STR;
            *mode = READ_MODE;
           break;

        default:
            PRINT_AND_EXIT(BAD_FILE_OPEN_MODE);
    }

    frag->offset++;
    frag->length--;
}

static void openFile(struct fragment_t* frag, const char* modeStr, uint8_t mode)
{
    if (memMngr.vterms[frag->offset].tag != V_INT_NUM_TAG)
        FMT_PRINT_AND_EXIT(BAD_ARGUMENTS, MAX_FILE_DESCR);

    uint8_t descr = 0;
    if (!convertToFileDescr(memMngr.vterms[frag->offset].intNum, &descr))
        FMT_PRINT_AND_EXIT(BAD_ARGUMENTS, MAX_FILE_DESCR);

    if (files[descr].file)
        FMT_PRINT_AND_EXIT(DESCR_ALREADY_IN_USE, descr);

    char* fileName = 0;
    fileName = assemblyFileName(frag);

    if (!fileName)
    {
        fileName = (char*)malloc(PATTERN_FILE_NAME_LENGHT);
        snprintf(fileName, PATTERN_FILE_NAME_LENGHT, FILE_NAME_PATTERN, descr);
    }

    files[descr].file = fopen(fileName, modeStr);

    if (!files[descr].file)
        FMT_PRINT_AND_EXIT(FILE_OPEN_ERROR, fileName, modeStr, strerror(errno));

    files[descr].mode = mode;
    free(fileName);
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

struct func_result_t Get(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView, int firstCall)
{

}

struct func_result_t Put(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView, int firstCall)
{

}

static uint64_t copySymbols(uint64_t first, uint64_t length)
{
    uint64_t firstOffset = memMngr.vtermsOffset;

    uint64_t i = 0;
    for (i = 0; i < length; ++i)
        allocateSymbolVTerm(memMngr.vterms[first + i]);

    return firstOffset;
}

static void printRange(struct fragment_t* frag)
{
	int i = 0;
	struct v_term* currTerm = memMngr.vterms + frag->offset;

	for (i = 0; i < frag->length; ++i)	
		printSymbol(currTerm + i);	

	printf("\n");
}

static void printSymbol(struct v_term* term)
{
	switch (term->tag)
	{
	case V_CHAR_TAG:
        printUTF32(term->ch);
		break;
	case V_IDENT_TAG:
        printUStr(term->str);
        printf(" ");
		break;
	case V_INT_NUM_TAG:
        printIntNumber(term->intNum);
		break;
    case V_DOUBLE_NUM_TAG:
        printf("%lf ", term->doubleNum);
		break;
    case V_CLOSURE_TAG:
        printUStr(term->closure->ident);
		break;
    case V_BRACKET_OPEN_TAG:
        printf("%c", '(' );
		break;
    case V_BRACKET_CLOSE_TAG:
        printf("%c", ')' );
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

void printUStr(struct v_string* str)
{
    if (!str)
        return;

    uint64_t i = 0;

    for (i = 0; i < str->length; ++i)
        printUTF32(str->head[i]);
}


static void printIntNumber(struct v_int* intNum)
{
    mpz_t num;
    mpz_init(num);

    mpz_import(num, intNum->length, 1, sizeof(uint8_t), 1, 0, intNum->bytes);

    if (intNum->sign)
        mpz_mul_si(num, num, -1);

    gmp_printf("%Zd ", num);

    mpz_clear(num);
}
