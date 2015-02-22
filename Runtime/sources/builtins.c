#include <stdio.h>
#include <stdlib.h>

#include "ConvertUTF.h"
#include "vmachine.h"
#include "builtins.h"

#define N 256

static void printRange(struct fragment_t* frag);
static void printSymbol(struct v_term* term);
static void printUnicodeChar(uint32_t ch);

struct func_result_t Card(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView)
{
	char ch;
	uint32_t lastOffset;
	uint32_t firstOffset;
	struct lterm_t* mainChain = 0;

	if((ch = getchar()) != '\n')
	{
		firstOffset = lastOffset = allocateSymbol(ch);

		while ((ch = getchar()) != '\n')
			lastOffset = allocateSymbol(ch);

		mainChain = constructLterm(firstOffset, lastOffset - firstOffset + 1);
	}

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
	UTF32 source = ch;
	const UTF32 *pSource = &source;
	UTF8 target[UNI_MAX_UTF8_BYTES_PER_CODE_POINT];
	UTF8 *pTarget = target;

	ConversionResult cr;
	cr = ConvertUTF32toUTF8(&pSource, pSource + 1, &pTarget, pTarget+sizeof(target)-1, lenientConversion);

	if (conversionOK != cr)
		return;

	uint32_t bytesNum = getNumBytesForUTF8(target[0]);
	int i = 0;
	for (i = 0; i < bytesNum; ++i)
		printf("%c", target[i]);
}

