#include <stdio.h>
#include <time.h>
#include <string.h>

#include <memory_manager.h>
#include <allocators/data_alloc.h>

static struct lterm_t* copyFuncCallLTerm(struct lterm_t* term);
static struct env_t* copyEnv(int funcCalled, struct env_t* from, struct env_t* to);
static struct lterm_t* copyChainVTerm(struct lterm_t* term);

struct lterm_t* copySimpleChain(struct lterm_t* chain)
{
    //if (!chain)
      //  PRINT_AND_EXIT(GC_NULL_CHAIN_SIMPLE_CHAIN_COPY);

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
                newTerm = currTerm->chain;
                break;
        }

        if (newTerm)        
            ADD_TO_CHAIN(newChain, newTerm);        
        else                 
            PRINT_AND_EXIT(CANT_COPY_TERM);

        currTerm = currTerm->next;
    }

    return newChain;
}

static struct lterm_t* copyChainVTerm(struct lterm_t* term)
{
    GC_DATA_HEAP_CHECK_EXIT(CHAIN_LTERM_SIZE);

    struct lterm_t* chainTerm = allocateSimpleChain();
    chainTerm->tag = L_TERM_CHAIN_KEEPER_TAG;

    chainTerm->chain = copySimpleChain(term->chain);

    return chainTerm;
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

    to->env = copyEnv(!from->funcPtr, from->env, to->env);

    if (from->subCall)
        to->subCall = copySimpleChain(from->subCall);

    if (from->parentCall)
    {
        if (from->parentCall->tag != GC_MOVED)
            PRINT_AND_EXIT(GC_PARENT_CALL_NOT_MOVED);

        to->parentCall = from->parentCall->chain;
    }

    if (from->next)
    {
        if (from->next->tag != GC_MOVED)
            to->next = copyFuncCallLTerm(from->next);
        else
            to->next = from->next->chain;
    }
    else
    {
        to->next = 0;
    }

    newTerm->tag = L_TERM_FUNC_CALL;
    oldTerm->tag = GC_MOVED;
    oldTerm->chain = newTerm;

    return newTerm;
}

static struct env_t* copyEnv(int isFuncCalled, struct env_t* from, struct env_t* to)
{
    GC_DATA_HEAP_CHECK_EXIT(ENV_SIZE(from->localsCount, from->paramsCount));

    allocateEnvData(from, from->localsCount, from->fovsCount);

    memcpy(to->params, from->params, from->paramsCount * sizeof(struct lterm_t));

    if (isFuncCalled)
    {
        memcpy(to->locals, from->locals, from->localsCount * sizeof(struct lterm_t));
        memcpy(to->stretchVarsNumber, from->stretchVarsNumber, from->fovsCount * sizeof(int));
    }

    uint32_t i =0;
    for (i = 0; i < from->fovsCount; ++i)
    {
        if (from->fovs[i])
            to->fovs[i] = copySimpleChain(from->fovs[i]);
    }

    for (i = 0; i < from->fovsCount; ++i)
    {
        if (from->assembledFOVs[i])
            to->assembledFOVs[i] = copyFragmentLTerm(from->assembledFOVs[i]);
    }

    return to;
}

struct lterm_t* copyFragmentLTerm(struct lterm_t* term)
{
    GC_DATA_HEAP_CHECK_EXIT(FRAGMENT_LTERM_SIZE(1));

    struct lterm_t* newTerm = allocateFragmentLTerm(1);
    memcpy(newTerm->fragment, term->fragment, sizeof(struct fragment_t));

    return newTerm;
}

