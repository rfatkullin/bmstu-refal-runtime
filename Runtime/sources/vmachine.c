#include <stdio.h>
#include <stdlib.h>

#include "func_call.h"
#include "vmachine.h"
#include "memory_manager.h"

static void printChainOfCalls(struct lterm_t* callTerm);
static struct lterm_t* updateFieldOfView(struct lterm_t* mainChain, struct func_result_t* funcResult);
static struct lterm_t* addFuncCallFiledOfView(struct lterm_t* currNode, struct func_result_t* funcResult);
static void assemblyChain(struct lterm_t* chain);
static void destroyFuncCallTerm(struct lterm_t* term);
static struct lterm_t* createFieldOfViewForReCall(struct lterm_t* funcCall);

static struct func_call_t* ConstructStartFunc(const char* funcName, struct func_result_t (*firstFuncPtr)(int entryPoint, struct env_t* env, struct lterm_t* fieldOfView),
	struct lterm_t* chain)
{
	struct func_call_t* goCall = (struct func_call_t*)malloc(sizeof(struct func_call_t));

	goCall->funcName = funcName;
	goCall->funcPtr = firstFuncPtr;

	goCall->env = (struct env_t*)malloc(sizeof(struct env_t));
	goCall->env->params = 0;

	goCall->fieldOfView = chain;
	goCall->entryPoint = 0;
	goCall->next = 0;

	return goCall;
}

static struct lterm_t* ConstructStartFieldOfView(struct func_result_t (*firstFuncPtr)(int entryPoint, struct env_t* env, struct lterm_t* fieldOfView))
{
	struct lterm_t* fieldOfView = (struct lterm_t*)malloc(sizeof(struct lterm_t));
	struct lterm_t* term = (struct lterm_t*)malloc(sizeof(struct lterm_t));

	fieldOfView->next = term;
	fieldOfView->prev = term;
	term->prev = fieldOfView;
	term->next = fieldOfView;

	term->tag = L_TERM_FUNC_CALL;
	term->funcCall = ConstructStartFunc("Go", firstFuncPtr, fieldOfView);

	return term;
}

void mainLoop(struct func_result_t (*firstFuncPtr)(int entryPoint, struct env_t* env, struct lterm_t* fieldOfView))
{
	struct lterm_t* fieldOfView = ConstructStartFieldOfView(firstFuncPtr);
	struct lterm_t* callTerm = fieldOfView;
	struct func_result_t funcRes;

	while (callTerm)
	{
		printChainOfCalls(callTerm);

		if (callTerm->funcCall->entryPoint != 0)
			createFieldOfViewForReCall(callTerm);

		funcRes = callTerm->funcCall->funcPtr(callTerm->funcCall->entryPoint, callTerm->funcCall->env, callTerm->funcCall->fieldOfView);

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

static struct lterm_t* createFieldOfViewForReCall(struct lterm_t* callTerm)
{
	//TO CHECK: освобождается ли память для поля current
	//callTerm->funcCall->fieldOfView = (struct lterm_t*)malloc(sizeof(struct lterm_t));
	//callTerm->funcCall->fieldOfView->begin = callTerm->funcCall->subCall->next;
	//callTerm->funcCall->fieldOfView->end = callTerm->funcCall->subCall->prev;
	return callTerm;
}

static struct lterm_t* addFuncCallFiledOfView(struct lterm_t* currNode, struct func_result_t* funcResult)
{
	if (!currNode->funcCall->subCall)
		currNode->funcCall->subCall = (struct lterm_t*)malloc(sizeof(struct lterm_t));

	struct lterm_t* insertChain = funcResult->fieldChain;
	struct lterm_t* targetChain = currNode->funcCall->subCall;
	struct lterm_t* newCallNode = funcResult->callChain->next;

	targetChain->next = insertChain->next;
	targetChain->prev = insertChain->prev;
	funcResult->callChain->prev->next = currNode;

	free(funcResult->callChain);

	return newCallNode;
}

static struct lterm_t* updateFieldOfView(struct lterm_t* currNode, struct func_result_t* funcResult)
{
	struct lterm_t* newCurrNode = currNode->funcCall->next;

	if (funcResult->fieldChain)
	{
		//Обновляем поле зрения
		struct lterm_t* insertChain = funcResult->fieldChain;

		currNode->prev->next = insertChain->next;
		insertChain->next->prev = currNode->prev;

		currNode->next->prev = insertChain->prev;
		insertChain->prev->next = currNode->next;

		//Обновляем цепочку вызовов
		if (funcResult->callChain)
		{
			newCurrNode = funcResult->callChain->next;
			funcResult->callChain->prev->next = currNode->funcCall->next;
		}
	}

	free(funcResult->callChain);
	free(funcResult->fieldChain);
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

struct lterm_t* getAssembliedChain(struct lterm_t* chain)
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
static void assemblyChain(struct lterm_t* chain)
{
	struct lterm_t* currTerm = chain->next;

	while (currTerm != chain)
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














