#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>

#include <gc/gc.h>
#include <vmachine.h>
#include <memory_manager.h>
#include <builtins/builtins.h>
#include <defines/gc_macros.h>
#include <defines/errors_str.h>
#include <allocators/data_alloc.h>

static struct lterm_t* getActual(struct lterm_t* term);

void collectGarbage()
{
#ifdef DEBUG
    printf("Start garbage collection.\n");    
#endif

    collectVTermGarbage(_memMngr.fieldOfView);

    _memMngr.fieldOfView = copySimpleChain(_memMngr.fieldOfView);

    // Get node from new heap
    _currCallTerm = getActual(_currCallTerm);

    // Fix pointers in func call chain
    struct func_call_t* funcCall = _currCallTerm->funcCall;
    while (funcCall->next)
    {        
        funcCall->next = getActual(funcCall->next);
        funcCall = funcCall->next->funcCall;
    }

    memset(_memMngr.vterms + _memMngr.vtInactiveOffset, 0, _memMngr.vtermsMaxOffset * sizeof(struct vterm_t));
    memset(_memMngr.data + _memMngr.dtInactiveOffset, 0, _memMngr.dataMaxOffset * sizeof(uint8_t));

#ifdef DEBUG
    printf("End garbage collection.\n");
#endif
}

int checkAndCleanHeaps(uint64_t needTermCount, uint64_t needDataSize)
{
    int isCollect = 0;

    if (GC_VTERM_OV(needTermCount) || GC_DATA_OV(needDataSize))
    {
        collectGarbage();
        isCollect = 1;
    }

    if (GC_VTERM_OV(needTermCount) || GC_DATA_OV(needDataSize))
        PRINT_AND_EXIT(GC_MEMORY_OVERFLOW_MSG);

    return isCollect;
}

static struct lterm_t* getActual(struct lterm_t* term)
{
    if (term->tag == GC_MOVED)
        return term->prev;

    return term;
}



