#ifndef __HELPERS_H__
#define __HELPERS_H__

#include <stdio.h>
#include <stdlib.h>

#define FMT_PRINT_AND_EXIT(fmt, ...)    \
do{                                     \
    printf(fmt, __VA_ARGS__);           \
    exit(0);                            \
}while(0)

#define PRINT_AND_EXIT(str)             \
do{                                     \
    printf(str);                        \
    exit(0);                            \
}while(0)

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

#endif
