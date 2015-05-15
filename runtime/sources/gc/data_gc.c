#include <stdio.h>
#include <time.h>
#include <string.h>
#include <inttypes.h>

#include <memory_manager.h>
#include <defines/gc_macros.h>
#include <defines/errors_str.h>
#include <allocators/data_alloc.h>
#include <defines/data_struct_sizes.h>

static struct lterm_t* copyFuncCallLTerm(struct lterm_t* term);
static struct env_t* copyEnv(struct env_t* from, struct env_t* to);
static struct lterm_t* copyChainVTerm(struct lterm_t* term);
static struct lterm_t* copyFragmentLTerm(struct lterm_t* oldTerm);

#define SET_MOVED(oldTerm, newTerm) \
do{                                 \
    oldTerm->prev = newTerm;        \
    oldTerm->tag = GC_MOVED;        \
}while(0)

struct lterm_t* copySimpleChain(struct lterm_t* chain)
{
    if (!chain)
        PRINT_AND_EXIT(GC_NULL_CHAIN_SIMPLE_CHAIN_COPY);

    if (chain->tag != L_TERM_CHAIN_TAG)
        PRINT_AND_EXIT(GC_BAD_CHAIN_SIMPLE_CHAIN_COPY);    

    GC_DATA_HEAP_CHECK_EXIT(CHAIN_LTERM_SIZE);

    struct lterm_t* newChain = allocateSimpleChain();
    struct lterm_t* newTerm = 0;
    struct lterm_t* currTerm = chain->next;

    while (currTerm != chain)
    {
        newTerm = 0;

        switch (currTerm->tag)
        {
            case L_TERM_FRAGMENT_TAG:
                newTerm = copyFragmentLTerm(currTerm);
                break;

            case L_TERM_CHAIN_KEEPER_TAG:                
                newTerm = copyChainVTerm(currTerm);
                break;

            case L_TERM_FUNC_CALL:
                newTerm = copyFuncCallLTerm(currTerm);
                break;

            case GC_MOVED:                
                newTerm = currTerm->prev;
                break;
        }

        if (newTerm)
        {            
            SET_MOVED(currTerm, newTerm);
            ADD_TO_CHAIN(newChain, newTerm);        
        }
        else                 
            PRINT_AND_EXIT(GC_CANT_COPY_TERM);

        currTerm = currTerm->next;
    }

    SET_MOVED(chain, newChain);

    return newChain;
}

static struct lterm_t* copyChainVTerm(struct lterm_t* oldChainTerm)
{
    GC_DATA_HEAP_CHECK_EXIT(CHAIN_LTERM_SIZE);

    struct lterm_t* newChainTerm = allocateSimpleChain();
    newChainTerm->tag = L_TERM_CHAIN_KEEPER_TAG;

    newChainTerm->chain = copySimpleChain(oldChainTerm->chain);

    SET_MOVED(oldChainTerm, newChainTerm);

    return newChainTerm;
}

static struct lterm_t* copyFuncCallLTerm(struct lterm_t* oldTerm)
{    
    GC_DATA_HEAP_CHECK_EXIT(FUNC_CALL_LTERM_SIZE);

    struct lterm_t* newTerm = allocateFuncCallLTerm();
    struct func_call_t* to = newTerm->funcCall;
    struct func_call_t* from = oldTerm->funcCall;

    to->entryPoint      = from->entryPoint;
    to->failEntryPoint  = from->failEntryPoint;
    to->funcPtr         = from->funcPtr;
    to->ident           = from->ident;
    to->rollback        = from->rollback;

    if (from->fieldOfView)
        to->fieldOfView = copySimpleChain(from->fieldOfView);    

    if (from->funcPtr)
        to->env = copyEnv(from->env, to->env);    

    if (from->parentCall)
    {
        if (from->parentCall->tag != GC_MOVED)
            PRINT_AND_EXIT(GC_PARENT_CALL_NOT_MOVED);

        to->parentCall = from->parentCall->prev;
    }

    to->next = from->next;

    newTerm->tag = L_TERM_FUNC_CALL;

    SET_MOVED(oldTerm, newTerm);

    if (from->subCall)
        to->subCall = copySimpleChain(from->subCall);

    return newTerm;
}

static struct env_t* copyEnv(struct env_t* from, struct env_t* to)
{    
    GC_DATA_HEAP_CHECK_EXIT(ENV_SIZE(from->localsCount, from->fovsCount, from->bracketsCount) + // ENV
                            FRAGMENT_STRUCT_SIZE(from->paramsCount));                           // params

    uint32_t i = 0;
    initEnvData(to, from->localsCount, from->fovsCount, from->bracketsCount);

    if (from->params)
    {
        to->paramsCount = from->paramsCount;
        to->params = allocateFragment(from->paramsCount);
        memcpy(to->params, from->params, FRAGMENT_STRUCT_SIZE(from->paramsCount));
    }

    if (from->locals)
        memcpy(to->locals, from->locals, FRAGMENT_STRUCT_SIZE(from->localsCount));

    memcpy(to->assembled, from->assembled, from->fovsCount * sizeof(uint64_t));
    memcpy(to->stretchVarsNumber, from->stretchVarsNumber, from->fovsCount * sizeof(int));

    if (from->workFieldOfView)
        to->workFieldOfView = copySimpleChain(from->workFieldOfView);    

    return to;
}

static struct lterm_t* copyFragmentLTerm(struct lterm_t* oldTerm)
{
    GC_DATA_HEAP_CHECK_EXIT(FRAGMENT_LTERM_SIZE(1));

    struct lterm_t* newTerm = allocateFragmentLTerm(1);
    memcpy(newTerm->fragment, oldTerm->fragment, sizeof(struct fragment_t));

    SET_MOVED(oldTerm, newTerm);

    return newTerm;
}
