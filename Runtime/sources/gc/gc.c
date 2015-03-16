#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#include <memory_manager.h>
#include <allocators/data_alloc.h>
#include <builtins/builtins.h>

void collectGarbage()
{
    clock_t start, end;
    printf("Start garbage collection.\n");
    start = clock();

    collectVTermGarbage(memMngr.fieldOfView);

    copyChainLTerm(memMngr.fieldOfView);
    assembledFragInBuiltins = copyFragmentLTerm(assembledFragInBuiltins);

    end = clock();

    printf("End garbage collection. Time elapsed: %f\n", ((float)(end - start)) / CLOCKS_PER_SEC);
}

void failWithMemoryOverflow()
{
    printf("Failed with memory overflow!");
    exit(1);
}

void strictCheckHeaps(uint64_t needTermCount, uint64_t needDataSize)
{
    if (memMngr.vtermsOffset + needTermCount > memMngr.vtermsMaxOffset ||
        memMngr.dataOffset + needDataSize > memMngr.dataMaxOffset)
    {
        failWithMemoryOverflow();
    }
}

int isHeapsOverflowed(uint64_t needTermCount, uint64_t needDataSize)
{
    if (memMngr.vtermsOffset + needTermCount > memMngr.vtermsMaxOffset ||
        memMngr.dataOffset + needDataSize > memMngr.dataMaxOffset)
        return 1;

    return 0;
}

int checkAndCleanHeaps(uint64_t needTermCount, uint64_t needDataSize)
{
    int isCollect = 0;

    if (memMngr.vtermsOffset + needTermCount > memMngr.vtermsMaxOffset ||
        memMngr.dataOffset + needDataSize > memMngr.dataMaxOffset)
    {
        collectGarbage();
        isCollect = 1;
    }

    if (memMngr.vtermsOffset + needTermCount > memMngr.vtermsMaxOffset ||
        memMngr.dataOffset + needDataSize > memMngr.dataMaxOffset)
    {
        failWithMemoryOverflow();
    }

    return isCollect;
}






