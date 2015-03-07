#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "data_allocators.h"
#include "memory_manager.h"

static struct lterm_t* allocateFragmentLTerm(uint32_t count);

struct lterm_t* allocateBuiltinsResult(uint64_t offset, uint64_t length)
{
    struct lterm_t* chain = 0;

    struct lterm_t* lterm = allocateFragmentLTerm(1);
    lterm->fragment->offset = offset;
    lterm->fragment->length = length;

    chain = allocateChainLTerm(1);
    chain->next = lterm;
    chain->prev = lterm;
    lterm->next = chain;
    lterm->prev = chain;

    return chain;
}

struct lterm_t* gcAllocateFragmentLTerm(uint32_t count)
{
    checkAndCleanData(count * (sizeof(struct fragment_t) + sizeof(struct lterm_t)));

    return allocateFragmentLTerm(count);
}

static struct lterm_t* allocateFragmentLTerm(uint32_t count)
{
    struct lterm_t* lterm = (struct lterm_t*)(memMngr.data + memMngr.dataOffset);
    memMngr.dataOffset += count * sizeof(struct lterm_t);

    struct fragment_t* fragment = (struct fragment_t*)(memMngr.data + memMngr.dataOffset);
    memMngr.dataOffset += count * sizeof(struct fragment_t);

    struct lterm_t* head = lterm;

    uint32_t i = 0;
    for (i = 0; i < count; ++i)
    {
        lterm->tag = L_TERM_FRAGMENT_TAG;
        lterm->fragment = fragment;
        lterm++;
        fragment++;
    }    

    return head;
}

struct lterm_t* allocateFuncCallLTerm()
{    
    checkAndCleanData(sizeof(struct func_call_t) + sizeof(struct env_t) + sizeof(struct lterm_t));

    struct lterm_t* lterm = (struct lterm_t*)(memMngr.data + memMngr.dataOffset);
    memMngr.dataOffset +=  sizeof(struct lterm_t);

    lterm->funcCall = (struct func_call_t*)(memMngr.data + memMngr.dataOffset);
    memMngr.dataOffset += sizeof(struct func_call_t);

    memset(lterm->funcCall, 0, sizeof(struct func_call_t));

    lterm->funcCall->env = (struct env_t*)(memMngr.data + memMngr.dataOffset);
    memMngr.dataOffset += sizeof(struct env_t);

    lterm->tag = L_TERM_FUNC_CALL;

    return lterm;
}

struct lterm_t* allocateChainLTerm(uint32_t count)
{
    checkAndCleanData(2 * count * sizeof(struct lterm_t));

    struct lterm_t* lterm = (struct lterm_t*)(memMngr.data + memMngr.dataOffset);
    memMngr.dataOffset += 2 * count * sizeof(struct lterm_t);

    struct lterm_t* chain = lterm + count;
    struct lterm_t* head = lterm;

    uint32_t i = 0;

    for (i = 0; i < count; ++i)
    {
        lterm->tag = L_TERM_CHAIN_TAG;
        lterm->chain = chain;
        lterm->chain->next = lterm->chain;
        lterm->chain->prev = lterm->chain;

        lterm++;
        chain++;
    }

    return head;
}

struct env_t* allocateEnvData(struct env_t* env, uint32_t localsCount, uint32_t patternsCount)
{
    checkAndCleanData(patternsCount * sizeof(struct lterm_t*) +
                            patternsCount * sizeof(struct lterm_t*) +
                            patternsCount * sizeof(int));

    env->locals = allocateFragmentLTerm(localsCount);

    env->_FOVs = (struct lterm_t**)(memMngr.data + memMngr.dataOffset);
    memMngr.dataOffset += patternsCount * sizeof(struct lterm_t*);

    env->assembledFOVs = (struct lterm_t**)(memMngr.data + memMngr.dataOffset);
    memMngr.dataOffset += patternsCount * sizeof(struct lterm_t*);

    env->stretchVarsNumber = (int*)(memMngr.data + memMngr.dataOffset);
    memMngr.dataOffset += patternsCount * sizeof(int);

    memset(env->_FOVs, 0, patternsCount * sizeof(struct lterm_t*));
    memset(env->assembledFOVs, 0, patternsCount * sizeof(struct lterm_t*));
    memset(env->stretchVarsNumber, 0, patternsCount * sizeof(int));

    return env;
}

