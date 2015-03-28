#ifndef __V_TERM_ALLOCATORS_H__
#define __V_TERM_ALLOCATORS_H__

#include <vterm.h>
#include <gc/gc.h>

/// Выделяют память в хипе vterm'ов
/// Статус выполнения возвращается в аргументе res.
/// Если памяти хватило и она выделена,то в *res будет Ok,
/// в противном случае NEED_DATA_CLEAN
uint64_t chAllocateClosureVTerm(allocate_result* res);

/// Выдыляет память под vterm без проверок.
uint64_t allocateBracketsVTerm();
uint64_t allocateIntNumVTerm(struct vint_t* value);
uint64_t allocateDoubleNumVTerm(double value);
uint64_t allocateSymbolVTerm(uint32_t ch);
uint64_t allocateUInt8VTerm(uint8_t val);
uint64_t allocateClosureVTerm();

/// Копирует термы из фрагмента в новую область.
allocate_result allocateVTerms(struct fragment_t* fragment_t);

/// Изменяет длину выражения в скобках.
void setBracketsData(uint64_t bracketsTermoffset, uint64_t offset, uint64_t length);

uint64_t gcAllocateBracketVterm();

#endif
