#ifndef __HELPERS_H__
#define __HELPERS_H__

#include "memory_manager.h"

typedef int  allocate_result;

//TO DO: Перенести в gc.h или memory_manager.h

#define OK          0
#define NEED_CLEAN  1

/// Изменяет длину выражения в скобках.
void changeBracketLength(uint64_t offset, uint64_t newLength);

/// Проверка на переполнение и очистка хипов.
int checkAndCleanVTerms(uint64_t);
int checkAndCleanData(uint64_t);

/// Проверка переполнения хипов
int checkVTermsOverflow(uint64_t);
int checkDataOverflow(uint64_t);

void strictCheckVTermsOverflow(uint64_t needVTermsCount);

#endif
