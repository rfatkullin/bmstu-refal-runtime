#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#include <memory_manager.h>
#include <allocators/data_alloc.h>
#include <builtins/builtins.h>
#include <gc/gc.h>

void collectGarbage()
{
    printf("Start garbage collection.\n");
    printMemoryInfo();

    printFieldOfView(stdout, memMngr.fieldOfView);

    collectVTermGarbage(memMngr.fieldOfView);

    //printFieldOfView(stdout, memMngr.fieldOfView);

    memMngr.fieldOfView = copySimpleChain(memMngr.fieldOfView);

    printFieldOfView(stdout, memMngr.fieldOfView);

    printf("End garbage collection.\n");
    printMemoryInfo();

    _currCallTerm = _currCallTerm->prev;

    struct func_call_t* funcCall = _currCallTerm->funcCall;

    while (funcCall->next)
    {
        funcCall->next = funcCall->next->prev;
        funcCall = funcCall->next->funcCall;
    }
}

void failWithMemoryOverflow()
{
    printf("Failed with memory overflow!");
    exit(1);
}

void strictCheckHeaps(uint64_t needTermCount, uint64_t needDataSize)
{
    if (GC_VTERM_OV(needTermCount) || GC_LTERM_OV(needDataSize))
    {
        failWithMemoryOverflow();
    }
}

int isHeapsOverflowed(uint64_t needTermCount, uint64_t needDataSize)
{
    if (GC_VTERM_OV(needTermCount) || GC_LTERM_OV(needDataSize))
        return 1;

    return 0;
}

int checkAndCleanHeaps(uint64_t needTermCount, uint64_t needDataSize)
{
    int isCollect = 0;

    if (GC_VTERM_OV(needTermCount) || GC_LTERM_OV(needDataSize))
    {
        collectGarbage();
        isCollect = 1;
    }

    if (GC_VTERM_OV(needTermCount) || GC_LTERM_OV(needDataSize))
    {
        failWithMemoryOverflow();
    }

    return isCollect;
}






