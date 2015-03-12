#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "memory_manager.h"

static void allocateMemoryForSegmentTree(uint64_t termsNumber, uint8_t** pointer);
static void allocateMemoryForVTerms(uint64_t size, uint8_t** pointer);
static void allocateMemoryForData(uint64_t size, uint8_t** pointer);

void initAllocator(uint64_t size)
{
	memMngr.vterms = (struct v_term*)malloc(size);
	memMngr.totalSize = size;
}

void initHeaps(uint64_t literalsNumber)
{
    uint64_t size = memMngr.totalSize - literalsNumber * sizeof(struct v_term);
    uint64_t dataHeapSize = DATA_HEAP_SIZE_FACTOR * size;
    uint64_t vtermsHeapSize = V_TERMS_HEAP_SIZE_FACTOR * size;

    memMngr.vtermsBeginOffset = literalsNumber;

	uint8_t* pointer = (uint8_t*)memMngr.vterms;

	allocateMemoryForVTerms(vtermsHeapSize, &pointer);
	allocateMemoryForData(dataHeapSize, &pointer);

    memMngr.vtermsOffset = memMngr.vtActiveOffset;
    memMngr.dataOffset = memMngr.dtActiveOffset;    
}

// Значение n выводится из формулы:
// size = n * (sizeof(struct v_term) + sizeof(uint8_t))
static uint64_t getTermsMaxNumber(uint64_t size)
{
    return size / (sizeof(struct v_term) + sizeof(uint8_t));
}

static void allocateMemoryForVTerms(uint64_t size, uint8_t** pointer)
{
    uint64_t maxTermsNumber = getTermsMaxNumber(size);

    memMngr.vtActiveOffset = memMngr.vtermsBeginOffset;
    memMngr.vtInactiveOffset = memMngr.vtActiveOffset + maxTermsNumber;
    memMngr.gcInUseVTerms = (uint8_t*)(memMngr.vterms + memMngr.vtInactiveOffset + maxTermsNumber);

    memMngr.vtermsMaxOffset = maxTermsNumber;

    *pointer += memMngr.vtermsBeginOffset * sizeof(struct v_term);
	*pointer += 2 * maxTermsNumber * sizeof(struct v_term);
    *pointer += maxTermsNumber * sizeof(uint8_t);
}

static void allocateMemoryForData(uint64_t size, uint8_t** pointer)
{
    uint64_t singleDataHeapSize = size / 2;

    memMngr.data = *pointer;
    memMngr.dtActiveOffset = 0;
    memMngr.dtInactiveOffset = singleDataHeapSize;
	memMngr.dataMaxOffset = singleDataHeapSize;

    *pointer += size;
}
