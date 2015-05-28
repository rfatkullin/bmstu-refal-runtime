#ifndef _BMSTU_REFC_V_INT_H
#define _BMSTU_REFC_V_INT_H

#include <stdint.h>

#define INT_LENGTH_MASK                     UINT64_C(0x3FFFFFFFFFFFFFFF)
#define GET_INT_LENGTH(var)                 (var->info & 0x3FFFFFFFFFFFFFFF)  // Вряд ли длина будет больше.
#define SET_INT_LENGTH(var, newLength)      (var->info = newLength & INT_LENGTH_MASK)

#define INT_SIGN_MASK           UINT64_C(0x4000000000000000)
#define GET_INT_SIGN(var)       (var->info & INT_SIGN_MASK)
#define SET_INT_SIGN(var)       (var->info |= INT_SIGN_MASK)

/// vterm целочисленных чисел.
struct vint_t
{
    /// Байты для хранения большого числа.
    uint8_t* bytes;

    /// Сколько байтов занимает число.
    /// 0|0|000000.000000000000000000
    uint64_t info;
};

#endif
