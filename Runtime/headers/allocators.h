#ifndef __ALLOCATORS_H__
#define __ALLOCATORS_H__

#include "memory_manager.h"

/// Выделяет память под vterm'ы
void allocateVTerms(struct fragment_t* fragment_t);

/// Выдыляет память под vterm типа V_BRACKET_OPEN_TAG или V_BRACKET_CLOSE_TAG
uint64_t allocateOpenBracketVTerm(uint64_t length);
uint64_t allocateCloseBracketVTerm(uint64_t length);

/// Изменяет длину выражения в скобках.
void changeBracketLength(uint64_t offset, uint64_t newLength);

/// Выделяет память под один символ и возвращает смещение для v_term
uint64_t allocateSymbol(uint32_t ch);

/// Выделяет память под vterm для целочисленного значения
uint64_t allocateIntNum(uint64_t count);

/// Выделяет память под vterm для вещественного значения
uint64_t allocateDoubleNum();

/// Выделяет память для замыкания
uint64_t allocateClosure(RefalFunc ptr, uint32_t paramsCount, struct v_string* ident, int rollback);

/// Выделяет память с помощью malloc'а под структуру v_string
struct v_string* allocateVStringLiteral(uint32_t* runes, uint64_t length);

/// Выделяет память с помощью malloc'а под структуру v_int
struct v_int* allocateIntNumberLiteral(uint8_t* bytes, uint8_t sign, uint64_t length);

/// Выделяет память в хипе под структуру v_int
struct v_int* allocateIntNumber(uint64_t length);

/// Выделяет память в хипе под lterm типа L_TERM_FRAGMENT_TAG
struct lterm_t* allocateFragmentLTerm(uint32_t count);

/// Выделяет память в хипе под lterm типа L_TERM_FUNC_CALL
struct lterm_t* allocateFuncCallLTerm();

/// Выделяет память в хипе под lterm типа L_TERM_CHAIN_TAG
struct lterm_t* allocateChainLTerm(uint64_t count);

/// Выделяет память для L_TERM_CHAIN_TAG, добавляет L_TERM_FRAGMENT_TAG с пере-
/// данными значениями offset и length.
struct lterm_t* allocateBuiltinsResult(uint64_t offset, uint64_t length);

/// Выделяет память для полей структуры env_t.
struct lterm_t* allocateEnvData(struct env_t* env, uint32_t localsCount, uint32_t patternsCount);

#endif
