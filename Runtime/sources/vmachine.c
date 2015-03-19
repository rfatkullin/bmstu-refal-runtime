#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <func_call.h>
#include <vmachine.h>
#include <memory_manager.h>
#include <allocators/vterm_alloc.h>
#include <allocators/data_alloc.h>
#include <builtins/builtins.h>

static void printChainOfCalls(struct lterm_t* callTerm);
static struct lterm_t* updateFieldOfView(struct lterm_t* mainChain, struct func_result_t* funcResult);
static struct lterm_t* addFuncCallFiledOfView(struct lterm_t* currNode, struct func_result_t* funcResult);
static int assemblyChain(struct lterm_t* chain);
static struct lterm_t* createFieldOfViewForReCall(struct lterm_t* funcCall);
static RefalFunc getFuncPointer(struct lterm_t* callTerm);
static void onFuncFail(struct lterm_t** callTerm, int failResult);

static struct v_string* constructVStringFromASCIIName(const char* name)
{
    struct v_string* ptr = (struct v_string*)malloc(sizeof(struct v_string));
    ptr->length = strlen(name);
    ptr->head = (uint32_t*)malloc(ptr->length * sizeof(uint32_t));

    // USE MEMSET
    uint64_t i = 0;
    for (i = 0; i < ptr->length; ++i)
        ptr->head[i] = name[i];

    return ptr;
}

// TO FIX: Проверки на переполнение.
static struct func_call_t* ConstructStartFunc(const char* funcName, RefalFunc entryFuncPointer)
{
	struct func_call_t* goCall = (struct func_call_t*)malloc(sizeof(struct func_call_t));

	struct lterm_t* fieldOfView = (struct lterm_t*)malloc(sizeof(struct lterm_t));
	struct lterm_t* currTerm = (struct lterm_t*)malloc(sizeof(struct lterm_t));
    struct v_string* ident = constructVStringFromASCIIName(funcName);
	currTerm->tag = L_TERM_FRAGMENT_TAG;
	currTerm->fragment = (struct fragment_t*)malloc(sizeof(struct fragment_t));
    currTerm->fragment->offset = allocateClosureVTerm();
    memMngr.vterms[currTerm->fragment->offset].closure = gcAllocateClosureStruct(entryFuncPointer, 0, ident, 0);
	currTerm->fragment->length = 1;
    memMngr.vterms[currTerm->fragment->offset].closure->ident = ident;

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
    memMngr.fieldOfView = ConstructStartFieldOfView(entryFuncName, entryFuncPointer);
    struct lterm_t* callTerm = memMngr.fieldOfView;
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
            callTerm->funcCall->funcPtr = getFuncPointer(callTerm);

            // Первый терм в скобках аткивации не является функциональным --> откат.
            if (callTerm->funcCall->funcPtr == 0)
            {
                onFuncFail(&callTerm, 0);
                entryStatus = ROLL_BACK;
            }
        }

        _currFuncCall = callTerm->funcCall;
        funcRes = callTerm->funcCall->funcPtr(entryStatus);

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
                onFuncFail(&callTerm, 1);
                break;
        }
	}
}

static void onFuncFail(struct lterm_t** callTerm, int failResult)
{
    if ((failResult && !(*callTerm)->funcCall->rollback) || !(*callTerm)->funcCall->parentCall || (*callTerm)->funcCall->failEntryPoint == -1)
    {
        // TO FIX: Must work!
        // struct lterm_t* assembledFOV = gcGetAssembliedChain(memMngr.fieldOfView);
        // printFragment(stdout, assembledFOV->fragment);
        PRINT_AND_EXIT(FUNC_CALL_FAILED);
    }
    else
    {
        (*callTerm)->funcCall->parentCall->funcCall->entryPoint = (*callTerm)->funcCall->failEntryPoint;
        (*callTerm) = (*callTerm)->funcCall->parentCall;
    }
}

static RefalFunc getFuncPointer(struct lterm_t* callTerm)
{
    struct lterm_t* fieldOfView = callTerm->funcCall->fieldOfView;

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
    callTerm->funcCall->env->params = closure->params;
    callTerm->funcCall->env->paramsCount = closure->paramsCount;
    callTerm->funcCall->rollback = closure->rollback;

    if (closure->ident)
    {
        printf("^");
        printUStr(stdout, closure->ident);
        printf("^\n");
    }
    else
        printf("Builtin!\n");

    // Remove fragment with closure => lost closure => GC will clean it.
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

struct lterm_t* gcGetAssembliedChain(struct lterm_t* chain)
{    
	struct lterm_t* assembledChain = 0;

    if (chain != 0)
    {
        assembledChain = gcAllocateFragmentLTerm(1);
        assembledChain->fragment->offset = memMngr.vtermsOffset;

        if(!assemblyChain(chain))
        {            
            collectGarbage();

            if (!assemblyChain(chain))
            {
                printf("%s\n", MEMORY_OVERFLOW);
                exit(0);
            }
        }

        assembledChain->fragment->length = memMngr.vtermsOffset - assembledChain->fragment->offset;
    }

	return assembledChain;
}

static int assemblyChain(struct lterm_t* chain)
{
    struct lterm_t* currTerm = chain->next;

    while (currTerm != chain)
    {
        switch (currTerm->tag)
        {
            case L_TERM_FRAGMENT_TAG :
            {
                if (!allocateVTerms(currTerm->fragment))
                    return 0;
                break;
            }
            case L_TERM_CHAIN_TAG:
            {
                if (memMngr.vtermsOffset + 1 > memMngr.vtermsMaxOffset)
                    return 0;

                uint64_t openBracketOffset = allocateOpenBracketVTerm(0);

                if (!assemblyChain(currTerm->chain))
                    return 0;

                changeBracketLength(openBracketOffset, memMngr.vtermsOffset - openBracketOffset + 1);

                if (memMngr.vtermsOffset + 1 > memMngr.vtermsMaxOffset)
                    return 0;

                allocateCloseBracketVTerm(memMngr.vtermsOffset - openBracketOffset + 1);
                break;
            }
        }

        currTerm = currTerm->next;
    }

    return 1;
}














