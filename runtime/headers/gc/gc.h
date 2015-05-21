#ifndef _BMSTU_REFC_GC_H_
#define _BMSTU_REFC_GC_H_

#include <helpers.h>
#include <memory_manager.h>
#include <defines/errors_str.h>

typedef int  allocate_result;

#define GC_OK          1
#define GC_NEED_CLEAN  2

/// Этапы сборки vterm'ов.
#define GC_VTERMS_MARK_STAGE            1
#define GC_VTERMS_SET_ACTUAL_STAGE      2

#define GC_MOVED_MASK   UINT64_C(0x8000000000000000)

struct gc
{
    /// В массиве отмечаются использованные vterm'ы.
    uint8_t* inUseVTerms;

    /// Этап сборки vterm'ов.
    int stage;
};

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
void collectVTermGarbage();

void collectDataGarbage();

/// Проверяет есть ли в кучах место для needTermCount vterm'ов
/// и needDataSize байтов для данных. Если нет, запускает сборку мусора
/// Если сборка мусора ничего не дает, программа завершается.
int checkAndCleanHeaps(uint64_t needTermCount, uint64_t needDataSize);

/// Копирует отмеченные vterm'ы в новую кучу.
void copyVTerms();

struct gc _gc;

#endif
