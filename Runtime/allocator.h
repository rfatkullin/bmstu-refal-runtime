#ifndef __ALLOCATOR_H__
#define __ALLOCATOR_H__

#include <stdint.h>

#include "v_term.h"
#include "l_term.h"

//Количество элементов в листе дерева отрезков
#define N 1

//Сколько процентов памяти выдялется тому или иному типу данных
//Память для дерева отрезков берется из памяти для v_term'ов.
#define V_TERMS_HEAP_SIZE_FACTOR 		0.3f
#define DATA_HEAP_SIZE_FACTOR 			0.4f
#define L_TERMS_HEAP_SIZE_FACTOR		0.3f

struct memory_manager
{	
	uint8_t* activeTermsHeap;
	uint8_t* inactiveTermsHeap;
	
	uint32_t* segmentTree;
	
	uint8_t* activeDataHeap;
	uint8_t* inactiveDataHeap;
	
	uint8_t* lTermsHeap;
	
	uint32_t vtermsOffset;
	uint32_t dataOffset;
	uint32_t ltermsOffset;		
		
	uint32_t size;
};

struct memory_manager memoryManager;

void initAllocator(uint32_t size);
void markTerms(struct l_term* term);
struct v_term* allocate(struct l_term* expr);
	
#endif