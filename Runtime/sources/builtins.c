#include <stdio.h>
#include <stdlib.h>

#include <unicode/ustdio.h>
#include <unicode/uchar.h>

#include "vmachine.h"
#include "builtins.h"

#define N 256
#define ARITHM_BASE "4294967296"

static void printRange(struct fragment_t* frag);
static void printSymbol(struct v_term* term);
static void printUnicodeChar(uint32_t ch);

struct func_result_t Card(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView)
{
    uint64_t firstOffset = memMngr.vtermsOffset;
	struct lterm_t* mainChain = 0;

	UChar ch = u_fgetc(input);
	while (ch != '\n')
	{
		allocateSymbol(ch);
		ch = u_fgetc(input);
	}

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
		printUnicodeChar(term->ch);
		break;
	case V_IDENT_TAG:
		printf("%s ", term->str);
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
	case V_BRACKET_TAG:
		printf("%c", term->inBracketLength > 0 ? '(' : ')');
		break;
	}
}

static void printUnicodeChar(uint32_t ch)
{
	u_fprintf(output, "%C", ch);
}

void initBuiltins()
{
	input = u_finit(stdin, NULL, "UTF-8");
	output = u_finit(stdout, NULL, "UTF-8");

	mpz_init_set_str(base, ARITHM_BASE, 10);
}

void deinitBuiltins()
{
	u_fclose(input);
	u_fclose(output);

	mpz_clear(base);
}
