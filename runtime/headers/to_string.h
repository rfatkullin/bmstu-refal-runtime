#ifndef __TO_STRING_H__
#define __TO_STRING_H__

#include <stdint.h>
#include <gmp.h>

#include <vterm.h>

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
