#ifndef __INPUT_H__
#define __INPUT_H__

#include "builtins.h"

/// Read UTF-8 char as UTF32 char
uint32_t readUTF8Char();

/// Print UTF32 char as UTF8 char
void printUTF32 (uint32_t ch);

#define MAX_FILE_DESCR  20
#define READ_MODE       1
#define WRITE_MODE      2

int openedFiles[MAX_FILE_DESCR];

#endif
