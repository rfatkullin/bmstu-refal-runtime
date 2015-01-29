#include <stdio.h>
#include <stdlib.h>

#include "func_call.h"
#include "vmachine.h"
#include "memory_manager.h"

static void printChainOfCalls(struct lterm_t* callTerm);
static struct lterm_t* updateFieldOfView(struct lterm_t* mainChain, struct func_result_t* funcResult);
static struct lterm_t* addFuncCallFiledOfView(struct lterm_t* currNode, struct func_result_t* funcResult);
static void assemblyChain(struct lterm_chain_t* chain);
static void destroyFuncCallTerm(struct lterm_t* term);
static struct lterm_chain_t* createFieldOfViewForReCall(struct lterm_t* funcCall);

static struct lterm_chain_t* ConstructEmptyLTermChain()
{
	struct lterm_chain_t* chain = (struct lterm_chain_t*)malloc(sizeof(struct lterm_chain_t));

	chain->begin = 0;
	chain->end = 0;

	return chain;
}

static struct func_call_t* ConstructStartFunc(const char* funcName, struct func_result_t (*firstFuncPtr)(int entryPoint, struct env_t* env, struct lterm_chain_t* fieldOfView),
	struct lterm_chain_t* chain)
{
	struct func_call_t* goCall = (struct func_call_t*)malloc(sizeof(struct func_call_t));

	goCall->funcName = funcName;
	goCall->funcPtr = firstFuncPtr;

	goCall->env = (struct env_t*)malloc(sizeof(struct env_t));
	goCall->env->params = 0;

	goCall->inFieldOfView = chain;
	goCall->entryPoint = 0;
	goCall->next = 0;

	return goCall;
}

static struct lterm_t* ConstructStartFieldOfView(struct func_result_t (*firstFuncPtr)(int entryPoint, struct env_t* env, struct lterm_chain_t* fieldOfView))
{
	struct lterm_chain_t* fieldOfView = (struct lterm_chain_t*)malloc(sizeof(struct lterm_chain_t));
	struct lterm_t* term = (struct lterm_t*)malloc(sizeof(struct lterm_t));

	fieldOfView->begin = term;
	fieldOfView->end = term;

	term->tag = L_TERM_FUNC_CALL;
	term->funcCall = ConstructStartFunc("Go", firstFuncPtr, fieldOfView);
	term->prev = 0;
	term->next = 0;

	return term;
}

void mainLoop(struct func_result_t (*firstFuncPtr)(int entryPoint, struct env_t* env, struct lterm_chain_t* fieldOfView))
{
	struct lterm_t* fieldOfView = ConstructStartFieldOfView(firstFuncPtr);
	struct lterm_t* callTerm = fieldOfView;
	struct func_result_t funcRes;

	while (callTerm)
	{
		printChainOfCalls(callTerm);

		if (callTerm->funcCall->entryPoint != 0)
			createFieldOfViewForReCall(callTerm);

		funcRes = callTerm->funcCall->funcPtr(callTerm->funcCall->entryPoint, callTerm->funcCall->env, callTerm->funcCall->inFieldOfView);

		switch (funcRes.status)
		{
			case OK_RESULT:

				callTerm = updateFieldOfView(callTerm, &funcRes);
				break;

			case CALL_RESULT:
				callTerm = addFuncCallFiledOfView(callTerm, &funcRes);
				break;

			case FAIL_RESULT:
				printf("Fail!\n");
				exit(1);
				break;
		}
	}
}

static struct lterm_chain_t* createFieldOfViewForReCall(struct lterm_t* callTerm)
{
	//TO CHECK: освобождается ли память для поля current
	callTerm->funcCall->inFieldOfView = (struct lterm_chain_t*)malloc(sizeof(struct lterm_chain_t));
	callTerm->funcCall->inFieldOfView->begin = callTerm->funcCall->subCall->next;
	callTerm->funcCall->inFieldOfView->end = callTerm->funcCall->subCall->prev;
}

static struct lterm_t* addFuncCallFiledOfView(struct lterm_t* currNode, struct func_result_t* funcResult)
{
	if (!currNode->funcCall->subCall)
		currNode->funcCall->subCall = (struct lterm_t*)malloc(sizeof(struct lterm_t));

	currNode->funcCall->subCall->next = funcResult->fieldChain->begin;
	funcResult->fieldChain->begin->prev = currNode->funcCall->subCall;

	currNode->funcCall->subCall->prev = funcResult->fieldChain->end;
	funcResult->fieldChain->end->next = currNode->funcCall->subCall;

	funcResult->callChain->end->funcCall->next = currNode;

	return funcResult->callChain->begin;
}

static struct lterm_t* updateFieldOfView(struct lterm_t* currNode, struct func_result_t* funcResult)
{
	if (funcResult->fieldChain)
	{
		//Обновляем поле зрения
		struct lterm_chain_t* insertChain = funcResult->fieldChain;

		insertChain->begin->prev = currNode->prev;
		if (currNode->prev)
			currNode->prev->next = insertChain->begin;

		insertChain->end->next = currNode->next;
		if (currNode->next)
			currNode->next->prev = insertChain->end;

		//Обновляем цепочку вызовов
		if (funcResult->callChain)
		{
			insertChain = funcResult->callChain;
			insertChain->end->funcCall->next = currNode->funcCall->next;
			currNode->funcCall->next = insertChain->begin;
		}
	}

	struct lterm_t* newCurrNode = currNode->funcCall->next;

	destroyFuncCallTerm(currNode);

	return newCurrNode;
}


//TO FIX: Освободить всю структуру.
static void destroyFuncCallTerm(struct lterm_t* term)
{

	free(term->funcCall->subCall);
	free(term);
}

static void printChainOfCalls(struct lterm_t* callTerm)
{
	printf("[Debug]Call chain: ");
	while (callTerm)
	{
		if (callTerm->funcCall)
		{
			printf("%s%s", callTerm->funcCall->funcName, callTerm->funcCall->next ? "->" : "");
			callTerm = callTerm->funcCall->next;
		}
		else
		{
			printf("[Error]: Bad func call term!\n");
			break;
		}
	}

	printf("\n");
}

struct lterm_t* getAssembliedChain(struct lterm_chain_t* chain)
{
	struct lterm_t* assembledChain = 0;

	if (chain != 0)
	{
		assembledChain = (struct lterm_t*)malloc(sizeof(struct lterm_t));
		assembledChain->tag = L_TERM_FRAGMENT_TAG;
		assembledChain->fragment = (struct fragment_t*)malloc(sizeof(struct fragment_t));
		assembledChain->fragment->offset = memMngr.vtermsOffset;

		assemblyChain(chain);

		assembledChain->fragment->length = memMngr.vtermsOffset - assembledChain->fragment->offset;
	}

	return assembledChain;
}

// TO FIX: Пока рекурсивно!
static void assemblyChain(struct lterm_chain_t* chain)
{
	struct lterm_t* currTerm = chain->begin;

	while (currTerm)
	{
		switch (currTerm->tag)
		{
			case L_TERM_FRAGMENT_TAG :
				allocateVTerms(currTerm->fragment);
				break;

			case L_TERM_CHAIN_TAG:
			{
				uint32_t leftBracketOffset = allocateBracketVTerm(0);
				assemblyChain(currTerm->chain);
				changeBracketLength(leftBracketOffset, memMngr.vtermsOffset - leftBracketOffset);
				allocateBracketVTerm(0);
				break;
			}
		}

		currTerm = currTerm->next;
	}
}














