#ifndef __V_TERM_ALLOCATORS_H__
#define __V_TERM_ALLOCATORS_H__

#include "vterm.h"

/// Выдыляет память под vterm типа V_BRACKET_OPEN_TAG или V_BRACKET_CLOSE_TAG
/// В процессе выполнения может быть вызван сборщик мусора для vterm'ов.
uint64_t gcAllocateOpenBracketVTerm(uint64_t length);
uint64_t gcAllocateCloseBracketVTerm(uint64_t length);

/// Выделяет память под один символ и возвращает смещение для v_term
/// В процессе выполнения может быть вызван сборщик мусора для vterm'ов.
uint64_t gcAllocateSymbolVTerm(uint32_t ch);

/// Выделяет память под vterm для целочисленного значения
/// В процессе выполнения может быть вызван сборщик мусора для vterm'ов.
uint64_t gcAllocateIntNumVTerm(struct v_int* value);

/// Выделяет память под vterm для вещественного значения
/// В процессе выполнения может быть вызван сборщик мусора для vterm'ов.
uint64_t gcAllocateDoubleNumVTerm(double value);

/// Выдыляет память под vterm типа V_BRACKET_OPEN_TAG
uint64_t allocateOpenBracketVTerm(uint64_t length);

/// Выдыляет память под vterm типа V_BRACKET_CLOSE_TAG
uint64_t allocateCloseBracketVTerm(uint64_t length);

/// Выделяет память под vterm'ы
int allocateVTerms(struct fragment_t* fragment_t);

/// Выделяет память для замыкания
uint64_t gcAllocateClosure(RefalFunc ptr, uint32_t paramsCount, struct v_string* ident, int rollback);

#endif
