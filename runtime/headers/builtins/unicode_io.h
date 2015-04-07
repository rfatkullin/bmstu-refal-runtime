#ifndef _BMSRU_REFAL_COMPILER_UNICODE_IO_H_
#define _BMSRU_REFAL_COMPILER_UNICODE_IO_H_

/// Read UTF-8 char as UTF32 char from file.
uint32_t readUTF8Char(FILE* file);

/// Read UTF-8 char as UTF32 char from char array.
char* readUTF8CharFromStr(char* str, uint32_t* resPointer);

/// Print UTF32 char as UTF8 char
void printUTF32 (FILE* file, uint32_t ch);

/// Convert UTF-32 char to UTf-8 char and write to buff.
char* writeAsUTF8ToBuff(uint32_t ch, char* buff);

#endif
