#ifndef __TO_STRING_H__
#define __TO_STRING_H__

#include <stdint.h>

#include <vterm.h>

uint64_t calcBytesForIntCharArr(struct v_int* intNumber);
uint64_t calcBytesForVStringCharArr(struct v_string* str);

char* vIntToCharArr(struct v_int* intNumStruct, char* buff);
char* vStringToCharArr(struct v_string* str, char* buff);

#endif
