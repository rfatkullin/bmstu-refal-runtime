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

/// Парсит аргументы командной строки. Если присутствует
/// параметр -heapSize=size_in_bytes, возвращает size_in_bytes.
/// В противном случае вернет DEFAULT_HEAP_SIZE, определенный
/// в файле memory_manager.h
uint64_t getHeapSizeFromCmdArgs(int argc, char** argv);

/// Сравнивает два vterm'а со смещениями a и b.
int eqSymbol(uint64_t a, uint64_t b);

/// Сравнивает два фрагмента vterm'ов длиной length.
/// Начиная со смещений a и b.
int eqFragment(uint64_t a, uint64_t b, uint64_t length);

#endif
