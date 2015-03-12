#ifndef __ALLOCATOR_H__
#define __ALLOCATOR_H__

#include <stdint.h>

#include "vterm.h"
#include "lterm.h"

//Сколько процентов памяти выдялется тому или иному типу данных
//Память для дерева отрезков берется из памяти для v_term'ов.
#define V_TERMS_HEAP_SIZE_FACTOR 		0.4f
#define DATA_HEAP_SIZE_FACTOR 			0.6f

struct memory_manager
{
	// Указатель на начало vterm'ов
	struct v_term* vterms;
    uint64_t vtActiveOffset;
    uint64_t vtInactiveOffset;

	// Указатели на данные
    uint8_t* data;
    uint64_t dtActiveOffset;
    uint64_t dtInactiveOffset;

    // Текущие сдвиги
    uint64_t vtermsOffset;
    uint64_t dataOffset;    

    // Вспомогательный массив для GC. Отмечаются использованные vterm'ы.
    uint8_t* gcInUseVTerms;

    // Начиная с какого vterm'а начинаются не литеральные vterm'ы в массиве vterms
    uint64_t vtermsBeginOffset;

    // Максимальный размер массива vterm'ов.
    uint64_t vtermsMaxOffset;	

    // Максимальный размер данных.
    uint64_t dataMaxOffset;

    // Размер выделенного участка
    uint64_t totalSize;

    // Поле зрения
    struct lterm_t* fieldOfView;
};

struct memory_manager memMngr;

/// Выделяет память размера size
/// и сохраняет указатель на выделенный участок
/// в переменной mainHeap.
void initAllocator(uint64_t size);

/// Распределеяет память для типов данных
/// т.е. инциализирует поля activeTermsHeap, inactiveTermsHeap и т.д.
void initHeaps(uint64_t literalsNumber);

/// Собирает мусор.
void collectGarbage();

/// Дебажный вывод vterm
void debugLiteralsPrint();

void collectVTermGarbage();
void collectDataGarbage();

#endif
