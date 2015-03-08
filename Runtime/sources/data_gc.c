#include <stdio.h>
#include <time.h>
#include <string.h>

#include "memory_manager.h"
#include "data_allocators.h"

static struct lterm_t* copyFuncCallLTerm(struct lterm_t* term);
static struct lterm_t* copyFragmentLTerm(struct lterm_t* term);
static void addTerm(struct lterm_t* chain, struct lterm_t* term);
static struct env_t* copyEnv(struct env_t* from, struct env_t* to);

struct lterm_t* copyChainLTerm(struct lterm_t* chain)
{
    struct lterm_t* newChain = allocateChainLTerm(1);
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

            case L_TERM_CHAIN_TAG:
                // TO FIX: Правильная обработка!
                printf("[Warn]: Chain contains another chain!");
                newTerm = copyChainLTerm(currTerm);
                break;
            case L_TERM_FUNC_CALL:
                newTerm = copyFuncCallLTerm(currTerm);
                break;
        }

        if (newTerm)
        {
            addTerm(newChain, newTerm);
        }
        else
        {
            // TO FIX: Правильная обработка!
            printf("[Error]: null term!");
        }

        currTerm = currTerm->next;
    }

    return newChain;
}


static struct lterm_t* copyFuncCallLTerm(struct lterm_t* oldTerm)
{
    struct lterm_t* newTerm = allocateFuncCallLTerm();
    struct func_call_t* to = newTerm->funcCall;
    struct func_call_t* from = oldTerm->funcCall;

    memcpy(to, from, sizeof(struct func_call_t));

    to->fieldOfView = copyChainLTerm(from->fieldOfView);
    to->env = copyEnv(from->env, to->env);
    to->subCall = copyChainLTerm(from->subCall);

    // TO FIX: Кажется, поля parentCall и next одно и то же.
    if (from->next->tag != GC_MOVED)
    {
        printf("[Warn]: Strange situation");
        to->next = copyFuncCallLTerm(from->next);
    }

    if (from->parentCall->tag != GC_MOVED)
    {
        printf("[Warn]: Strange situation");
        to->parentCall = copyFuncCallLTerm(from->parentCall);
    }

    newTerm->tag = L_TERM_FUNC_CALL;
    oldTerm->tag = GC_MOVED;

    return newTerm;
}

static struct env_t* copyEnv(struct env_t* from, struct env_t* to)
{
    allocateEnvData(from, from->localsCount, from->fovsCount);

    memcpy(to->params, from->params, from->paramsCount * sizeof(struct lterm_t));
    memcpy(to->locals, from->locals, from->localsCount * sizeof(struct lterm_t));
    memcpy(to->stretchVarsNumber, from->stretchVarsNumber, from->fovsCount * sizeof(int));

    uint32_t i =0;
    for (i = 0; i < from->fovsCount; ++i)
        to->fovs[i] = copyChainLTerm(from->fovs[i]);

    for (i = 0; i < from->fovsCount; ++i)
        to->assembledFOVs[i] = copyFragmentLTerm(from->assembledFOVs[i]);

    return to;
}

static struct lterm_t* copyFragmentLTerm(struct lterm_t* term)
{
    struct lterm_t* newTerm = allocateFragmentLTerm(1);

    memcpy(newTerm->fragment, term->fragment, sizeof(struct fragment_t));

    return newTerm;
}

static void addTerm(struct lterm_t* chain, struct lterm_t* term)
{
    chain->prev->next = term;
    term->prev = chain->prev;
    chain->prev = term;
    term->next = chain;
}
