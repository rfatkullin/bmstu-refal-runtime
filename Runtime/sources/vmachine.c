#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include <func_call.h>
#include <vmachine.h>
#include <memory_manager.h>
#include <allocators/vterm_alloc.h>
#include <allocators/data_alloc.h>
#include <builtins/builtins.h>
#include <gc/gc.h>

static void printChainOfCalls(struct lterm_t* callTerm);
static struct lterm_t* updateFieldOfView(struct lterm_t* mainChain, struct func_result_t* funcResult);
static struct lterm_t* addFuncCallFiledOfView(struct lterm_t* currNode, struct func_result_t* funcResult);
static allocate_result assemblyChain(struct lterm_t* chain);
static struct lterm_t* createFieldOfViewForReCall(struct lterm_t* funcCall);
static RefalFunc getFuncPointer(struct lterm_t* callTerm);
static void onFuncFail(struct lterm_t** callTerm, int failResult);

uint64_t getHeapSize(int argc, char** argv)
{
    uint64_t heapSize = 0;
    int i = 0;

    for (i = 0; i < argc; ++i)
    {
        if (argv[i][0] == '-')
        {
            if (sscanf(argv[i], "-heapSize=%" PRIu64 , &heapSize) == 1)
            {
                printf("Heap size: %" PRIu64 "\n", heapSize);
                return heapSize;
            }
        }
    }

    return DEFAULT_HEAP_SIZE;
}

static struct vstring_t* constructVStringFromASCIIName(const char* name)
{
    struct vstring_t* ptr = (struct vstring_t*)malloc(sizeof(struct vstring_t));
    ptr->length = strlen(name);
    ptr->head = (uint32_t*)malloc(ptr->length * sizeof(uint32_t));

    // USE MEMSET
    uint64_t i = 0;
    for (i = 0; i < ptr->length; ++i)
        ptr->head[i] = name[i];

    return ptr;
}

static struct lterm_t* ConstructStartFunc(const char* funcName, RefalFunc entryFuncPointer)
{
    struct lterm_t* gofuncCallTerm = allocateFuncCallLTerm();
    gofuncCallTerm->funcCall->fieldOfView = allocateSimpleChain();

    struct lterm_t* fragTerm = allocateFragmentLTerm(1);
    fragTerm->fragment->offset = allocateClosureVTerm();
    fragTerm->fragment->length = 1;

    struct vstring_t* ident = constructVStringFromASCIIName(funcName);
    memMngr.vterms[fragTerm->fragment->offset].closure = gcAllocateClosureStruct(entryFuncPointer, 0, ident, 0);

    ADD_TO_CHAIN(gofuncCallTerm->funcCall->fieldOfView, fragTerm);

    return gofuncCallTerm;
}

static struct lterm_t* ConstructStartFieldOfView(const char* funcName, RefalFunc entryFuncPointer)
{
    struct lterm_t* fieldOfView = allocateSimpleChain();
    struct lterm_t* gofuncCallTerm = ConstructStartFunc(funcName, entryFuncPointer);

    ADD_TO_CHAIN(fieldOfView, gofuncCallTerm);

    return fieldOfView;
}

void mainLoop(const char* entryFuncName, RefalFunc entryFuncPointer)
{
    memMngr.fieldOfView = ConstructStartFieldOfView(entryFuncName, entryFuncPointer);

	struct func_result_t funcRes;    
    struct lterm_t* parentCall = 0;
    int entryStatus = 0;

    _currCallTerm = memMngr.fieldOfView->next;

    while (_currCallTerm)
	{
        // Указатель на функцию проставлен --> функция вызывается повторно.
        if (_currCallTerm->funcCall->funcPtr)
        {
            // Предыдущий результат успешен --> все скобки активации обработаны, можно передавать функции-потребителю.
            if (funcRes.status != FAIL_RESULT)
            {
                _currCallTerm->funcCall->fieldOfView = _currCallTerm->funcCall->subCall;
                _currCallTerm->funcCall->subCall = 0;
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
            _currCallTerm->funcCall->funcPtr = getFuncPointer(_currCallTerm);

            // Первый терм в скобках аткивации не является функциональным --> откат.
            if (_currCallTerm->funcCall->funcPtr == 0)
            {
                onFuncFail(&_currCallTerm, 0);
                entryStatus = ROLL_BACK;
            }
        }

        funcRes = CURR_FUNC_CALL->funcPtr(entryStatus);

        switch (funcRes.status)
        {
            case OK_RESULT:
                //printf("Before: ");
                //printFieldOfView(stdout, memMngr.fieldOfView);
                //printf("Insert chain: ");
                //if (funcRes.fieldChain)
                //    printFieldOfView(stdout, funcRes.fieldChain);
                _currCallTerm = updateFieldOfView(_currCallTerm, &funcRes);
                //printf("After: ");
                //printFieldOfView(stdout, memMngr.fieldOfView);
                break;

            case CALL_RESULT:
                parentCall = _currCallTerm;
                _currCallTerm = addFuncCallFiledOfView(_currCallTerm, &funcRes);
                _currCallTerm->funcCall->parentCall = parentCall;
                break;
            case FAIL_RESULT:
                onFuncFail(&_currCallTerm, 1);
                break;
        }
	}
}

static void onFuncFail(struct lterm_t** callTerm, int failResult)
{
    if ((failResult && !(*callTerm)->funcCall->rollback) || !(*callTerm)->funcCall->parentCall || (*callTerm)->funcCall->failEntryPoint == -1)
    {        
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

    struct vclosure_t* closure = memMngr.vterms[fieldOfView->next->fragment->offset].closure;

    RefalFunc newFuncPointer = closure->funcPtr;
    callTerm->funcCall->env->params = closure->params;
    callTerm->funcCall->env->paramsCount = closure->paramsCount;
    callTerm->funcCall->rollback = closure->rollback;

    /*
    if (closure->ident)
    {
        printf("^");
        printUStr(stdout, closure->ident);
        printf("^\n");
    }
    else
        printf("\n");    
    */

    // Remove fragment with closure => lost closure => GC will clean it.
    fieldOfView->next = fieldOfView->next->next;
    fieldOfView->next->prev = fieldOfView;

    //struct lterm_t* assembledFOV = gcGetAssembliedChain(callTerm->funcCall->fieldOfView);
    //printFragment(stdout, assembledFOV->fragment);

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

    if (funcResult->fieldChain)  // Insertn chain in the middle
	{		
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
    else
    {
        // Just exclude call node from fieldOfView
        currNode->prev->next = currNode->next;
        currNode->next->prev = currNode->prev;
    }

	return newCurrNode;
}

struct lterm_t* gcGetAssembliedChain(struct lterm_t* chain)
{    
	struct lterm_t* assembledChain = 0;

    if (chain != 0)
    {
        if (chain->tag != L_TERM_CHAIN_TAG)
            PRINT_AND_EXIT(ASSEMBLY_NOT_CHAIN);

        assembledChain = gcAllocateFragmentLTerm(1);

        if (chain->tag == GC_MOVED)
            chain = chain->prev;

        uint64_t offset = memMngr.vtermsOffset;

        if(assemblyChain(chain) == GC_NEED_CLEAN)
        {            
            collectGarbage();

            chain = chain->prev;
            offset = memMngr.vtermsOffset;

            if (assemblyChain(chain) == GC_NEED_CLEAN)
                PRINT_AND_EXIT(GC_MEMORY_OVERFLOW_MSG);

            if (GC_LTERM_OV(FRAGMENT_LTERM_SIZE(1)))
                PRINT_AND_EXIT(GC_MEMORY_OVERFLOW_MSG);

            assembledChain = allocateFragmentLTerm(1);
        }

        assembledChain->fragment->offset = offset;
        assembledChain->fragment->length = memMngr.vtermsOffset - offset;
    }

	return assembledChain;
}

static allocate_result assemblyChain(struct lterm_t* chain)
{
    struct lterm_t* currTerm = chain->next;

    while (currTerm != chain)
    {
        switch (currTerm->tag)
        {
            case L_TERM_FRAGMENT_TAG :
            {
                GC_RETURN_ON_FAIL(allocateVTerms(currTerm->fragment));
                break;
            }
            case L_TERM_CHAIN_KEEPER_TAG:
            {
                if (GC_VTERM_OV(1))
                    return GC_NEED_CLEAN;

                uint64_t openBracketOffset = allocateOpenBracketVTerm(0);

                GC_RETURN_ON_FAIL(assemblyChain(currTerm->chain));

                changeBracketLength(openBracketOffset, memMngr.vtermsOffset - openBracketOffset + 1);

                if (GC_VTERM_OV(1))
                    return GC_NEED_CLEAN;

                allocateCloseBracketVTerm(memMngr.vtermsOffset - openBracketOffset + 1);
                break;
            }

            case L_TERM_FUNC_CALL:            
                PRINT_AND_EXIT(FUNC_CALL_AT_ASSEMBLY);
                break;

            case L_TERM_CHAIN_TAG:
                PRINT_AND_EXIT(SIMPLE_CHAIN_AT_ASSEMBLY);
                break;

            default:                
                PRINT_AND_EXIT(BAD_TAG_AT_ASSEMBLY);
        }

        currTerm = currTerm->next;
    }

    return GC_OK;
}
