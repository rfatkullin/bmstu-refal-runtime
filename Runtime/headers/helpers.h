#ifndef __HELPERS_H__
#define __HELPERS_H__

#include "memory_manager.h"

typedef int  allocate_result;

//TO DO: Перенести в gc.h или memory_manager.h

#define OK                  0
#define NEED_DATA_CLEAN     1
#define NEED_VTERM_CLEAN    2

/// Изменяет длину выражения в скобках.
void changeBracketLength(uint64_t offset, uint64_t newLength);

/// Проверка на переполнение и очистка хипов.
void checkAndCleanVTerms(uint64_t);
void checkAndCleanData(uint64_t);

/// Проверка переполнения хипов
void checkVTermsOverflow(uint64_t);
void checkDataOverflow(uint64_t);

/// Переполен хотя бы один из хипов
int isOverflowed();

#endif
