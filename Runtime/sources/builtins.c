#include <stdio.h>
#include <stdlib.h>

#include "vmachine.h"
#include "builtins.h"

#define N 256

static void printRange(struct fragment_t* frag);

struct func_result_t Card(int entryPoint, struct env_t* env, struct lterm_t* fieldOfView)
{
	char ch;
	uint32_t lastOffset;
	uint32_t firstOffset;
	struct lterm_t* mainChain = 0;

	if((ch = getchar()) != '\n')
	{
		firstOffset = lastOffset = allocateSymbol(ch);

		while ((ch = getchar()) != '\n')
		{
			lastOffset = allocateSymbol(ch);
		}

		struct lterm_t* inputFragment = (struct lterm_t*)malloc(sizeof(struct lterm_t));
		inputFragment->tag = L_TERM_FRAGMENT_TAG;
		inputFragment->fragment = (struct fragment_t*)malloc(sizeof(struct fragment_t));
		inputFragment->fragment->offset = firstOffset;
		inputFragment->fragment->length = lastOffset - firstOffset + 1;

		mainChain = (struct lterm_t*)malloc(sizeof(struct lterm_t));
		mainChain->next = inputFragment;
		mainChain->prev = inputFragment;
		inputFragment->next = mainChain;
		inputFragment->prev = mainChain;
	}

	return (struct func_result_t){.status = OK_RESULT, .fieldChain = mainChain, .callChain = 0};
}

struct func_result_t Prout(int entryPoint, struct env_t* env, struct lterm_t* fieldOfView)
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
