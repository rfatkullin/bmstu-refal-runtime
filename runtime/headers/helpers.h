#ifndef _BMSTU_REFC_HELPERS_H__
#define _BMSTU_REFC_HELPERS_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <gmp.h>

#include <vterm.h>


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

/// Return need bytes number for store intNum. Without 0 char.
/// If outputNum != 0 mpz_t value of num sets to it.
uint64_t calcBytesForIntCharArr(struct vint_t* intNum, mpz_t* outputNum);

/// Return need bytes number for store str. Without 0 char.
uint64_t calcBytesForVStringCharArr(struct vstring_t* str);

/// v_int struct converts to char array and store in buff. Returns pointer to the next byte after last written byte.
char* vIntToCharArr(struct vint_t* intNum, char* buff);

/// v_string struct converts to char array and store in buff. Returns pointer to the next byte after last written byte.
char* vStringToCharArr(struct vstring_t* str, char* buff);

#endif
