#ifndef __ALLOCATORS_H__
#define __ALLOCATORS_H__

#include "memory_manager.h"

/// Изменяет длину выражения в скобках.
void changeBracketLength(uint64_t offset, uint64_t newLength);

/// Проверки на переполнение.
void checkAndCleanVTerms(uint64_t);
void checkAndCleanData(uint64_t);

#endif
