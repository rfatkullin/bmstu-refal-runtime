#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "allocators.h"

/// Проверки на переполнение.
static void checkVTermsMemoryOverflow(uint64_t);
static void checkLTermsMemoryOverflow(uint64_t);
static void checkDataMemoryOverflow(uint64_t);

static void failWithMemoryOverflow()
{
    printf("Failed with memory overflow!");
    exit(1);
}

//TO FIX: сделать проверку переполнения памяти.
void allocateVTerms(struct fragment_t* frag)
{
    uint64_t i = 0;
    for (i = 0; i < frag->length; ++i)
    {
        memMngr.vterms[memMngr.vtermsOffset].tag = memMngr.vterms[frag->offset + i].tag;

        switch (memMngr.vterms[frag->offset + i].tag)
        {
            case V_CHAR_TAG:
                memMngr.vterms[memMngr.vtermsOffset++].ch = memMngr.vterms[frag->offset + i].ch;
                break;

            case V_IDENT_TAG :
                memMngr.vterms[memMngr.vtermsOffset++].str = memMngr.vterms[frag->offset + i].str;
                break;

            case V_INT_NUM_TAG:
                memMngr.vterms[memMngr.vtermsOffset++].intNum = memMngr.vterms[frag->offset + i].intNum;
                break;

            case V_DOUBLE_NUM_TAG:
                memMngr.vterms[memMngr.vtermsOffset++].doubleNum = memMngr.vterms[frag->offset + i].doubleNum;
                break;

            case V_CLOSURE_TAG:
                allocateClosure(memMngr.vterms[frag->offset + i].closure->funcPtr,
                                memMngr.vterms[frag->offset + i].closure->paramsCount,
                                memMngr.vterms[frag->offset + i].closure->ident);
                break;

            case V_BRACKET_OPEN_TAG:
            case V_BRACKET_CLOSE_TAG:
                memMngr.vterms[memMngr.vtermsOffset++].inBracketLength = memMngr.vterms[frag->offset + i].inBracketLength;
                break;
        }        
    }
}

uint64_t allocateClosure(RefalFunc funcPtr, uint32_t paramsCount, struct v_string* ident)
{
    checkLTermsMemoryOverflow(1);
    checkDataMemoryOverflow(sizeof(struct v_closure) + paramsCount * sizeof(struct lterm_t));

    struct v_term* term = memMngr.vterms + memMngr.vtermsOffset;
    term->tag = V_CLOSURE_TAG;

    term->closure = (struct v_closure*)(memMngr.dataHeap + memMngr.dataOffset);
    memMngr.dataOffset += sizeof(struct v_closure);
    term->closure->params = (struct lterm_t*)(memMngr.dataHeap + memMngr.dataOffset);
    memMngr.dataOffset += paramsCount * sizeof(struct lterm_t);
    term->closure->funcPtr = funcPtr;
    term->closure->ident = ident;
    term->closure->paramsCount = paramsCount;

    return memMngr.vtermsOffset++;
}

uint64_t allocateSymbol(uint32_t ch)
{
    checkVTermsMemoryOverflow(1);

    struct v_term* term = memMngr.vterms + memMngr.vtermsOffset;
    term->tag = V_CHAR_TAG;
    term->ch = ch;

    return memMngr.vtermsOffset++;
}

uint64_t allocateIntNum(uint64_t count)
{
    uint64_t i = 0;
    uint64_t offset = 0;

    checkVTermsMemoryOverflow(count);

    for (i = 0; i < count; ++i)
        memMngr.vterms[memMngr.vtermsOffset + i].tag = V_INT_NUM_TAG;

    offset = memMngr.vtermsOffset;
    memMngr.vtermsOffset += count;

    return offset;
}

uint64_t allocateDoubleNum()
{
    checkVTermsMemoryOverflow(1);
    memMngr.vterms[memMngr.vtermsOffset].tag = V_DOUBLE_NUM_TAG;
    return memMngr.vtermsOffset++;
}

//TO FIX: сделать проверку переполнения памяти.
uint64_t allocateOpenBracketVTerm(uint64_t length)
{
    memMngr.vterms[memMngr.vtermsOffset].tag = V_BRACKET_OPEN_TAG;
    memMngr.vterms[memMngr.vtermsOffset].inBracketLength = length;

    return memMngr.vtermsOffset++;
}

uint64_t allocateCloseBracketVTerm(uint64_t length)
{
    memMngr.vterms[memMngr.vtermsOffset].tag = V_BRACKET_CLOSE_TAG;
    memMngr.vterms[memMngr.vtermsOffset].inBracketLength = length;

    return memMngr.vtermsOffset++;
}

void changeBracketLength(uint64_t offset, uint64_t newLength)
{
    memMngr.vterms[offset].inBracketLength = newLength;
}

//TO FIX: Проверка на переполнение памяти.
struct v_string* allocateVStringLiteral(uint32_t* runes, uint64_t length)
{
    struct v_string* pointer = (struct v_string*)malloc(sizeof(struct v_string));

    pointer->head = (uint32_t*) malloc(length * sizeof(uint32_t));
    pointer->length = length;

    uint64_t i =0;
    for (i = 0; i < length; ++i)
        pointer->head[i] = runes[i];

    return pointer;
}


//TO FIX: Проверка на переполнение памяти.
struct v_int* allocateIntNumberLiteral(uint8_t* bytes, uint8_t sign, uint64_t length)
{
    struct v_int* pointer = (struct v_int*)malloc(sizeof(struct v_int));

    pointer->bytes = (uint8_t*) malloc(length * sizeof(uint8_t));
    pointer->length = length;
    pointer->sign = sign;

    u_int64_t i = 0;
    for (i = 0; i < length; ++i)
        pointer->bytes[i] = bytes[i];

    return pointer;
}

//TO FIX: Проверка на переполнение.
struct v_int* allocateIntNumber(uint64_t length)
{
    struct v_int* pointer = (struct v_int*)(memMngr.dataHeap + memMngr.dataOffset);
    memMngr.dataOffset += sizeof(struct v_int);

    pointer->bytes = (uint8_t*)(memMngr.dataHeap + memMngr.dataOffset);
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
    checkLTermsMemoryOverflow(count);
    checkDataMemoryOverflow(count * sizeof(struct fragment_t));

    struct lterm_t* headLterm = memMngr.lterms + memMngr.ltermsOffset;
    struct lterm_t* lterm = headLterm;

    uint32_t i = 0;
    for (i = 0; i < count; ++i)
    {
        lterm->tag = L_TERM_FRAGMENT_TAG;
        lterm->fragment = (struct fragment_t*)(memMngr.dataHeap + memMngr.dataOffset);
        memMngr.dataOffset += sizeof(struct fragment_t);
        lterm++;
    }

    memMngr.ltermsOffset += count;

    return headLterm;
}

struct lterm_t* allocateFuncCallLTerm()
{
    checkLTermsMemoryOverflow(1);
    checkDataMemoryOverflow(sizeof(struct func_call_t) + sizeof(struct env_t));

    struct lterm_t* lterm = memMngr.lterms + memMngr.ltermsOffset;
    lterm->tag = L_TERM_FUNC_CALL;

    lterm->funcCall = (struct func_call_t*)(memMngr.dataHeap + memMngr.dataOffset);
    memMngr.dataOffset += sizeof(struct func_call_t);
    lterm->funcCall->env = (struct env_t*)(memMngr.dataHeap + memMngr.dataOffset);
    memMngr.dataOffset += sizeof(struct env_t);

    memMngr.ltermsOffset++;

    return lterm;
}

struct lterm_t* allocateChainLTerm(uint64_t count)
{
    checkLTermsMemoryOverflow(2 * count);

    struct lterm_t* headLTerm = memMngr.lterms + memMngr.ltermsOffset;
    struct lterm_t* lterm = headLTerm;
    uint64_t i = 0;

    for (i = 0; i < count; ++i)
    {
        lterm->tag = L_TERM_CHAIN_TAG;
        lterm->chain = lterm + count ;
        lterm->chain->next = lterm->chain;
        lterm->chain->prev = lterm->chain;

        lterm++;
    }

    memMngr.ltermsOffset += 2 * count;

    return headLTerm;
}

//TO FIX: Проверка на переполнение для env->locals
struct lterm_t* allocateEnvData(struct env_t* env, uint32_t localsCount, uint32_t patternsCount)
{
    env->locals = allocateFragmentLTerm(localsCount);
    env->_FOVs = (struct lterm_t**)(memMngr.dataHeap + memMngr.dataOffset);
    memMngr.dataOffset += patternsCount * sizeof(struct lterm_t*);
    env->assembledFOVs = (struct lterm_t**)(memMngr.dataHeap + memMngr.dataOffset);
    memMngr.dataOffset += patternsCount * sizeof(struct lterm_t*);
    env->stretchVarsNumber = (int*)(memMngr.dataHeap + memMngr.dataOffset);
    memMngr.dataOffset += patternsCount * sizeof(int);

    memset(env->_FOVs, 0, patternsCount * sizeof(struct lterm_t*));
    memset(env->assembledFOVs, 0, patternsCount * sizeof(struct lterm_t*));
    memset(env->stretchVarsNumber, 0, patternsCount * sizeof(int));
}

void checkVTermsMemoryOverflow(uint64_t needVTermsCount)
{
    if (memMngr.vtermsOffset + needVTermsCount > memMngr.vtermsMaxOffset)
    {
        //TO FIX: Передать корень FOV.
        collectGarbage(0);
    }

    if (memMngr.vtermsOffset + needVTermsCount > memMngr.vtermsMaxOffset)
        failWithMemoryOverflow();
}

void checkLTermsMemoryOverflow(uint64_t needLTermsCount)
{
    if (memMngr.ltermsOffset + needLTermsCount > memMngr.ltermsMaxOffset)
    {
        //TO FIX: Передать корень FOV.
        collectGarbage(0);
    }

    if (memMngr.ltermsOffset + needLTermsCount > memMngr.ltermsMaxOffset)
        failWithMemoryOverflow();
}

void checkDataMemoryOverflow(uint64_t needDataCount)
{
    if (memMngr.dataOffset + needDataCount > memMngr.dataMaxOffset)
    {
        //TO FIX: Передать корень FOV.
        collectGarbage(0);
    }

    if (memMngr.dataOffset + needDataCount > memMngr.dataMaxOffset)
        failWithMemoryOverflow();
}


