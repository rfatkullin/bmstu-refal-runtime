#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "memory_manager.h"

static void allocateMemoryForSegmentTree(uint64_t termsNumber, uint8_t** pointer);
static void allocateMemoryForVTerms(uint64_t size, uint8_t** pointer);
static void allocateMemoryForData(uint64_t size, uint8_t** pointer);
static void printMemoryAllocationInfo();
static double bytes2Gb(uint64_t bytesCount);

void initAllocator(uint64_t size)
{
	memMngr.vterms = (struct v_term*)malloc(size);
	memMngr.totalSize = size;
}

void initHeaps(uint64_t literalsNumber)
{    
    int64_t size = memMngr.totalSize - literalsNumber * sizeof(struct v_term);
    uint64_t dataHeapSize = DATA_HEAP_SIZE_FACTOR * size;
    uint64_t vtermsHeapSize = VTERMS_HEAP_SIZE_FACTOR * size;

    memMngr.vtermsBeginOffset = literalsNumber;

	uint8_t* pointer = (uint8_t*)memMngr.vterms;

	allocateMemoryForVTerms(vtermsHeapSize, &pointer);
	allocateMemoryForData(dataHeapSize, &pointer);

    memMngr.vtermsOffset = memMngr.vtActiveOffset;
    memMngr.dataOffset = memMngr.dtActiveOffset;

    //printMemoryAllocationInfo();
}

void printMemoryInfo()
{
    double totalMemoryGb = bytes2Gb(memMngr.totalSize);
    double dataSizeGb = bytes2Gb(memMngr.dataMaxOffset);

    printf("In use %" PRIu64 " vterms from %" PRIu64 ": %f\n",
           memMngr.vtermsOffset - memMngr.vtActiveOffset,
           memMngr.vtermsMaxOffset,
           (double)(memMngr.vtermsOffset - memMngr.vtActiveOffset) / memMngr.vtermsMaxOffset);

    printf("In use %" PRIu64 " data bytes from %" PRIu64 ": %f\n",
           memMngr.dataOffset - memMngr.dtActiveOffset,
           memMngr.dataMaxOffset,
           (double)(memMngr.dataOffset - memMngr.dtActiveOffset) / memMngr.dataMaxOffset);
}

static void printMemoryAllocationInfo()
{
    double totalMemoryGb = bytes2Gb(memMngr.totalSize);
    double dataSizeGb = bytes2Gb(memMngr.dataMaxOffset);

    printf("%-20s%fGB\n", "Allocated: ", totalMemoryGb);
    printf("%-20s%" PRIu64 " terms (x2 = %" PRIu64 " vterms, memory: %f GB)\n", "Max vterms count: ",
           memMngr.vtermsMaxOffset, 2 * memMngr.vtermsMaxOffset, 2 * bytes2Gb(memMngr.vtermsMaxOffset * sizeof(struct v_term)));
    printf("%-20s%" PRIu64 " bytes, %f GB (x2 = %f GB)\n", "Max data size: ", memMngr.dataMaxOffset, dataSizeGb, 2 * dataSizeGb);
}

static double bytes2Gb(uint64_t bytesCount)
{
    double toGb = 1024.0 * 1024.0 * 1024.0;

    return bytesCount / toGb;
}

// Значение n выводится из формулы:
// size = n * (sizeof(struct v_term) + sizeof(uint8_t))
// sizeof(struct v_term) - для vterm'ов
// sizeof(uint8_t) - для gc.
static uint64_t getTermsMaxNumber(uint64_t size)
{
    return size / (sizeof(struct v_term) + sizeof(uint8_t));
}

static void allocateMemoryForVTerms(uint64_t size, uint8_t** pointer)
{
    uint64_t maxTermsNumber = getTermsMaxNumber(size) / 2;

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
