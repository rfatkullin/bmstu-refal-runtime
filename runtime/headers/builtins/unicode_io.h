#ifndef _BMSRU_REFC_COMPILER_UNICODE_IO_H_
#define _BMSRU_REFC_COMPILER_UNICODE_IO_H_

/// Считывает из файла UTF-8 символ и конвертирует его в  UTF-32.
uint32_t readUTF8Char(FILE* file);

/// Считывает из char* строки UTF-8 символ и конвертирует его в UTF-32.
/// Возвращает указатель на байт за последним считанным байтом.
char* readUTF8CharFromStr(char* str, uint32_t* resPointer);

/// Выводит в файл UTF-32 символ как UTF-8.
void printUTF32 (FILE* file, uint32_t ch);

/// Конвертирует UTF-32 символ в UTf-8 и записывает его в буфер.
/// Возвращает указатель на байт за последним записанным байтом.
char* writeAsUTF8ToBuff(uint32_t ch, char* buff);

#endif
