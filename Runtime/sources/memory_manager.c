#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "memory_manager.h"

static float byte2KByte(uint64_t bytes);

static void allocateMemoryForSegmentTree(uint64_t termsNumber, uint8_t** pointer);
static void allocateMemoryForVTerms(uint64_t size, uint8_t** pointer);
static void allocateMemoryForData(uint64_t size, uint8_t** pointer);

void initAllocator(uint64_t size)
{
	memMngr.vterms = (struct v_term*)malloc(size);
	memMngr.totalSize = size;
}

void initHeaps(uint64_t segmentLen, uint64_t literalsNumber)
{
	//TO FIX: Если инициализируем данные, то их тоже нужно тут учитывать.
    uint64_t size = memMngr.totalSize - literalsNumber * sizeof(struct v_term);
    uint64_t dataHeapSize = DATA_HEAP_SIZE_FACTOR * size;
    uint64_t vtermsHeapSize = V_TERMS_HEAP_SIZE_FACTOR * size;

//	printf("\nAllocation size:                      %.2f Kb\n", byte2KByte(size));
//	printf("\nAllocation ratios and sizes:         Ratio\t   Size\n");
//	printf("\t For data:                    %.2f\t %.2f Kb\n", DATA_HEAP_SIZE_FACTOR, byte2KByte(dataHeapSize));
//	printf("\t For vterms:                  %.2f\t %.2f Kb\n", V_TERMS_HEAP_SIZE_FACTOR, byte2KByte(vtermsHeapSize));
//	printf("\t For lterms:                  %.2f\t %.2f Kb\n", 1.0f - (DATA_HEAP_SIZE_FACTOR + V_TERMS_HEAP_SIZE_FACTOR), byte2KByte(ltermsHeapSize));

	memMngr.segmentLen = segmentLen;
	memMngr.literalsNumber = literalsNumber;

	uint8_t* pointer = (uint8_t*)memMngr.vterms;

	allocateMemoryForVTerms(vtermsHeapSize, &pointer);
	allocateMemoryForData(dataHeapSize, &pointer);

    memMngr.vtermsOffset = memMngr.vtActiveOffset;
    memMngr.dataOffset = memMngr.dtActiveOffset;
}

static float byte2KByte(uint64_t bytes)
{
	return bytes / 1024.0f;
}

// Значение n выводится из формулы:
// size = 2 * n * sizeof(struct v_term) + (4 * n / N + n) * sizeof(uint64_t)
// где N - длина отрезка в листьях дерева.
static uint64_t getTermsMaxNumber(uint64_t size)
{
    uint64_t memSizeWithoutHeader = size - sizeof(struct segment_tree);
    uint64_t N = memMngr.segmentLen;
    return (N * memSizeWithoutHeader) / (2 * N * sizeof(struct v_term) + (4 + N) * sizeof(uint64_t));
}

static void allocateMemoryForSegmentTree(uint64_t termsNumber, uint8_t** pointer)
{
    uint64_t chunck = memMngr.segmentLen;
    uint64_t memSizeForTree = 4 * termsNumber / chunck * sizeof(uint64_t);
    uint64_t memSizeForElements = termsNumber * sizeof(uint64_t);

	memMngr.segmentTree = (struct segment_tree*)(*pointer);
	memMngr.segmentTree->tree = (int32_t*)(*pointer + sizeof(struct segment_tree));
    memMngr.segmentTree->elements = (int64_t*)(*pointer + memSizeForTree + sizeof(struct segment_tree));

	*pointer += memSizeForTree + memSizeForElements + sizeof(struct segment_tree);
}

static void allocateMemoryForVTerms(uint64_t size, uint8_t** pointer)
{
    uint64_t maxTermsNumber = getTermsMaxNumber(size);
    memMngr.vtActiveOffset = memMngr.literalsNumber;
    memMngr.vtInactiveOffset = memMngr.vtActiveOffset + maxTermsNumber;
	memMngr.vtermsMaxOffset = maxTermsNumber - 1;
	*pointer += memMngr.literalsNumber * sizeof(struct v_term);
	*pointer += 2 * maxTermsNumber * sizeof(struct v_term);

	allocateMemoryForSegmentTree(maxTermsNumber, pointer);

	//printf("\tMax terms: %d\n", memMngr.maxTermsNumber);
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
