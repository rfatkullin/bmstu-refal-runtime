#ifndef __V_TERM_H__
#define __V_TERM_H__

#define V_CHAR_TAG          0
#define V_IDENT_TAG         1
#define V_INT_NUM_TAG       2
#define V_DOUBLE_NUM_TAG    3
#define V_CLOSURE_TAG       4
#define V_BRACKETS_TAG      5

#include <stdint.h>

#include "lterm.h"
#include "func_call.h"

/// vterm функций.
struct vclosure_t
{
    RefalFunc           funcPtr;
    struct fragment_t*  params;
    struct vstring_t*   ident;
    uint32_t            paramsCount;
    int                 rollback;
};

/// vterm строк Юникода.
struct vstring_t
{
    /// Для хранения кодовых точек Юникода.
    uint32_t* head;

    /// Длина Юникод строки
    uint64_t length;
};

/// vterm целочисленных чисел.
struct vint_t
{
    /// Байты для хранения большого числа.
    uint8_t* bytes;

    /// Знак числа. 0 - число положительное, в противном случае отрицательное.
    uint8_t sign;

    /// Сколько байтов занимает число.
    uint64_t length;
};

struct vterm_t
{
	int tag;

	union
	{
		uint32_t ch;
        double doubleNum;
        struct vstring_t* str;
        struct vclosure_t* closure;
        struct vint_t* intNum;
        struct fragment_t* brackets;
	};
};

#endif
