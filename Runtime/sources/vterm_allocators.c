#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "allocators.h"

/// Функции выделения vterm'ов без проверок.
static uint64_t allocateDoubleNum();
static uint64_t allocateIntNumVTerm();
static uint64_t allocateSymbolVTerm(uint32_t ch);
static struct v_int* allocateIntStruct(uint64_t length);
static uint64_t allocateClosure(RefalFunc funcPtr, uint32_t paramsCount, struct v_string* ident, int rollback);

//TO FIX: сделать проверку переполнения памяти.
int allocateVTerms(struct fragment_t* frag)
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
                                memMngr.vterms[frag->offset + i].closure->ident,
                                memMngr.vterms[frag->offset + i].closure->rollback);
                break;

            case V_BRACKET_OPEN_TAG:
            case V_BRACKET_CLOSE_TAG:
                memMngr.vterms[memMngr.vtermsOffset++].inBracketLength = memMngr.vterms[frag->offset + i].inBracketLength;
                break;
        }        
    }

    return 1;
}

uint64_t gcAllocateClosure(RefalFunc ptr, uint32_t paramsCount, struct v_string* ident, int rollback)
{
    checkAndCleanVTerms(1);
    checkAndCleanData(sizeof(struct v_closure) + paramsCount * sizeof(struct lterm_t));

    return allocateClosure(ptr, paramsCount, ident, rollback);
}

struct v_int* gcAllocateIntStruct(uint64_t length)
{
    checkAndCleanData(sizeof(struct v_int) + length);

    return allocateIntStruct(length);
}

uint64_t gcAllocateSymbolVTerm(uint32_t ch)
{
    checkAndCleanVTerms(1);

    return allocateSymbolVTerm(ch);
}

uint64_t gcAllocateIntNumVTerm(struct v_int* value)
{
    checkAndCleanVTerms(1);

    return allocateIntNumVTerm(value);
}

uint64_t gcAllocateDoubleNumVTerm(double value)
{
    checkAndCleanVTerms(1);

    return allocateDoubleNum(value);
}

uint64_t gcAllocateOpenBracketVTerm(uint64_t length)
{
    checkAndCleanVTerms(1);

    return allocateOpenBracketVTerm(length);
}

uint64_t gcAllocateCloseBracketVTerm(uint64_t length)
{
    checkAndCleanVTerms(1);

    return allocateCloseBracketVTerm(length);
}

static uint64_t allocateClosure(RefalFunc funcPtr, uint32_t paramsCount, struct v_string* ident, int rollback)
{
    struct v_term* term = memMngr.vterms + memMngr.vtermsOffset;
    term->tag = V_CLOSURE_TAG;

    term->closure = (struct v_closure*)(memMngr.data + memMngr.dataOffset);
    memMngr.dataOffset += sizeof(struct v_closure);

    term->closure->params = (struct lterm_t*)(memMngr.data + memMngr.dataOffset);
    memMngr.dataOffset += paramsCount * sizeof(struct lterm_t);

    term->closure->funcPtr = funcPtr;
    term->closure->ident = ident;
    term->closure->paramsCount = paramsCount;
    term->closure->rollback = rollback;

    return memMngr.vtermsOffset++;
}

static struct v_int* allocateIntStruct(uint64_t length)
{
    struct v_int* pointer = (struct v_int*)(memMngr.data + memMngr.dataOffset);
    memMngr.dataOffset += sizeof(struct v_int);

    pointer->bytes = (uint8_t*)(memMngr.data + memMngr.dataOffset);
    pointer->length = length;

    memMngr.dataOffset += length;

    return pointer;
}

static uint64_t allocateSymbolVTerm(uint32_t ch)
{
    memMngr.vterms[memMngr.vtermsOffset].tag = V_CHAR_TAG;
    memMngr.vterms[memMngr.vtermsOffset].ch = ch;

    return memMngr.vtermsOffset++;
}

static uint64_t allocateIntNumVTerm(struct v_int* value)
{
    memMngr.vterms[memMngr.vtermsOffset].tag = V_INT_NUM_TAG;
    memMngr.vterms[memMngr.vtermsOffset].intNum = value;
    return memMngr.vtermsOffset++;
}

static uint64_t allocateDoubleNum(double value)
{
    memMngr.vterms[memMngr.vtermsOffset].tag = V_DOUBLE_NUM_TAG;
    memMngr.vterms[memMngr.vtermsOffset].doubleNum = value;
    return memMngr.vtermsOffset++;
}

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

/// Память для литералов выделяется с помощью malloc'а. Т.е. не в куче данных.
struct v_string* allocateVStringLiteral(uint32_t* runes, uint64_t length)
{
    struct v_string* pointer = (struct v_string*)malloc(sizeof(struct v_string));

    pointer->head = (uint32_t*) malloc(length * sizeof(uint32_t));
    pointer->length = length;

    memcpy(pointer->head, runes, length * sizeof(uint32_t));

    return pointer;
}

/// Память для литералов выделяется с помощью malloc'а. Т.е. не в куче данных.
struct v_int* allocateIntNumberLiteral(uint8_t* bytes, uint8_t sign, uint64_t length)
{
    struct v_int* pointer = (struct v_int*)malloc(sizeof(struct v_int));

    pointer->bytes = (uint8_t*) malloc(length * sizeof(uint8_t));
    pointer->length = length;
    pointer->sign = sign;

    memcpy(pointer->bytes, bytes, length * sizeof(uint8_t));

    return pointer;
}




