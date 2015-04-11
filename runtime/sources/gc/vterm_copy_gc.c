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
                    // пока vterm'ы идентификаторов могут хранятся только в области литеральных vterm'ов
                    // поэтому данные можно не копировать.
                    _memMngr.vterms[to] = _memMngr.vterms[from];
                    break;
            }

            _memMngr.vterms[to].tag = _memMngr.vterms[from].tag;
            _memMngr.vtermsOffset++;
            _memMngr.vterms[from].brackets = (struct fragment_t*)to; // Casting to hide warn! Save new offset in 'brackets' field.
        }
    }
}

static void copyClosureVTerm(uint64_t to, struct vclosure_t* closure)
{
    // Если указатель на функцию не нулевой, значит замыкание еще не скопировано в новую кучу.
    if (closure->funcPtr)
    {
        GC_DATA_HEAP_CHECK_EXIT(VCLOSURE_SIZE(closure->paramsCount));

        _memMngr.vterms[to].closure =
                allocateClosureStruct(closure->funcPtr, closure->paramsCount, closure->ident, closure->rollback);

        memcpy(_memMngr.vterms[to].closure->params, closure->params, closure->paramsCount * sizeof(struct fragment_t));

        // Отмечаем что замыкание скопировано и сохраняем адрес.
        closure->funcPtr = 0;
        closure->params = (struct fragment_t*)_memMngr.vterms[to].closure;
    }
    else  // Замыкание уже скопировано.
    {
        _memMngr.vterms[to].closure = (struct vclosure_t*)closure->params;
    }
}

static void copyIntVTerm(uint64_t to, struct vint_t* intNum)
{
    // Данные были выделены с помощью malloc'a. Они не перемещаются
    // из кучи в кучу.
    if (!ADDR_IN_INACTIVE_HEAP((uint8_t*)intNum))
    {
        _memMngr.vterms[to].intNum = intNum;
    }
    else if (CHECK_MST_SIGN_BIT(intNum->length)) // Данные были скопированы - береме только адрес.
    {
        _memMngr.vterms[to].intNum = (struct vint_t*)intNum->bytes;
    }
    else // Копируем данные и сохраняем новый адрес.
    {
        GC_DATA_HEAP_CHECK_EXIT(VINT_STRUCT_SIZE(intNum->length));

        struct vint_t* newIntNum = allocateIntStruct(intNum->length);
        newIntNum->sign = intNum->sign;
        memcpy(newIntNum->bytes, intNum->bytes, intNum->length);
        _memMngr.vterms[to].intNum = newIntNum;

        // Отмечаем что число скопировано и сохраняем адрес.
        SET_MST_SIGN_BIT(intNum->length);
        intNum->bytes = (uint8_t*)newIntNum;
    }
}

static void copyBracketsVTerm(uint64_t to, struct fragment_t* frag)
{
    // Фрагмент уже скопирован в новую кучу.
    if (CHECK_MST_SIGN_BIT(frag->length))
    {
        _memMngr.vterms[to].brackets = _memMngr.vterms[frag->offset].brackets;
    }
    else  // Копируем и отмечаем, что скопировали.
    {
        GC_DATA_HEAP_CHECK_EXIT(FRAGMENT_STRUCT_SIZE(1));

        _memMngr.vterms[to].brackets = allocateFragment(1);
        memcpy(_memMngr.vterms[to].brackets, frag, FRAGMENT_STRUCT_SIZE(1));

        // Отмечаем что скобки скопированы и сохраняем смещение.
        SET_MST_SIGN_BIT(frag->length);
        frag->offset = to;
    }
}

static void swap(uint64_t* a, uint64_t* b)
{
    uint64_t tmp = *a;
    *a = *b;
    *b = tmp;
}
