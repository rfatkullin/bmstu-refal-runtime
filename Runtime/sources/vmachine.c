#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "func_call.h"
#include "vmachine.h"
#include "memory_manager.h"

static void printChainOfCalls(struct lterm_t* callTerm);
static struct lterm_t* updateFieldOfView(struct lterm_t* mainChain, struct func_result_t* funcResult);
static struct lterm_t* addFuncCallFiledOfView(struct lterm_t* currNode, struct func_result_t* funcResult);
static void assemblyChain(struct lterm_t* chain);
static struct lterm_t* createFieldOfViewForReCall(struct lterm_t* funcCall);
static RefalFunc GetFuncPointer(struct lterm_t* fieldOfView, struct lterm_t** params);
static void OnFuncFail(struct lterm_t** callTerm, int failResult);

static struct v_string* constructVStringFromASCIIName(const char* name)
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
    int entryStatus = 0;

	while (callTerm)
	{
        // Указатель на функцию проставлен --> функция вызывается повторно.
        if (callTerm->funcCall->funcPtr)
        {
            // Предыдущий результат успешен --> все скобки активации обработаны, можно передавать функции-потребителю.
            if (funcRes.status != FAIL_RESULT)
            {
                callTerm->funcCall->fieldOfView = callTerm->funcCall->subCall;
                entryStatus = NEXT_ENTRYPOINT;
            }
            else // Обработка скобок активаций завершилась неудачно --> откат.
            {
                entryStatus = ROLL_BACK;
            }
        }
        else // Указатель на функцию не проставлен --> первый вызов.
        {
            entryStatus = FIRST_CALL;
            callTerm->funcCall->funcPtr = GetFuncPointer(callTerm->funcCall->fieldOfView, &callTerm->funcCall->env->params);

            // Первый терм в скобках аткивации не является функциональным --> откат.
            if (callTerm->funcCall->funcPtr == 0)
            {
                OnFuncFail(&callTerm, 0);
                entryStatus = ROLL_BACK;
            }
        }

        funcRes = callTerm->funcCall->funcPtr(&callTerm->funcCall->entryPoint, callTerm->funcCall->env, callTerm->funcCall->fieldOfView, entryStatus);

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
                OnFuncFail(&callTerm, 1);
                break;
        }
	}
}

static void OnFuncFail(struct lterm_t** callTerm, int failResult)
{
    if ((failResult && !(*callTerm)->funcCall->rollBack) || !(*callTerm)->funcCall->parentCall || (*callTerm)->funcCall->failEntryPoint == -1)
    {
        printf("%s\n", FUNC_CALL_FAILED);
        exit(0);
    }
    else
    {
        (*callTerm)->funcCall->parentCall->funcCall->entryPoint = (*callTerm)->funcCall->failEntryPoint;
        (*callTerm) = (*callTerm)->funcCall->parentCall;
    }
}

static RefalFunc GetFuncPointer(struct lterm_t* fieldOfView, struct lterm_t** params)
{
    //Fatal error!
    if (fieldOfView == 0)
    {
        printf("%s\n", BAD_EVAL_EXPR);
        exit(0);
    }

    if (fieldOfView->next == fieldOfView)
        return 0;

    //Fatal error!
    if (fieldOfView->next->tag != L_TERM_FRAGMENT_TAG)
    {
        printf("%s\n", BAD_EVAL_EXPR);
        exit(0);
    }

    if (fieldOfView->next->fragment->length == 0)
        return 0;

    //Fatal error!
	if (memMngr.vterms[fieldOfView->next->fragment->offset].tag != V_CLOSURE_TAG)
    {
        printf("%s\n", BAD_EVAL_EXPR);
        exit(0);
    }

    struct v_closure* closure = memMngr.vterms[fieldOfView->next->fragment->offset].closure;

    RefalFunc newFuncPointer = closure->funcPtr;
    *params = closure->params;

//    struct v_string* ident = memMngr.vterms[fieldOfView->next->fragment->offset].closure->ident;
//    printUStr(ident);
//    printf("\n");

    // Удаляем функциональный терм из поля зрения.
    fieldOfView->next = fieldOfView->next->next;
    fieldOfView->next->prev = fieldOfView;

	return newFuncPointer;
}

static struct lterm_t* addFuncCallFiledOfView(struct lterm_t* currNode, struct func_result_t* funcResult)
{
    currNode->funcCall->subCall = funcResult->fieldChain;
	struct lterm_t* newCallNode = funcResult->callChain->next;
	funcResult->callChain->prev->funcCall->next = currNode;

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

	return newCurrNode;
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














