#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#include <memory_manager.h>
#include <allocators/data_alloc.h>

void collectGarbage()
{
    clock_t start, end;
    printf("Start garbage collection.\n");
    start = clock();

    collectVTermGarbage(memMngr.fieldOfView);

    copyChainLTerm(memMngr.fieldOfView);

    end = clock();

    printf("End garbage collection. Time elapsed: %f\n", ((float)(end - start)) / CLOCKS_PER_SEC);
}

void failWithMemoryOverflow()
{
    printf("Failed with memory overflow!");
    exit(1);
}

void strictCheckVTermsOverflow(uint64_t needVTermsCount)
{
    if (memMngr.vtermsOffset + needVTermsCount > memMngr.vtermsMaxOffset)
        failWithMemoryOverflow();
}

int checkAndCleanVTerms(uint64_t needVTermsCount)
{
    int collected = 0;

    if (memMngr.vtermsOffset + needVTermsCount > memMngr.vtermsMaxOffset)
    {
        collectGarbage();
        collected = 1;
    }

    if (memMngr.vtermsOffset + needVTermsCount > memMngr.vtermsMaxOffset)
        failWithMemoryOverflow();

    return collected;
}

int checkAndCleanData(uint64_t needDataSize)
{
    int collected = 0;

    if (memMngr.dataOffset + needDataSize > memMngr.dataMaxOffset)
    {
        collectGarbage();
        collected = 1;
    }

    if (memMngr.dataOffset + needDataSize > memMngr.dataMaxOffset)
        failWithMemoryOverflow();

    return collected;
}


int checkVTermsOverflow(uint64_t needVTermsCount)
{
    return memMngr.vtermsOffset + needVTermsCount > memMngr.vtermsMaxOffset;
}

int checkDataOverflow(uint64_t needDataSize)
{
    return memMngr.dataOffset + needDataSize > memMngr.dataMaxOffset;
}

void checkAndCleanTermsAndData(uint64_t needTermCount, uint64_t needDataSize)
{
    if (memMngr.vtermsOffset + needTermCount > memMngr.vtermsMaxOffset ||
        memMngr.dataOffset + needDataSize > memMngr.dataMaxOffset)
    {
        collectGarbage();
    }

    if (memMngr.vtermsOffset + needTermCount > memMngr.vtermsMaxOffset ||
        memMngr.dataOffset + needDataSize > memMngr.dataMaxOffset)
    {
        failWithMemoryOverflow();
    }
}






