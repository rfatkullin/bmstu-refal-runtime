#ifndef __HELPERS_H__
#define __HELPERS_H__

#include <stdio.h>

#define FMT_PRINT_AND_EXIT(fmt, ...)    \
do{                                     \
    printf(fmt, __VA_ARGS__);           \
}while(0)

#define PRINT_AND_EXIT(str)             \
do{                                     \
    printf(str);                        \
}while(0)


#endif
