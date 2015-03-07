#ifndef __DATA_ALLOCATORS_H___
#define __DATA_ALLOCATORS_H___

#include "lterm.h"

/// Выделяет память в хипе под lterm типа L_TERM_FRAGMENT_TAG
/// В процессе выполнения может быть вызван сборщик мусора для lterm'ов.
struct lterm_t* gcAllocateFragmentLTerm(uint32_t count);

/// Выделяет память в хипе под lterm типа L_TERM_FUNC_CALL
struct lterm_t* allocateFuncCallLTerm();

/// Выделяет память в хипе под lterm типа L_TERM_CHAIN_TAG
struct lterm_t* allocateChainLTerm(uint32_t count);

/// Выделяет память для L_TERM_CHAIN_TAG, добавляет L_TERM_FRAGMENT_TAG с пере-
/// данными значениями offset и length.
struct lterm_t* allocateBuiltinsResult(uint64_t offset, uint64_t length);

/// Выделяет память для полей структуры env_t.
struct env_t* allocateEnvData(struct env_t* env, uint32_t localsCount, uint32_t patternsCount);

/// Выделяет память с помощью malloc'а под структуру v_string
struct v_string* allocateVStringLiteral(uint32_t* runes, uint64_t length);

/// Выделяет память с помощью malloc'а под структуру v_int
struct v_int* allocateIntNumberLiteral(uint8_t* bytes, uint8_t sign, uint64_t length);

/// Выделяет память в хипе под структуру v_int
/// В процессе выполнения может быть вызван сборщик мусора для vterm'ов.
struct v_int* gcAllocateIntStruct(uint64_t length);

#endif
