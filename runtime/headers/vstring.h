#ifndef _BMSTU_REFC_VSTRING_H_
#define _BMSTU_REFC_VSTRING_H_

#include <stdint.h>

#define VSTRING_LENGTH_MASK                     UINT64_C(0x7fffffffffffffff)
#define GET_VSTRING_LENGTH(var)                 (var->length & VSTRING_LENGTH_MASK)  // Вряд ли длина будет больше.
#define SET_VSTRING_LENGTH(var, newLength)      (var->length = newLength & VSTRING_LENGTH_MASK)

/// vterm строк Юникода.
struct vstring_t
{
    /// Для хранения кодовых точек Юникода.
    uint32_t* head;

    /// Длина Юникод строки
    uint64_t length;
};

#endif
