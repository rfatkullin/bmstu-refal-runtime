#ifndef __GC_H__
#define __GC_H__

#include <memory_manager.h>

typedef int  allocate_result;

#define OK          0
#define NEED_CLEAN  1

/// Собирает мусор.
void collectGarbage();

void collectVTermGarbage();
void collectDataGarbage();

/// Проверка на переполнение и очистка хипов.
int checkAndCleanVTerms(uint64_t);
int checkAndCleanData(uint64_t);

/// Проверка переполнения хипов
int checkVTermsOverflow(uint64_t);
int checkDataOverflow(uint64_t);

void strictCheckVTermsOverflow(uint64_t needVTermsCount);

#endif
