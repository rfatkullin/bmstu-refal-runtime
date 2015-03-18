#ifndef __GC_H__
#define __GC_H__

#include <memory_manager.h>
#include <helpers.h>

typedef int  allocate_result;

#define OK          0
#define NEED_CLEAN  1

#define MEMORY_OVERFLOW_MSG     "Memory overflow!\n"
#define CANT_COPY_TERM          "GC Can't copy term!\n"

#define GC_VTERM_HEAP_CHECK_RETURN(needCount, statusVar)    \
do{                                                         \
    if (memMngr.vtermsOffset + needCount >                  \
        memMngr.vtermsMaxOffset)                            \
    {                                                       \
        statusVar = NEED_CLEAN;                             \
        return 0;                                           \
    }                                                       \
    statusVar = OK;                                         \
}while(0)

#define GC_DATA_HEAP_CHECK_RETURN(needDataSize, statusVar)  \
do{                                                         \
    if (memMngr.dataOffset + needDataSize >                 \
        memMngr.dataMaxOffset)                              \
    {                                                       \
        statusVar = NEED_CLEAN;                             \
        return 0;                                           \
    }                                                       \
    statusVar = OK;                                         \
}while(0)

#define CHECK_ALLOCATION_RETURN(var, expr, statusVar)   \
do{                                                     \
    var = expr;                                         \
    if (statusVar == NEED_CLEAN)                        \
        return 0;                                       \
}while(0)

#define CHECK_ALLOCATION_CONTINUE(var, expr, statusVar) \
do{                                                     \
    var = expr;                                         \
    if (statusVar == NEED_CLEAN)                        \
    {                                                   \
        statusVar = 0;                                  \
        continue;                                       \
    }                                                   \
}while(0)

#define GC_VTERM_HEAP_CHECK_EXIT(needCount)     \
do{                                             \
    if (memMngr.vtermsOffset + needCount >      \
        memMngr.vtermsMaxOffset)                \
        PRINT_AND_EXIT(MEMORY_OVERFLOW_MSG);    \
}while(0)

#define GC_DATA_HEAP_CHECK_EXIT(needDataSize)   \
do{                                             \
    if (memMngr.dataOffset + needDataSize >     \
        memMngr.dataMaxOffset)                  \
        PRINT_AND_EXIT(MEMORY_OVERFLOW_MSG);    \
}while(0)

/// Собирает мусор.
void collectGarbage();

struct lterm_t* copySimpleChain(struct lterm_t* chain);
void collectVTermGarbage(struct lterm_t* fieldOfView);

struct lterm_t* copyFragmentLTerm(struct lterm_t* term);

void strictCheckHeaps(uint64_t needTermCount, uint64_t needDataSize);
int isHeapsOverflowed(uint64_t needTermCount, uint64_t needDataSize);
int checkAndCleanHeaps(uint64_t needTermCount, uint64_t needDataSize);

#endif
