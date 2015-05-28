#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include <gc/gc.h>
#include <vmachine.h>
#include <func_call.h>
#include <debug_print.h>
#include <memory_manager.h>
#include <builtins/builtins.h>
#include <defines/gc_macros.h>
#include <allocators/data_alloc.h>
#include <allocators/data_alloc.h>
#include <allocators/vterm_alloc.h>
#include <defines/data_struct_sizes.h>

static RefalFunc getFuncPointer(struct lterm_t* callTerm);
static struct lterm_t* onFuncFail(struct lterm_t* callTerm, int failResult);
static uint64_t chAssemblyChain(struct lterm_t* chain, uint64_t* length, allocate_result* res);
static struct lterm_t* constructStartFieldOfView(const char* funcName, RefalFunc entryFuncPointer);
static struct lterm_t* updateFieldOfView(struct lterm_t* currNode, struct func_result_t* funcResult);
static struct lterm_t* addFuncCallFiledOfView(struct lterm_t* currNode, struct func_result_t* funcResult);

void mainLoop(const char* entryFuncName, RefalFunc entryFuncPointer)
{
    _memMngr.fieldOfView = constructStartFieldOfView(entryFuncName, entryFuncPointer);

	struct func_result_t funcRes;    
    struct lterm_t* parentCall = 0;
    int entryStatus = 0;    

    _currCallTerm = _memMngr.fieldOfView->next;

    while (_currCallTerm)
    {
        // Указатель на функцию проставлен --> функция вызывается повторно.
        if (_currCallTerm->funcCall->funcPtr)
        {            
            // Предыдущий результат успешен --> все скобки активации обработаны, можно передавать функции-потребителю.
            if (funcRes.status != FAIL_RESULT)
            {                                
                _currCallTerm->funcCall->env->workFieldOfView = _currCallTerm->funcCall->subCall;
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
                _currCallTerm = onFuncFail(_currCallTerm, 0);
                entryStatus = ROLL_BACK;
            }
        }

        funcRes = CURR_FUNC_CALL->funcPtr(entryStatus);

        switch (funcRes.status)
        {
            case OK_RESULT:                
                _currCallTerm = updateFieldOfView(_currCallTerm, &funcRes);
                break;

            case CALL_RESULT:
                parentCall = _currCallTerm;
                _currCallTerm = addFuncCallFiledOfView(_currCallTerm, &funcRes);
                _currCallTerm->funcCall->parentCall = parentCall;
                break;
            case FAIL_RESULT:
                _currCallTerm = onFuncFail(_currCallTerm, 1);
                break;
        }

        // Обновляем счетчик вызовов функций, который используется встр.ф. Step.
        mpz_add_ui(_step, _step, 1);
	}
}

static struct lterm_t* onFuncFail(struct lterm_t* callTerm, int failResult)
{
    if ((failResult && !callTerm->funcCall->rollback) || !callTerm->funcCall->parentCall || callTerm->funcCall->failEntryPoint == -1)
    {
        printf(FUNC_CALL_FAILED);
        printf("Func call term:\n");
        printFuncCallLn(stdout, callTerm->funcCall);
        printf("Field of view:\n");
        printFieldOfView(stdout, _memMngr.fieldOfView);
        exit(0);
    }

    callTerm->funcCall->parentCall->funcCall->entryPoint = callTerm->funcCall->failEntryPoint;

    if (callTerm->funcCall->parentCall->funcCall->fieldOfView == 0)
        callTerm->funcCall->parentCall->funcCall->fieldOfView = callTerm->funcCall->fieldOfView;

    return callTerm->funcCall->parentCall;
}

static void printUStr(FILE* file, struct vstring_t* str);
static RefalFunc getFuncPointer(struct lterm_t* callTerm)
{
    struct lterm_t* fieldOfView = callTerm->funcCall->fieldOfView;

    if (fieldOfView == 0)
        PRINT_AND_EXIT(BAD_EVAL_EXPR);

    // There is no lterms.
    if (fieldOfView->next == fieldOfView)
        return 0;

    if (fieldOfView->next->tag != L_TERM_FRAGMENT_TAG)
        PRINT_AND_EXIT(BAD_EVAL_EXPR);

    // There is no func calls.
    if (fieldOfView->next->fragment->length == 0)
        return 0;

    // First vterm must be closure!
    if (_memMngr.vterms[fieldOfView->next->fragment->offset].tag != V_CLOSURE_TAG)
    {
        printUStr(stdout, _memMngr.vterms[fieldOfView->next->fragment->offset].str);
        PRINT_AND_EXIT(BAD_EVAL_EXPR);
    }

    struct vclosure_t* closure = _memMngr.vterms[fieldOfView->next->fragment->offset].closure;

    RefalFunc newFuncPointer = closure->funcPtr;
    callTerm->funcCall->env->params = closure->params;
    callTerm->funcCall->env->paramsCount = closure->paramsCount;
    callTerm->funcCall->rollback = closure->rollback;
    callTerm->funcCall->ident = closure->ident;


    if (fieldOfView->next->fragment->length == 1)
    {
        // Remove fragment with closure => lost closure => GC will clean it.
        fieldOfView->next = fieldOfView->next->next;
        fieldOfView->next->prev = fieldOfView;
    }
    else
    {
        fieldOfView->next->fragment->length--;
        fieldOfView->next->fragment->offset++;
    }

	return newFuncPointer;
}

static void printUStr(FILE* file, struct vstring_t* str)
{
    if (!str)
        return;

    uint64_t i = 0;

    for (i = 0; i < str->length; ++i)
        printUTF32(file, str->head[i]);

    fprintf(file, " ");
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

void clearCurrFuncEnvData()
{
    CURR_FUNC_CALL->env->stretchVarsNumber[0] = 0;

    // Значение в CURR_FUNC_CALL->env->stretchVarsNumber[1] не трогем
    // Так как это единственный указатель на собранное поле зрения, которое было
    // передано функции при первом вызове.
    uint32_t i = 0;
    for (i = 1; i < CURR_FUNC_CALL->env->fovsCount; ++i)
    {
        CURR_FUNC_CALL->env->stretchVarsNumber[i] = 0;
        CURR_FUNC_CALL->env->assembled[i] = 0;
    }

    for (i = 1; i < CURR_FUNC_CALL->env->bracketsCount; ++i)
    {
        CURR_FUNC_CALL->env->bracketsOffset[i] = 0;
    }

    for (i = 0; i < CURR_FUNC_CALL->env->localsCount; ++i)
    {
        memset(CURR_FUNC_CALL->env->locals + i, 0, FRAGMENT_STRUCT_SIZE(1));
    }
}

uint64_t chGetAssembliedChain(struct lterm_t* chain, allocate_result *res)
{    
    uint64_t assembledVtermOffset = 0;
    uint64_t length = 0;
    uint64_t offset = 0;

    if (chain != 0)
    {
        if (chain->tag != L_TERM_CHAIN_TAG)
            PRINT_AND_EXIT(ASSEMBLY_NOT_CHAIN);

        CHECK_ALLOCATION_RETURN(assembledVtermOffset, chAllocateBracketVterm(res), *res);

        CHECK_ALLOCATION_RETURN(offset, chAssemblyChain(chain, &length, res), *res);

        VTERM_BRACKETS(assembledVtermOffset)->offset = offset;
        VTERM_BRACKETS(assembledVtermOffset)->length = length;
    }

    return assembledVtermOffset;
}

static uint64_t assemblyTopVTerms(struct lterm_t* chain, uint64_t* length, allocate_result* res)
{
    uint64_t resOffset = _memMngr.vtermsOffset;
    struct lterm_t* currTerm = chain->next;
    *length= 0;

    while (currTerm != chain)
    {
        switch (currTerm->tag)
        {
            case L_TERM_FRAGMENT_TAG :
            {
                uint64_t offset = 0;
                CHECK_ALLOCATION_RETURN(offset, allocateVTerms(currTerm->fragment, res), *res);
                *length += currTerm->fragment->length;
                break;
            }
            case L_TERM_CHAIN_KEEPER_TAG:
            {
                GC_VTERM_HEAP_CHECK_RETURN(1, *res);
                GC_DATA_HEAP_CHECK_RETURN(FRAGMENT_STRUCT_SIZE(1), *res);

                allocateBracketsVTerm();
                ++*length;
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

    return resOffset;
}

static uint64_t chAssemblyChain(struct lterm_t* chain, uint64_t* length, allocate_result* res)
{
    *res = GC_OK;
    uint64_t resOffset = _memMngr.vtermsOffset;
    struct lterm_t* currTerm = chain->next;
    uint64_t topVTermsOffset = 0;

    // Цепочка состоит из единственного фрагментного l-терма. Возвращаем
    // смещение этого фрагментного l-терма.
    if (chain->next == chain->prev && chain->next->tag == L_TERM_FRAGMENT_TAG)
    {
        *length = chain->next->fragment->length;
        return chain->next->fragment->offset;
    }

    CHECK_ALLOCATION_RETURN(topVTermsOffset, assemblyTopVTerms(chain, length, res), *res);

    while (currTerm != chain)
    {
        switch (currTerm->tag)
        {
            case L_TERM_FRAGMENT_TAG :
            {                
                topVTermsOffset += currTerm->fragment->length;
                break;
            }
            case L_TERM_CHAIN_KEEPER_TAG:
            {
                GC_VTERM_HEAP_CHECK_RETURN(1, *res);
                GC_DATA_HEAP_CHECK_RETURN(FRAGMENT_STRUCT_SIZE(1), *res);

                uint64_t offset = 0;
                uint64_t tmpLength = 0;

                CHECK_ALLOCATION_RETURN(offset, chAssemblyChain(currTerm->chain, &tmpLength, res), *res);

                setBracketsData(topVTermsOffset, offset, tmpLength);

                ++topVTermsOffset;
                break;
            }
        }

        currTerm = currTerm->next;
    }

    return resOffset;
}

static struct lterm_t* constructStartFunc(const char* funcName, RefalFunc entryFuncPointer)
{
    allocate_result     res;
    struct vstring_t*   ident;
    struct lterm_t*     fragTerm;
    struct lterm_t*     gofuncCallTerm;

    CHECK_ALLOCATION_EXIT(gofuncCallTerm, chAllocateFuncCallLTerm(&res), res);
    CHECK_ALLOCATION_EXIT(gofuncCallTerm->funcCall->fieldOfView, chAllocateSimpleChainLTerm(&res), res);

    CHECK_ALLOCATION_EXIT(fragTerm, chAllocateFragmentLTerm(1, &res), res);
    CHECK_ALLOCATION_EXIT(fragTerm->fragment->offset, chAllocateClosureVTerm(&res), res);

    fragTerm->fragment->length = 1;

    CHECK_ALLOCATION_EXIT(ident, chAllocateVStringFromASCIIName(funcName, &res), res);

    CHECK_ALLOCATION_EXIT(_memMngr.vterms[fragTerm->fragment->offset].closure,
            chAllocateClosureStruct(entryFuncPointer, 0, ident, 0, &res),
            res);

    ADD_TO_CHAIN(gofuncCallTerm->funcCall->fieldOfView, fragTerm);

    return gofuncCallTerm;
}

static struct lterm_t* constructStartFieldOfView(const char* funcName, RefalFunc entryFuncPointer)
{
    allocate_result     res;
    struct lterm_t*     fieldOfView;

    CHECK_ALLOCATION_EXIT(fieldOfView, chAllocateSimpleChainLTerm(&res), res);

    struct lterm_t* goFuncCallTerm = constructStartFunc(funcName, entryFuncPointer);

    ADD_TO_CHAIN(fieldOfView, goFuncCallTerm);

    return fieldOfView;
}

struct lterm_t* chCopySimpleExpr(struct lterm_t* chain, allocate_result* res)
{
    if (!chain)
        PRINT_AND_EXIT(GC_NULL_CHAIN_SIMPLE_CHAIN_COPY);

    if (chain->tag != L_TERM_CHAIN_TAG)
        PRINT_AND_EXIT(GC_BAD_CHAIN_SIMPLE_CHAIN_COPY);


    GC_DATA_HEAP_CHECK_RETURN(CHAIN_LTERM_SIZE, *res);

    struct lterm_t* newChain = allocateSimpleChain();
    struct lterm_t* currTerm = chain->next;

    while (currTerm != chain)
    {
        if (currTerm->tag != L_TERM_FRAGMENT_TAG)
            PRINT_AND_EXIT(OBJ_EXPR_CONTAINS_ONLY_FRAGS);

        GC_DATA_HEAP_CHECK_RETURN(FRAGMENT_LTERM_SIZE(1), *res);

        struct lterm_t* newTerm = allocateFragmentLTerm(1);
        memcpy(newTerm->fragment, currTerm->fragment, sizeof(struct fragment_t));

        ADD_TO_CHAIN(newChain, newTerm);

        currTerm = currTerm->next;
    }

    return newChain;
}
