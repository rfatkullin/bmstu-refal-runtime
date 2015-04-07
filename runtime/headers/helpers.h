#ifndef _BMSTU_REFC_HELPERS_H__
#define _BMSTU_REFC_HELPERS_H__

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

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

uint64_t getHeapSize(int argc, char** argv);

int eqSymbol(uint64_t a, uint64_t b);
int eqFragment(uint64_t a, uint64_t b, uint64_t length);

#endif
