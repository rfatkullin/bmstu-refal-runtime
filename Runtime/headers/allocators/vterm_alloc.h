#ifndef __V_TERM_ALLOCATORS_H__
#define __V_TERM_ALLOCATORS_H__

#include <vterm.h>
#include <gc/gc.h>

/// Выделяют память в хипе vterm'ов
/// Статус выполнения возвращается в аргументе res.
/// Если памяти хватило и она выделена,то в *res будет Ok,
/// в противном случае NEED_DATA_CLEAN
uint64_t chAllocateClosureVTerm(allocate_result* res);

/// Выдыляют память под vterm
/// В процессе выполнения может быть вызван сборщик мусора для vterm'ов.
uint64_t gcAllocateOpenBracketVTerm(uint64_t length);
uint64_t gcAllocateCloseBracketVTerm(uint64_t length);
uint64_t gcAllocateIntNumVTerm(struct v_int* value);
uint64_t gcAllocateDoubleNumVTerm(double value);
uint64_t gcAllocateClosureVTerm();
uint64_t gcAllocateUInt8VTerm(uint8_t val);

/// Выдыляет память под vterm без проверок.
uint64_t allocateOpenBracketVTerm(uint64_t length);
uint64_t allocateCloseBracketVTerm(uint64_t length);
uint64_t allocateIntNumVTerm(struct v_int* value);
uint64_t allocateDoubleNumVTerm(double value);
uint64_t allocateSymbolVTerm(uint32_t ch);

/// Копирует термы из фрагмента в новую область.
int allocateVTerms(struct fragment_t* fragment_t);

/// Изменяет длину выражения в скобках.
void changeBracketLength(uint64_t offset, uint64_t newLength);

#endif
