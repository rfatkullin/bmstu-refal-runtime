#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <allocators/data_alloc.h>
#include <allocators/vterm_alloc.h>
#include <memory_manager.h>

/// Функции выделения vterm'ов без проверок.
static uint64_t allocateClosureVterm();

int allocateVTerms(struct fragment_t* frag)
{
    if (checkVTermsOverflow(frag->length))
        return 0;

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
                memMngr.vterms[memMngr.vtermsOffset++].closure = memMngr.vterms[frag->offset + i].closure;
                break;

            case V_BRACKET_OPEN_TAG:
            case V_BRACKET_CLOSE_TAG:
                memMngr.vterms[memMngr.vtermsOffset++].inBracketLength = memMngr.vterms[frag->offset + i].inBracketLength;
                break;
        }        
    }

    return 1;
}

uint64_t chAllocateClosureVTerm(allocate_result* res)
{
    if (memMngr.vtermsOffset + 1 > memMngr.vtermsMaxOffset)
    {
        *res = NEED_CLEAN;
        return 0;
    }

    *res = OK;

    return allocateClosureVterm();
}

uint64_t allocateDoubleNumVTerm(double value)
{
    memMngr.vterms[memMngr.vtermsOffset].tag = V_DOUBLE_NUM_TAG;
    memMngr.vterms[memMngr.vtermsOffset].doubleNum = value;
    return memMngr.vtermsOffset++;
}

uint64_t gcAllocateIntNumVTerm(struct v_int* value)
{
    checkAndCleanVTerms(1);

    return allocateIntNumVTerm(value);
}

uint64_t gcAllocateDoubleNumVTerm(double value)
{
    checkAndCleanVTerms(1);

    return allocateDoubleNumVTerm(value);
}

uint64_t gcAllocateClosureVTerm()
{
    checkAndCleanVTerms(1);

    return allocateClosureVterm();
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

static uint64_t allocateClosureVterm()
{
    memMngr.vterms[memMngr.vtermsOffset].tag = V_CLOSURE_TAG;

    return memMngr.vtermsOffset++;
}

uint64_t allocateSymbolVTerm(uint32_t ch)
{
    memMngr.vterms[memMngr.vtermsOffset].tag = V_CHAR_TAG;
    memMngr.vterms[memMngr.vtermsOffset].ch = ch;

    return memMngr.vtermsOffset++;
}

uint64_t allocateIntNumVTerm(struct v_int* value)
{
    memMngr.vterms[memMngr.vtermsOffset].tag = V_INT_NUM_TAG;
    memMngr.vterms[memMngr.vtermsOffset].intNum = value;
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
