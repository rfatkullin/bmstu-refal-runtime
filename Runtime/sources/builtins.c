#include <stdio.h>
#include <stdlib.h>

#include "vmachine.h"
#include "builtins.h"
#include "unicode_io.h"
#include "data_allocators.h"

#define N 256

static void printRange(struct fragment_t* frag);
static void printSymbol(struct v_term* term);
static void printUnicodeChar(uint32_t ch);
static void printIntNumber(struct v_int* num);
static uint64_t copySymbols(uint64_t first, uint64_t length);

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

/*
struct func_result_t Open(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView, int firstCall)
{
    struct fragment_t* frag = gcGetAssembliedChain(fieldOfView)->fragment;

    if (frag->length < 2)
    {
        printf("%s\n", TOO_FEW_ARGUMENTS);
        exit(0);
    }

    if (memMngr.vterms[frag->offset].tag != V_CHAR_TAG)
    {
        printf("%s Expected 'W','w', 'R' or 'r'\n", BAD_ARGUMENTS);
        exit(0);
    }

    int mode = -1;
    switch (memMngr.vterms[frag->offset].tag)
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
            printf("%s Expected 'W','w', 'R' or 'r'\n", BAD_ARGUMENTS);
            exit(0);
    }

    if (memMngr.vterms[frag->offset + 1].tag != V_INT_NUM_TAG)
    {
        printf("%s Expected descr number less than %d\n", BAD_ARGUMENTS, MAX_FILE_DESCR);
        exit(0);
    }

    uint8_t fileDescr = 0;
    if (!converToFileDescr(memMngr.vterms[frag->offset + 1].intNum, &fileDescr))
    {
        printf("%s Expected descr number less than %d\n", BAD_ARGUMENTS, MAX_FILE_DESCR);
        exit(0);
    }


}

struct func_result_t Get(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView, int firstCall)
{

}

struct func_result_t Put(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView, int firstCall)
{

}
*/
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
