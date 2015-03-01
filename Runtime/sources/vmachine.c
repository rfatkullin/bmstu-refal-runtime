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
static RefalFunc GetFuncPointer(struct lterm_t* fieldOfView, struct lterm_t** params);

static struct v_string* constructVStringFromASCIIName(char* name)
{
    struct v_string* ptr = (struct v_string*)malloc(sizeof(struct v_string));
    ptr->length = strlen(name);
    ptr->head = (uint32_t*)malloc(ptr->length * sizeof(uint32_t));

    uint64_t i = 0;
    for (i = 0; i < ptr->length; ++i)
        ptr->head[i] = name[i];

    return ptr;
}

static struct func_call_t* ConstructStartFunc(const char* funcName, RefalFunc entryFuncPointer)
{
	struct func_call_t* goCall = (struct func_call_t*)malloc(sizeof(struct func_call_t));

	struct lterm_t* fieldOfView = (struct lterm_t*)malloc(sizeof(struct lterm_t));
	struct lterm_t* currTerm = (struct lterm_t*)malloc(sizeof(struct lterm_t));
	currTerm->tag = L_TERM_FRAGMENT_TAG;
	currTerm->fragment = (struct fragment_t*)malloc(sizeof(struct fragment_t));
	currTerm->fragment->offset = allocateClosure(entryFuncPointer, 0);
	currTerm->fragment->length = 1;
    memMngr.vterms[currTerm->fragment->offset].closure->ident = constructVStringFromASCIIName(funcName);

	fieldOfView->next = currTerm;
	fieldOfView->prev = currTerm;
	currTerm->prev = fieldOfView;
	currTerm->next = fieldOfView;

	goCall->env = (struct env_t*)malloc(sizeof(struct env_t));
	goCall->env->params = 0;

	goCall->fieldOfView = fieldOfView;
	goCall->entryPoint = 0;
	goCall->next = 0;

	return goCall;
}

static struct lterm_t* ConstructStartFieldOfView(const char* funcName, RefalFunc entryFuncPointer)
{
	struct lterm_t* fieldOfView = (struct lterm_t*)malloc(sizeof(struct lterm_t));
	struct lterm_t* term = (struct lterm_t*)malloc(sizeof(struct lterm_t));

	fieldOfView->next = term;
	fieldOfView->prev = term;
	term->prev = fieldOfView;
	term->next = fieldOfView;

	term->tag = L_TERM_FUNC_CALL;
	term->funcCall = ConstructStartFunc(funcName, entryFuncPointer);

	return term;
}

void mainLoop(const char* entryFuncName, RefalFunc entryFuncPointer)
{
	struct lterm_t* fieldOfView = ConstructStartFieldOfView(entryFuncName, entryFuncPointer);
	struct lterm_t* callTerm = fieldOfView;
	struct func_result_t funcRes;    
    struct lterm_t* parentCall = 0;

	while (callTerm)
	{
        if (callTerm->funcCall->funcPtr)
            callTerm->funcCall->fieldOfView = callTerm->funcCall->subCall;
        else
            callTerm->funcCall->funcPtr = GetFuncPointer(callTerm->funcCall->fieldOfView, &callTerm->funcCall->env->params);

		if (callTerm->funcCall->funcPtr == 0)
		{
            printf("[Error]: Func pointer is null!\n");
			exit(0);
		}

		funcRes = callTerm->funcCall->funcPtr(&callTerm->funcCall->entryPoint, callTerm->funcCall->env, callTerm->funcCall->fieldOfView);

        switch (funcRes.status)
        {
            case OK_RESULT:
                callTerm = updateFieldOfView(callTerm, &funcRes);
                break;

            case CALL_RESULT:
                parentCall = callTerm;
                callTerm = addFuncCallFiledOfView(callTerm, &funcRes);
                callTerm->funcCall->parentCall = parentCall;
                break;
            case FAIL_RESULT:
                if (!callTerm->funcCall->rollBack || !callTerm->funcCall->parentCall || callTerm->funcCall->failEntryPoint == -1)
                {
                    printf("[Error]: Bad func result!\n");
                    exit(0);
                }
                else
                {
                    callTerm->funcCall->parentCall->funcCall->entryPoint = callTerm->funcCall->failEntryPoint;
                    callTerm = parentCall;
                }
                break;
        }
	}
}

static RefalFunc GetFuncPointer(struct lterm_t* fieldOfView, struct lterm_t** params)
{
	if (fieldOfView == 0)
		return 0;

	if (fieldOfView->next->tag != L_TERM_FRAGMENT_TAG)
		return 0;

	if (memMngr.vterms[fieldOfView->next->fragment->offset].tag != V_CLOSURE_TAG)
		return 0;

    RefalFunc newFuncPointer = memMngr.vterms[fieldOfView->next->fragment->offset].closure->funcPtr;
    *params = memMngr.vterms[fieldOfView->next->fragment->offset].closure->env;
    struct v_string* ident = memMngr.vterms[fieldOfView->next->fragment->offset].closure->ident;

	//TO FIX:
	fieldOfView->next = fieldOfView->next->next;
    fieldOfView->next->next->prev = fieldOfView;

//    printUStr(ident);
//    printf("\n");

	return newFuncPointer;
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

	insertChain->next->prev = targetChain;
	insertChain->prev->next = targetChain;

	funcResult->callChain->prev->funcCall->next = currNode;

	//CHECK
	free(funcResult->fieldChain);
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
			funcResult->callChain->prev->funcCall->next = currNode->funcCall->next;
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
                uint64_t openBracketOffset = allocateOpenBracketVTerm(0);
				assemblyChain(currTerm->chain);
                changeBracketLength(openBracketOffset, memMngr.vtermsOffset - openBracketOffset + 1);
                allocateCloseBracketVTerm(memMngr.vtermsOffset - openBracketOffset + 1);
				break;
			}
		}

		currTerm = currTerm->next;
	}
}














