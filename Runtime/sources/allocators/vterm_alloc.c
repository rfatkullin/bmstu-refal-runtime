#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <allocators/data_alloc.h>
#include <allocators/vterm_alloc.h>
#include <memory_manager.h>

uint64_t gcAllocateBracketVterm()
{
    checkAndCleanHeaps(1, sizeof(struct fragment_t));

    return allocateBracketsVTerm();
}

allocate_result allocateVTerms(struct fragment_t* frag)
{    
    if (GC_VTERM_OV(frag->length))
        return GC_NEED_CLEAN;

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

            case V_BRACKETS_TAG:
                memMngr.vterms[memMngr.vtermsOffset++].brackets = memMngr.vterms[frag->offset + i].brackets;
                break;
        }        
    }

    return GC_OK;
}

uint64_t chAllocateClosureVTerm(allocate_result* res)
{
    GC_VTERM_HEAP_CHECK_RETURN(1, *res);

    return allocateClosureVTerm();
}

uint64_t allocateDoubleNumVTerm(double value)
{
    memMngr.vterms[memMngr.vtermsOffset].tag = V_DOUBLE_NUM_TAG;
    memMngr.vterms[memMngr.vtermsOffset].doubleNum = value;
    return memMngr.vtermsOffset++;
}

uint64_t allocateUInt8VTerm(uint8_t val)
{    
    struct vint_t* num = allocateIntStruct(1);
    num->sign = 0;
    *num->bytes = val;

    return allocateIntNumVTerm(num);
}

uint64_t allocateClosureVTerm()
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

uint64_t allocateIntNumVTerm(struct vint_t* value)
{
    memMngr.vterms[memMngr.vtermsOffset].tag = V_INT_NUM_TAG;
    memMngr.vterms[memMngr.vtermsOffset].intNum = value;
    return memMngr.vtermsOffset++;
}

uint64_t allocateBracketsVTerm()
{
    memMngr.vterms[memMngr.vtermsOffset].tag = V_BRACKETS_TAG;    
    memMngr.vterms[memMngr.vtermsOffset].brackets = (struct fragment_t*)(memMngr.data + memMngr.dataOffset);
    memMngr.dataOffset += sizeof(struct fragment_t);

    return memMngr.vtermsOffset++;
}

void setBracketsData(uint64_t bracketsTermoffset, uint64_t offset, uint64_t length)
{    
    memMngr.vterms[bracketsTermoffset].brackets->offset = offset;
    memMngr.vterms[bracketsTermoffset].brackets->length = length;
}

/// Память для литералов выделяется с помощью malloc'а. Т.е. не в куче данных.
struct vstring_t* allocateVStringLiteral(uint32_t* runes, uint64_t length)
{
    struct vstring_t* pointer = (struct vstring_t*)malloc(sizeof(struct vstring_t));

    pointer->head = (uint32_t*) malloc(length * sizeof(uint32_t));
    pointer->length = length;

    memcpy(pointer->head, runes, length * sizeof(uint32_t));

    return pointer;
}

/// Память для литералов выделяется с помощью malloc'а. Т.е. не в куче данных.
struct vint_t* allocateIntNumberLiteral(uint8_t* bytes, uint8_t sign, uint64_t length)
{
    struct vint_t* pointer = (struct vint_t*)malloc(sizeof(struct vint_t));

    pointer->bytes = (uint8_t*) malloc(length * sizeof(uint8_t));
    pointer->length = length;
    pointer->sign = sign;

    memcpy(pointer->bytes, bytes, length * sizeof(uint8_t));

    return pointer;
}
