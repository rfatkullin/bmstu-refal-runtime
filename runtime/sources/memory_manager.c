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
    _memMngr.vterms = (struct vterm_t*)malloc(size);
    _memMngr.totalSize = size;
}

void initHeaps(uint64_t literalsNumber)
{    
    int64_t size = _memMngr.totalSize - literalsNumber * sizeof(struct vterm_t);
    uint64_t dataHeapSize = DATA_HEAP_SIZE_FACTOR * size;
    uint64_t vtermsHeapSize = VTERMS_HEAP_SIZE_FACTOR * size;

    _memMngr.vtermsBeginOffset = literalsNumber;

    uint8_t* pointer = (uint8_t*)_memMngr.vterms;

	allocateMemoryForVTerms(vtermsHeapSize, &pointer);
	allocateMemoryForData(dataHeapSize, &pointer);

    _memMngr.vtermsOffset = _memMngr.vtActiveOffset;
    _memMngr.dataOffset = _memMngr.dtActiveOffset;

    //printMemoryAllocationInfo();
}

void printMemoryInfo()
{
    double totalMemoryGb = bytes2Gb(_memMngr.totalSize);
    double dataSizeGb = bytes2Gb(_memMngr.dataMaxOffset);

    printf("In use %" PRIu64 " vterms from %" PRIu64 ": %f\n",
           _memMngr.vtermsOffset - _memMngr.vtActiveOffset,
           _memMngr.vtermsMaxOffset,
           (double)(_memMngr.vtermsOffset - _memMngr.vtActiveOffset) / _memMngr.vtermsMaxOffset);

    printf("In use %" PRIu64 " data bytes from %" PRIu64 ": %f\n",
           _memMngr.dataOffset - _memMngr.dtActiveOffset,
           _memMngr.dataMaxOffset,
           (double)(_memMngr.dataOffset - _memMngr.dtActiveOffset) / _memMngr.dataMaxOffset);
}

static void printMemoryAllocationInfo()
{
    double totalMemoryGb = bytes2Gb(_memMngr.totalSize);
    double dataSizeGb = bytes2Gb(_memMngr.dataMaxOffset);

    printf("%-20s%fGB\n", "Allocated: ", totalMemoryGb);
    printf("%-20s%" PRIu64 " terms (x2 = %" PRIu64 " vterms, memory: %f GB)\n", "Max vterms count: ",
           _memMngr.vtermsMaxOffset, 2 * _memMngr.vtermsMaxOffset, 2 * bytes2Gb(_memMngr.vtermsMaxOffset * sizeof(struct vterm_t)));
    printf("%-20s%" PRIu64 " bytes, %f GB (x2 = %f GB)\n", "Max data size: ", _memMngr.dataMaxOffset, dataSizeGb, 2 * dataSizeGb);
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
    return size / (sizeof(struct vterm_t) + sizeof(uint8_t));
}

static void allocateMemoryForVTerms(uint64_t size, uint8_t** pointer)
{
    uint64_t maxTermsNumber = getTermsMaxNumber(size) / 2;

    _memMngr.vtActiveOffset = _memMngr.vtermsBeginOffset;
    _memMngr.vtInactiveOffset = _memMngr.vtActiveOffset + maxTermsNumber;
    _memMngr.gcInUseVTerms = (uint8_t*)(_memMngr.vterms + _memMngr.vtInactiveOffset + maxTermsNumber);

    _memMngr.vtermsMaxOffset = maxTermsNumber;

    *pointer += _memMngr.vtermsBeginOffset * sizeof(struct vterm_t);
	*pointer += 2 * maxTermsNumber * sizeof(struct vterm_t);
    *pointer += maxTermsNumber * sizeof(uint8_t);
}

static void allocateMemoryForData(uint64_t size, uint8_t** pointer)
{
    uint64_t singleDataHeapSize = size / 2;

    _memMngr.data = *pointer;
    _memMngr.dtActiveOffset = 0;
    _memMngr.dtInactiveOffset = singleDataHeapSize;
    _memMngr.dataMaxOffset = singleDataHeapSize;

    *pointer += size;
}
