#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <memory_manager.h>
#include <allocators/data_alloc.h>

static struct lterm_t* chCopyChainKeeperLTerm(struct lterm_t* term, allocate_result* res);
static struct lterm_t* chCopyFragmentLTerm(struct lterm_t* term, allocate_result* res);

struct lterm_t* chAllocateFuncCallLTerm(allocate_result* res)
{
    GC_DATA_HEAP_CHECK_RETURN(FUNC_CALL_LTERM_SIZE, *res);

    return allocateFuncCallLTerm();
}

struct lterm_t* chAllocateChainKeeperLTerm(uint32_t count, allocate_result* res)
{
    GC_DATA_HEAP_CHECK_RETURN(CHAIN_KEEPER_LTERM_SIZE(count), *res);

    return allocateChainKeeperLTerm(count);
}

struct lterm_t* chAllocateSimpleChainLTerm(allocate_result* res)
{
    GC_DATA_HEAP_CHECK_RETURN(CHAIN_LTERM_SIZE, *res);

    return allocateSimpleChain();
}

struct lterm_t* chAllocateFragmentLTerm(uint32_t count, allocate_result *res)
{
    GC_DATA_HEAP_CHECK_RETURN(FRAGMENT_LTERM_SIZE(count), *res);

    return allocateFragmentLTerm(count);
}

struct v_closure* chAllocateClosureStruct(RefalFunc funcPtr, uint32_t paramsCount, struct v_string* ident, int rollback, allocate_result *res)
{
    GC_DATA_HEAP_CHECK_RETURN(VCLOSURE_SIZE(paramsCount), *res);

    return allocateClosureStruct(funcPtr, paramsCount, ident, rollback);
}

struct lterm_t* chCopyFieldOfView(struct lterm_t* chain, allocate_result* result)
{
    struct lterm_t* newChain = 0;
    struct lterm_t* newTerm = 0;
    struct lterm_t* currTerm = chain->next;

    CHECK_ALLOCATION_RETURN(newChain, chAllocateSimpleChainLTerm(result), *result);

    while (currTerm != chain)
    {
        newTerm = 0;

        switch (currTerm->tag)
        {
            case L_TERM_FRAGMENT_TAG:
                CHECK_ALLOCATION_RETURN(newTerm, chCopyFragmentLTerm(currTerm, result), *result);
                break;

            case L_TERM_CHAIN_KEEPER_TAG:
                CHECK_ALLOCATION_RETURN(newTerm, chCopyChainKeeperLTerm(currTerm, result), *result);
                break;

            case L_TERM_FUNC_CALL:
                PRINT_AND_EXIT(FOV_CONTAINS_FUNC_CALL);
                break;

            case L_TERM_CHAIN_TAG:
                PRINT_AND_EXIT(FOV_CONTAINS_SIMPLE_CHAIN);
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

struct v_closure* gcAllocateClosureStruct(RefalFunc funcPtr, uint32_t paramsCount, struct v_string* ident, int rollback)
{
    checkAndCleanHeaps(0, VCLOSURE_SIZE(paramsCount));

    return allocateClosureStruct(funcPtr, paramsCount, ident, rollback);
}

struct v_int* gcAllocateIntStruct(uint64_t length)
{
    checkAndCleanHeaps(0, VINT_STRUCT_SIZE(length));

    return allocateIntStruct(length);
}

struct lterm_t* gcAllocateFragmentLTerm(uint32_t count)
{
    checkAndCleanHeaps(0, count * (sizeof(struct fragment_t) + sizeof(struct lterm_t)));

    return allocateFragmentLTerm(count);
}

struct env_t* gcAllocateEnvData(struct env_t* env, uint32_t localsCount, uint32_t patternsCount)
{
    checkAndCleanHeaps(0, ENV_SIZE(localsCount, patternsCount));

    return allocateEnvData(env, localsCount, patternsCount);
}

// Params sets in mainLoop.
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

    closure->params = allocateFragmentLTerm(paramsCount);

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
    struct lterm_t* fragmentLTerm = allocateFragmentLTerm(1);
    fragmentLTerm->fragment->offset = offset;
    fragmentLTerm->fragment->length = length;

    struct lterm_t* chain = allocateSimpleChain();

    ADD_TO_CHAIN(chain, fragmentLTerm);

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

struct lterm_t* allocateChainKeeperLTerm(uint32_t count)
{
    struct lterm_t* chainKeeperTerm = (struct lterm_t*)(memMngr.data + memMngr.dataOffset);
    memMngr.dataOffset += count * sizeof(struct lterm_t);

    struct lterm_t* head = chainKeeperTerm;

    uint32_t i = 0;

    for (i = 0; i < count; ++i)
    {
        chainKeeperTerm->tag = L_TERM_CHAIN_KEEPER_TAG;
        chainKeeperTerm->chain = allocateSimpleChain();
        chainKeeperTerm++;
    }

    return head;
}

struct lterm_t* allocateSimpleChain()
{
    struct lterm_t* chain = (struct lterm_t*)(memMngr.data + memMngr.dataOffset);
    memMngr.dataOffset += sizeof(struct lterm_t);

    chain->tag = L_TERM_CHAIN_TAG;
    chain->prev = chain;
    chain->next = chain;

    return chain;
}

static struct lterm_t* chCopyChainKeeperLTerm(struct lterm_t* term, allocate_result* res)
{
    GC_DATA_HEAP_CHECK_RETURN(CHAIN_KEEPER_LTERM_SIZE(1), *res);

    struct lterm_t* chainKeeperTerm = allocateChainKeeperLTerm(1);

    CHECK_ALLOCATION_RETURN(chainKeeperTerm->chain, chCopyFieldOfView(term->chain, res), *res);

    return chainKeeperTerm;
}

static struct lterm_t* chCopyFragmentLTerm(struct lterm_t* term, allocate_result* res)
{
    GC_DATA_HEAP_CHECK_RETURN(FRAGMENT_LTERM_SIZE(1), *res);

    struct lterm_t* newTerm = allocateFragmentLTerm(1);
    memcpy(newTerm->fragment, term->fragment, sizeof(struct fragment_t));

    return newTerm;
}
