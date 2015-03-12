#ifndef _BMSRU_REFAL_COMPILER_UNICODE_IO_H_
#define _BMSRU_REFAL_COMPILER_UNICODE_IO_H_

/// Read UTF-8 char as UTF32 char
uint32_t readUTF8Char();

/// Print UTF32 char as UTF8 char
void printUTF32 (uint32_t ch);

#endif
