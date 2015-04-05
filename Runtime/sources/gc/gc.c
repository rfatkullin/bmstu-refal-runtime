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

    collectVTermGarbage(_memMngr.fieldOfView);

    _memMngr.fieldOfView = copySimpleChain(_memMngr.fieldOfView);

    // Get node from new heap
    SET_ACTUAL(_currCallTerm);

    // Fix pointers in func call chain
    struct func_call_t* funcCall = _currCallTerm->funcCall;
    while (funcCall->next)
    {
        SET_ACTUAL(funcCall->next);
        funcCall = funcCall->next->funcCall;
    }

    // To catch bugs
    // Can't set to zero data heap, because target program get net lterms through old lterms (GET_ACTUAL)
    //memset(memMngr.vterms + memMngr.vtInactiveOffset, 0, memMngr.vtermsMaxOffset * sizeof(struct vterm_t));

    printf("End garbage collection:\n");
    printMemoryInfo();
    fflush(stdout);
}

void strictCheckHeaps(uint64_t needTermCount, uint64_t needDataSize)
{
    if (GC_VTERM_OV(needTermCount) || GC_LTERM_OV(needDataSize))
        PRINT_AND_EXIT(GC_MEMORY_OVERFLOW_MSG);
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
        PRINT_AND_EXIT(GC_MEMORY_OVERFLOW_MSG);

    return isCollect;
}






