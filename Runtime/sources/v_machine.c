#include <stdio.h>
#include <stdlib.h>

#include "func_call_t.h"
#include "v_machine.h"

static void printChainOfCalls(struct l_term* callTerm);
static void insertTermChainToFieldOfView(struct l_term* mainChain, struct l_term_chain_t* insertChain);
static struct l_term* insertFuncCallToCallChain(struct l_term* mainChain, struct l_term_chain_t* insertChain);

//struct l_term* createLTermFuncCall(const char* funcName, struct l_term* prev, struct l_term* (*func)(void* args), struct l_term* args, void* stackArgs)
//{
//	struct l_term* term = (struct l_term*)malloc(sizeof(struct l_term));

//	term->parent->prev = prev;
//	term->tag = L_TERM_FUNC_CALL;
//	term->funcCall = (struct func_call*)malloc(sizeof(func_call));

//	term->funcCall->funcName = funcName;
//	term->funcCall->funcPtr = func;
//	term->funcCall->range = arg;
//	return 0;
//}

static struct l_term_chain_t* ConstructEmptyLTermChain()
{
	struct l_term_chain_t* chain = (struct l_term_chain_t*)malloc(sizeof(struct l_term_chain_t));

	chain->begin = 0;
	chain->end = 0;

	return chain;
}

static struct func_call_t* ConstructStartFunc(struct func_result_t (*firstFuncPtr)(int entryPoint, struct env_t* env, struct field_view_t* fieldOfView))
{
	struct func_call_t* goCall = (struct func_call_t*)malloc(sizeof(struct func_call_t));

	goCall->funcPtr = firstFuncPtr;

	goCall->env = (struct env_t*)malloc(sizeof(struct env_t));
	goCall->env->params = 0;

	goCall->fieldOfView = (struct field_view_t*)malloc(sizeof(struct field_view_t));
	goCall->fieldOfView->current = 0;
	goCall->fieldOfView->backups = 0;

	goCall->entryPoint = 0;

	goCall->nextFuncCall = 0;

	return goCall;
}

static struct l_term* ConstructStartFieldOfView(struct func_result_t (*firstFuncPtr)(int entryPoint, struct env_t* env, struct field_view_t* fieldOfView))
{
	struct l_term* term = (struct l_term*)malloc(sizeof(struct l_term));

	term->tag = L_TERM_FUNC_CALL;
	term->funcCall = ConstructStartFunc(firstFuncPtr);
	term->prev = 0;
	term->next = 0;

	return term;
}

void mainLoop(struct func_result_t (*firstFuncPtr)(int entryPoint, struct env_t* env, struct field_view_t* fieldOfView))
{
	struct l_term* fieldOfView = ConstructStartFieldOfView(firstFuncPtr);
	struct l_term* fcTerm = fieldOfView;
	struct func_result_t funcRes;

	while (fcTerm)
	{
		printChainOfCalls(fcTerm);

		funcRes = fcTerm->funcCall->funcPtr(fcTerm->funcCall->entryPoint, fcTerm->funcCall->env, fcTerm->funcCall->fieldOfView);

		switch (funcRes.status)
		{
			case OK_RESULT:
				insertTermChainToFieldOfView(fieldOfView, funcRes.resChain);

				if (funcRes.callChain != 0)
					fcTerm = insertFuncCallToCallChain(fcTerm, funcRes.callChain);

				break;

			case CALL_RESULT:
				//TO DO
				break;

			case FAIL_RESULT:
				printf("Fail!\n");
				exit(1);
				break;
		}
	}
}

static void insertTermChainToFieldOfView(struct l_term* mainChain, struct l_term_chain_t* insertChain)
{
	mainChain->prev->next = insertChain->begin;
	insertChain->begin->prev = mainChain->prev;

	insertChain->end->next = mainChain->next;
	mainChain->next->prev = insertChain->end;
}

static struct l_term* insertFuncCallToCallChain(struct l_term* mainChain, struct l_term_chain_t* insertChain)
{
	insertChain->end->funcCall->nextFuncCall = mainChain->funcCall->nextFuncCall;

	//TO FIX: Пока так.
	free(mainChain);

	return insertChain->begin;
}

static void printChainOfCalls(struct l_term* callTerm)
{
	while (callTerm)
	{
		printf("%s%s", callTerm->funcCall->funcName, callTerm->funcCall->nextFuncCall ? "->" : "");
		callTerm = callTerm->funcCall->nextFuncCall;
	}

	printf("\n");
}