#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <memory_manager.h>
#include <allocators/data_alloc.h>

struct lterm_t* chAllocateFuncCallLTerm(allocate_result* res)
{
    uint64_t needDataSize = sizeof(struct func_call_t) + sizeof(struct env_t) + sizeof(struct lterm_t);

    if (memMngr.dataOffset + needDataSize > memMngr.dataMaxOffset)
    {
        *res = NEED_CLEAN;
        return 0;
    }

    *res = OK;

    return allocateFuncCallLTerm();
}

struct lterm_t* chAllocateChainLTerm(uint32_t count, allocate_result* res)
{
    uint64_t needDataSize = 2 * count * sizeof(struct lterm_t);
    if (memMngr.dataOffset + needDataSize > memMngr.dataMaxOffset)
    {
        *res = NEED_CLEAN;
        return 0;
    }

    *res = OK;

    return allocateChainLTerm(count);
}

struct lterm_t* chAllocateFragmentLTerm(uint32_t count, allocate_result *res)
{
    uint64_t needDataSize = count * (sizeof(struct fragment_t) + sizeof(struct lterm_t));
    if (memMngr.dataOffset + needDataSize > memMngr.dataMaxOffset)
    {
        *res = NEED_CLEAN;
        return 0;
    }

    *res = OK;

    return allocateFragmentLTerm(count);
}

struct v_closure* chAllocateClosureStruct(RefalFunc funcPtr, uint32_t paramsCount, struct v_string* ident, int rollback, allocate_result *res)
{
    if (memMngr.dataOffset + CLOSURE_SIZE(paramsCount) > memMngr.dataMaxOffset)
    {
        *res = NEED_CLEAN;
        return 0;
    }

    *res = OK;

    return allocateClosureStruct(funcPtr, paramsCount, ident, rollback);
}

struct v_closure* gcAllocateClosureStruct(RefalFunc funcPtr, uint32_t paramsCount, struct v_string* ident, int rollback)
{
    checkAndCleanData(CLOSURE_SIZE(paramsCount));

    return allocateClosureStruct(funcPtr, paramsCount, ident, rollback);
}

struct v_int* gcAllocateIntStruct(uint64_t length)
{
    checkAndCleanData(VINT_STRUCT_SIZE(length));

    return allocateIntStruct(length);
}

struct lterm_t* gcAllocateFragmentLTerm(uint32_t count)
{
    checkAndCleanData(count * (sizeof(struct fragment_t) + sizeof(struct lterm_t)));

    return allocateFragmentLTerm(count);
}

struct env_t* gcAllocateEnvData(struct env_t* env, uint32_t localsCount, uint32_t patternsCount)
{
    checkAndCleanData(patternsCount * sizeof(struct lterm_t*) +
                            patternsCount * sizeof(struct lterm_t*) +
                            patternsCount * sizeof(int));

    return allocateEnvData(env, localsCount, patternsCount);
}

struct env_t* allocateEnvData(struct env_t* env, uint32_t localsCount, uint32_t patternsCount)
{
    env->locals = allocateFragmentLTerm(localsCount);

    env->fovs = (struct lterm_t**)(memMngr.data + memMngr.dataOffset);
    memMngr.dataOffset += patternsCount * sizeof(struct lterm_t*);

    env->assembledFOVs = (struct lterm_t**)(memMngr.data + memMngr.dataOffset);
    memMngr.dataOffset += patternsCount * sizeof(struct lterm_t*);

    env->stretchVarsNumber = (int*)(memMngr.data + memMngr.dataOffset);
    memMngr.dataOffset += patternsCount * sizeof(int);

    env->localsCount = localsCount;
    env->fovsCount = patternsCount;

    memset(env->fovs, 0, patternsCount * sizeof(struct lterm_t*));
    memset(env->assembledFOVs, 0, patternsCount * sizeof(struct lterm_t*));
    memset(env->stretchVarsNumber, 0, patternsCount * sizeof(int));

    return env;
}

struct v_closure* allocateClosureStruct(RefalFunc funcPtr, uint32_t paramsCount, struct v_string* ident, int rollback)
{
    struct v_closure* closure = (struct v_closure*)(memMngr.data + memMngr.dataOffset);
    memMngr.dataOffset += sizeof(struct v_closure);

    closure->params = (struct lterm_t*)(memMngr.data + memMngr.dataOffset);
    memMngr.dataOffset += paramsCount * sizeof(struct lterm_t);

    closure->funcPtr = funcPtr;
    closure->ident = ident;
    closure->paramsCount = paramsCount;
    closure->rollback = rollback;

    return closure;
}

struct v_int* allocateIntStruct(uint64_t length)
{
    struct v_int* pointer = (struct v_int*)(memMngr.data + memMngr.dataOffset);
    memMngr.dataOffset += sizeof(struct v_int);

    pointer->bytes = (uint8_t*)(memMngr.data + memMngr.dataOffset);
    pointer->length = length;

    memMngr.dataOffset += length;

    return pointer;
}

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

struct lterm_t* allocateFragmentLTerm(uint32_t count)
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
