#include <string.h>

#include <gc/gc.h>
#include <memory_manager.h>
#include <defines/gc_macros.h>
#include <allocators/data_alloc.h>
#include <defines/data_struct_sizes.h>

static void copyClosureVTerm(uint64_t to, struct vclosure_t* closure);
static void copyIntVTerm(uint64_t to, struct vint_t* intNum);
static void copyBracketsVTerm(uint64_t to, struct fragment_t* frag);
static void swap(uint64_t* a, uint64_t* b);

void copyVTerms()
{
    swap(&_memMngr.vtInactiveOffset, &_memMngr.vtActiveOffset);
    swap(&_memMngr.dtInactiveOffset, &_memMngr.dtActiveOffset);

    _memMngr.vtermsOffset = _memMngr.vtActiveOffset;
    _memMngr.dataOffset = _memMngr.dtActiveOffset;

    uint64_t i = 0;
    for (i = 0; i < _memMngr.vtermsMaxOffset; ++i)
    {
        if (_gc.inUseVTerms[i])
        {
            GC_VTERM_HEAP_CHECK_EXIT(1);

            uint64_t to = _memMngr.vtermsOffset;
            uint64_t from = _memMngr.vtInactiveOffset + i;

            switch (_memMngr.vterms[from].tag)
            {
                case V_INT_NUM_TAG:
                    copyIntVTerm(to, _memMngr.vterms[from].intNum);
                    break;
                case V_CLOSURE_TAG:
                    copyClosureVTerm(to, _memMngr.vterms[from].closure);
                    break;

                case V_BRACKETS_TAG:
                    copyBracketsVTerm(to, _memMngr.vterms[from].brackets);
                    break;

                case V_CHAR_TAG:
                case V_DOUBLE_NUM_TAG:
                    _memMngr.vterms[to] = _memMngr.vterms[from];
                    break;

                case V_IDENT_TAG:
                    // Ident structs only in literals data area --> no need to copy to new place. Just copy.
                    _memMngr.vterms[to] = _memMngr.vterms[from];
                    break;
            }

            _memMngr.vterms[to].tag = _memMngr.vterms[from].tag;
            _memMngr.vtermsOffset++;
            _memMngr.vterms[from].brackets = (struct fragment_t*)to; // Casting to hide warn! Save new offset in 'brackets' field.
        }
    }
}



// TO FIX: Должно копироваться только один раз.
// Параметры копируются криво.
static void copyClosureVTerm(uint64_t to, struct vclosure_t* closure)
{
    GC_DATA_HEAP_CHECK_EXIT(VCLOSURE_SIZE(closure->paramsCount));

    _memMngr.vterms[to].closure =
            allocateClosureStruct(closure->funcPtr, closure->paramsCount, closure->ident, closure->rollback);

    memcpy(_memMngr.vterms[to].closure->params, closure->params, closure->paramsCount * sizeof(struct fragment_t));
}

// TO FIX: Должно копироваться только один раз.
static void copyIntVTerm(uint64_t to, struct vint_t* intNum)
{
    GC_DATA_HEAP_CHECK_EXIT(VINT_STRUCT_SIZE(intNum->length));

    struct vint_t* newIntNum = allocateIntStruct(intNum->length);
    newIntNum->sign = intNum->sign;
    memcpy(newIntNum->bytes, intNum->bytes, intNum->length);
    _memMngr.vterms[to].intNum = newIntNum;
}

static void copyBracketsVTerm(uint64_t to, struct fragment_t* frag)
{
    GC_DATA_HEAP_CHECK_EXIT(FRAGMENT_STRUCT_SIZE(1));

    _memMngr.vterms[to].brackets = allocateFragment(1);
    memcpy(_memMngr.vterms[to].brackets, frag, FRAGMENT_STRUCT_SIZE(1));
}

static void swap(uint64_t* a, uint64_t* b)
{
    uint64_t tmp = *a;
    *a = *b;
    *b = tmp;
}
