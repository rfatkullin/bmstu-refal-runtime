#ifndef _BMSTU_REFC_GC_H_
#define _BMSTU_REFC_GC_H_

#include <helpers.h>
#include <memory_manager.h>
#include <defines/errors_str.h>

typedef int  allocate_result;

#define GC_OK          1
#define GC_NEED_CLEAN  2

/// Собирает мусор.
void collectGarbage();

/// Копирует список в текущую активную кучу.
/// Передается указатель на головной lterm списка,
/// который нужно скопировать.
/// Возвращает указатель на головной lterm нового списка.
struct lterm_t* copySimpleChain(struct lterm_t* chain);

/// Перемещает достижимые vterm'ы активной кучи в пассивную кучу.
/// В процессе работы менят местами активную и пассивную кучи (как
/// для vterm'ов, так и для данных).
void collectVTermGarbage(struct lterm_t* fieldOfView);

/// Проверяет есть ли в кучах место для needTermCount vterm'ов
/// и needDataSize байтов для данных. Если нет, запускает сборку мусора
/// Если сборка мусора ничего не дает, программа завершается.
int checkAndCleanHeaps(uint64_t needTermCount, uint64_t needDataSize);

#endif
