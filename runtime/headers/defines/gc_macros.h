#ifndef _BMSTU_REFC_MACROS_GC_H_
#define _BMSTU_REFC_MACROS_GC_H_

#include <memory_manager.h>

/* Макросы, используемые сборщиком мусора для проверки переполнения куч. */

#define GC_VTERM_OV(needCount)      (_memMngr.vtermsOffset + needCount > _memMngr.vtActiveOffset + _memMngr.vtermsMaxOffset)
#define GC_DATA_OV(needDataSize)   (_memMngr.dataOffset + needDataSize > _memMngr.dtActiveOffset + _memMngr.dataMaxOffset)

#define GC_VTERM_HEAP_CHECK_RETURN(needCount, statusVar)    \
do{                                                         \
    if (GC_VTERM_OV(needCount))                             \
    {                                                       \
        statusVar = GC_NEED_CLEAN;                          \
        return 0;                                           \
    }                                                       \
    statusVar = GC_OK;                                      \
}while(0)

#define GC_VTERM_HEAP_CHECK_EXIT(needCount)                 \
do{                                                         \
    if (GC_VTERM_OV(needCount))                             \
    PRINT_AND_EXIT(GC_MEMORY_OVERFLOW_MSG);                 \
}while(0)


#define GC_DATA_HEAP_CHECK_RETURN(needDataSize, statusVar)  \
do{                                                         \
    if (GC_DATA_OV(needDataSize))                           \
    {                                                       \
        statusVar = GC_NEED_CLEAN;                          \
        return 0;                                           \
    }                                                       \
    statusVar = GC_OK;                                      \
}while(0)

#define GC_DATA_HEAP_CHECK_EXIT(needDataSize)               \
do{                                                         \
    if (GC_DATA_OV(needDataSize))                           \
        PRINT_AND_EXIT(GC_MEMORY_OVERFLOW_MSG);             \
}while(0)

#define CHECK_ALLOCATION_EXIT(var, expr, statusVar)     \
do{                                                     \
    var = expr;                                         \
    if (statusVar == GC_NEED_CLEAN)                     \
        PRINT_AND_EXIT(GC_MEMORY_OVERFLOW_MSG);         \
}while(0)

#define CHECK_ALLOCATION_RETURN(var, expr, statusVar)   \
do{                                                     \
    var = expr;                                         \
    if (statusVar == GC_NEED_CLEAN)                     \
        return 0;                                       \
}while(0)

/// Используем continue --> нельзя использовать do-while.
#define CHECK_ALLOCATION_CONTINUE(var, expr, statusVar) \
{                                                       \
    var = expr;                                         \
    if (statusVar == GC_NEED_CLEAN)                     \
        continue;                                       \
}

#define GC_RETURN_ON_NEED_CLEAN(res)                        \
do{                                                         \
    if (res == GC_NEED_CLEAN)                               \
        return res;                                         \
}while(0)                                                   \

#define DOUBLE_TRY(var, expr, statusVar)            \
do{                                                 \
    var = expr;                                     \
    if (statusVar == GC_NEED_CLEAN)                 \
    {                                               \
        collectGarbage();                           \
        var = expr;                                 \
        if (statusVar == GC_NEED_CLEAN)             \
            PRINT_AND_EXIT(GC_MEMORY_OVERFLOW_MSG); \
    }                                               \
}while(0)

#define CHECK_ON_ZERO_VTERM_OFFSET_EXIT(vtermOffset)    \
do{                                                     \
    if (vtermOffset == 0)                               \
        PRINT_AND_EXIT(GC_ACCESS_TO_ZERO_ADDRESS);      \
}while(0)

#define SET_MST_SIGN_BIT(val)   (val |= GC_MOVED_MASK)
#define CHECK_MST_SIGN_BIT(val) (val & GC_MOVED_MASK)

#define ADDR_IN_INACTIVE_HEAP(addr)                                             \
    (_memMngr.data + _memMngr.dtInactiveOffset <= addr &&                       \
     addr < _memMngr.data + _memMngr.dtInactiveOffset + _memMngr.dataMaxOffset)

#define ADDR_IN_ACTIVE_HEAP(addr)                                             \
    (_memMngr.data + _memMngr.dtActiveOffset <= addr &&                       \
     addr < _memMngr.data + _memMngr.dtActiveOffset + _memMngr.dataMaxOffset)

#endif
