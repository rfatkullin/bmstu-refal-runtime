#ifndef __GC_H__
#define __GC_H__

#include <memory_manager.h>
#include <helpers.h>

typedef int  allocate_result;

#define GC_OK          0
#define GC_NEED_CLEAN  1

#define GC_MEMORY_OVERFLOW_MSG          "[GC]: Memory overflow!\n"
#define GC_CANT_COPY_TERM               "[GC]: Can't copy term!\n"
#define GC_VTERM_PROCESS_BAD_CHAIN_TAG  "[GC]: Bad lterm chain tag at GC vterm process. Chains can't contains simple chain without chain keeper!\n"
#define GC_VTERM_PROCESS_BAD_TAG        "[GC]: Bad lterm tag at GC vterm process!\n"
#define GC_BAD_CHAIN_SIMPLE_CHAIN_COPY  "[GC]: Bad chain term passed to copy simple chain func!\n"
#define GC_NULL_CHAIN_SIMPLE_CHAIN_COPY "[GC]: Null chain passed to copy simple chain func!\n"
#define GC_PARENT_CALL_NOT_MOVED        "[GC]: Parent call must be moved at subcall copy!\n"

#define GC_VTERM_OV(needCount)      (memMngr.vtermsOffset + needCount > memMngr.vtActiveOffset + memMngr.vtermsMaxOffset)
#define GC_LTERM_OV(needDataSize)   (memMngr.dataOffset + needDataSize > memMngr.dtActiveOffset + memMngr.dataMaxOffset)

#define GC_VTERM_HEAP_CHECK_RETURN(needCount, statusVar)    \
do{                                                         \
    if (GC_VTERM_OV(needCount))                             \
    {                                                       \
        statusVar = GC_NEED_CLEAN;                          \
        return 0;                                           \
    }                                                       \
    statusVar = GC_OK;                                      \
}while(0)

#define GC_DATA_HEAP_CHECK_RETURN(needDataSize, statusVar)  \
do{                                                         \
    if (GC_LTERM_OV(needDataSize))                          \
    {                                                       \
        statusVar = GC_NEED_CLEAN;                          \
        return 0;                                           \
    }                                                       \
    statusVar = GC_OK;                                      \
}while(0)

#define CHECK_ALLOCATION_RETURN(var, expr, statusVar)   \
do{                                                     \
    var = expr;                                         \
    if (statusVar == GC_NEED_CLEAN)                     \
        return 0;                                       \
}while(0)

#define CHECK_ALLOCATION_CONTINUE(var, expr, statusVar) \
do{                                                     \
    var = expr;                                         \
    if (statusVar == GC_NEED_CLEAN)                     \
    {                                                   \
        statusVar = 0;                                  \
        continue;                                       \
    }                                                   \
}while(0)

#define GC_VTERM_HEAP_CHECK_EXIT(needCount)                 \
do{                                                         \
    if (GC_VTERM_OV(needCount))                             \
    PRINT_AND_EXIT(GC_MEMORY_OVERFLOW_MSG);                 \
}while(0)

#define GC_DATA_HEAP_CHECK_EXIT(needDataSize)               \
do{                                                         \
    if (GC_LTERM_OV(needDataSize))                          \
        PRINT_AND_EXIT(GC_MEMORY_OVERFLOW_MSG);             \
}while(0)


#define GC_RETURN_ON_FAIL(res)                              \
do{                                                         \
    if (res == GC_NEED_CLEAN)                               \
        return res;                                         \
}while(0)                                                   \

/// Собирает мусор.
void collectGarbage();

struct lterm_t* copySimpleChain(struct lterm_t* chain);
void collectVTermGarbage(struct lterm_t* fieldOfView);

struct lterm_t* copyFragmentLTerm(struct lterm_t* term);

void strictCheckHeaps(uint64_t needTermCount, uint64_t needDataSize);
int isHeapsOverflowed(uint64_t needTermCount, uint64_t needDataSize);
int checkAndCleanHeaps(uint64_t needTermCount, uint64_t needDataSize);

#endif
