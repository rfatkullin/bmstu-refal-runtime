#include <stdio.h>
#include <time.h>
#include <string.h>
#include <inttypes.h>

#include <memory_manager.h>
#include <allocators/data_alloc.h>

static struct lterm_t* copyFuncCallLTerm(struct lterm_t* term);
static struct env_t* copyEnv(struct env_t* from, struct env_t* to);
static struct lterm_t* copyChainVTerm(struct lterm_t* term);

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
            PRINT_AND_EXIT(CANT_COPY_TERM);

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
    to->rollback        = from->rollback;

    if (from->fieldOfView)
        to->fieldOfView = copySimpleChain(from->fieldOfView);    

    if (from->funcPtr)
        to->env = copyEnv(from->env, to->env);

    if (from->subCall)
        to->subCall = copySimpleChain(from->subCall);

    if (from->parentCall)
    {
        if (from->parentCall->tag != GC_MOVED)
            PRINT_AND_EXIT(GC_PARENT_CALL_NOT_MOVED);

        to->parentCall = from->parentCall->chain;
    }

    to->next = from->next;

    newTerm->tag = L_TERM_FUNC_CALL;

    return newTerm;
}

static struct env_t* copyEnv(struct env_t* from, struct env_t* to)
{
    GC_DATA_HEAP_CHECK_EXIT(ENV_SIZE(from->localsCount, from->fovsCount) +  // ENV
                            FRAGMENT_LTERM_SIZE(from->paramsCount));        // params

    uint32_t i = 0;
    allocateEnvData(to, from->localsCount, from->fovsCount);

    if (from->params)
    {
        to->params = allocateFragmentLTerm(from->paramsCount);

        for (i = 0; i < from->paramsCount; ++i)
            memcpy(to->params[i].fragment, from->params[i].fragment, sizeof(struct fragment_t));
    }

    for (i = 0; i < from->localsCount; ++i)
        memcpy(to->locals[i].fragment, from->locals[i].fragment, sizeof(struct fragment_t));

    for (i = 0; i < from->fovsCount; ++i)
    {
        if (from->fovs[i])
            to->fovs[i] = copySimpleChain(from->fovs[i]);

        if (from->assembledFOVs[i])
            to->assembledFOVs[i] = copyFragmentLTerm(from->assembledFOVs[i]);
    }

    memcpy(to->stretchVarsNumber, from->stretchVarsNumber, from->fovsCount * sizeof(int));

    return to;
}

struct lterm_t* copyFragmentLTerm(struct lterm_t* oldTerm)
{
    GC_DATA_HEAP_CHECK_EXIT(FRAGMENT_LTERM_SIZE(1));

    struct lterm_t* newTerm = allocateFragmentLTerm(1);
    memcpy(newTerm->fragment, oldTerm->fragment, sizeof(struct fragment_t));

    SET_MOVED(oldTerm, newTerm);

    return newTerm;
}

