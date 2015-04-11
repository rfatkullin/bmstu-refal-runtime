#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <memory_manager.h>
#include <defines/gc_macros.h>
#include <allocators/data_alloc.h>
#include <allocators/vterm_alloc.h>
#include <defines/data_struct_sizes.h>

uint64_t allocateVTerms(struct fragment_t* frag, allocate_result* res)
{
    uint64_t resOffset = _memMngr.vtermsOffset;
    GC_VTERM_HEAP_CHECK_RETURN(frag->length, *res);

    uint64_t i = 0;
    for (i = 0; i < frag->length; ++i)
    {
        _memMngr.vterms[_memMngr.vtermsOffset].tag = _memMngr.vterms[frag->offset + i].tag;

        switch (_memMngr.vterms[frag->offset + i].tag)
        {
            case V_CHAR_TAG:
                _memMngr.vterms[_memMngr.vtermsOffset++].ch = _memMngr.vterms[frag->offset + i].ch;
                break;

            case V_IDENT_TAG :
                _memMngr.vterms[_memMngr.vtermsOffset++].str = _memMngr.vterms[frag->offset + i].str;
                break;

            case V_INT_NUM_TAG:
                _memMngr.vterms[_memMngr.vtermsOffset++].intNum = _memMngr.vterms[frag->offset + i].intNum;
                break;

            case V_DOUBLE_NUM_TAG:
                _memMngr.vterms[_memMngr.vtermsOffset++].doubleNum = _memMngr.vterms[frag->offset + i].doubleNum;
                break;

            case V_CLOSURE_TAG:
                _memMngr.vterms[_memMngr.vtermsOffset++].closure = _memMngr.vterms[frag->offset + i].closure;
                break;

            case V_BRACKETS_TAG:
                _memMngr.vterms[_memMngr.vtermsOffset++].brackets = _memMngr.vterms[frag->offset + i].brackets;
                break;
        }        
    }

    return resOffset;
}

uint64_t chAllocateBracketVterm(allocate_result* res)
{
    GC_VTERM_HEAP_CHECK_RETURN(1, *res);
    GC_DATA_HEAP_CHECK_RETURN(FRAGMENT_STRUCT_SIZE(1), *res);

    return allocateBracketsVTerm();
}

uint64_t chAllocateClosureVTerm(allocate_result* res)
{
    GC_VTERM_HEAP_CHECK_RETURN(1, *res);

    return allocateClosureVTerm();
}

uint64_t allocateDoubleNumVTerm(double value)
{
    _memMngr.vterms[_memMngr.vtermsOffset].tag = V_DOUBLE_NUM_TAG;
    _memMngr.vterms[_memMngr.vtermsOffset].doubleNum = value;
    return _memMngr.vtermsOffset++;
}

uint64_t allocateUInt8VTerm(uint8_t val)
{    
    struct vint_t* num = allocateIntStruct(1);    
    *num->bytes = val;

    return allocateIntNumVTerm(num);
}

uint64_t allocateClosureVTerm()
{
    _memMngr.vterms[_memMngr.vtermsOffset].tag = V_CLOSURE_TAG;

    return _memMngr.vtermsOffset++;
}

uint64_t allocateSymbolVTerm(uint32_t ch)
{
    _memMngr.vterms[_memMngr.vtermsOffset].tag = V_CHAR_TAG;
    _memMngr.vterms[_memMngr.vtermsOffset].ch = ch;

    return _memMngr.vtermsOffset++;
}

uint64_t allocateIntNumVTerm(struct vint_t* value)
{
    _memMngr.vterms[_memMngr.vtermsOffset].tag = V_INT_NUM_TAG;
    _memMngr.vterms[_memMngr.vtermsOffset].intNum = value;
    return _memMngr.vtermsOffset++;
}

uint64_t allocateBracketsVTerm()
{
    _memMngr.vterms[_memMngr.vtermsOffset].tag = V_BRACKETS_TAG;
    _memMngr.vterms[_memMngr.vtermsOffset].brackets = (struct fragment_t*)(_memMngr.data + _memMngr.dataOffset);
    _memMngr.dataOffset += sizeof(struct fragment_t);

    return _memMngr.vtermsOffset++;
}

void setBracketsData(uint64_t bracketsTermoffset, uint64_t offset, uint64_t length)
{    
    _memMngr.vterms[bracketsTermoffset].brackets->offset = offset;
    _memMngr.vterms[bracketsTermoffset].brackets->length = length;
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
    SET_INT_LENGTH(pointer, length);

    if (sign)
        SET_INT_SIGN(pointer);

    memcpy(pointer->bytes, bytes, length * sizeof(uint8_t));

    return pointer;
}
