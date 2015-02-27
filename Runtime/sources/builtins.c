#include <stdio.h>
#include <stdlib.h>

#include "vmachine.h"
#include "builtins.h"
#include "input.h"

#define N 256
#define ARITHM_BASE "4294967296"

static void printRange(struct fragment_t* frag);
static void printSymbol(struct v_term* term);
static void printUnicodeChar(uint32_t ch);
static void printUStr(struct v_string* str);

struct func_result_t Card(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView)
{
    uint64_t firstOffset = memMngr.vtermsOffset;
	struct lterm_t* mainChain = 0;

    uint32_t ch;
    while ((ch = readUTF8Char()) != '\n')
		allocateSymbol(ch);    


	if (firstOffset != memMngr.vtermsOffset)
		mainChain = constructLterm(firstOffset, memMngr.vtermsOffset - firstOffset);

	return (struct func_result_t){.status = OK_RESULT, .fieldChain = mainChain, .callChain = 0};
}

struct func_result_t Prout(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView)
{
	struct lterm_t* currExpr = getAssembliedChain(fieldOfView);

	printRange(currExpr->fragment);

	return (struct func_result_t){.status = OK_RESULT, .fieldChain = 0, .callChain = 0};
}

static void printRange(struct fragment_t* frag)
{
	int i = 0;
	struct v_term* currTerm = memMngr.vterms + frag->offset;

	for (i = 0; i < frag->length; ++i)
	{
		printSymbol(currTerm + i);
	}

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
		printf("%u ", term->intNum);
		break;
	case V_FLOAT_NUM_TAG:
		printf("%f ", term->floatNum);
		break;
	case V_CLOSURE_TAG:
		//TO DO
		break;
    case V_BRACKET_OPEN_TAG:
        printf("%c", '(' );
		break;
    case V_BRACKET_CLOSE_TAG:
        printf("%c", ')' );
        break;
	}
}


void initBuiltins()
{
	mpz_init_set_str(base, ARITHM_BASE, 10);
}

void deinitBuiltins()
{
	mpz_clear(base);
}

/// Сравнение двух строк. 1 - успех, 0 - неудача.
int UStrCmp(struct v_string* a, struct v_string* b)
{
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


static void printUStr(struct v_string* str)
{
    uint64_t i = 0;

    for (i = 0; i < str->length; ++i)
        printUTF32(str->head[i]);
}
