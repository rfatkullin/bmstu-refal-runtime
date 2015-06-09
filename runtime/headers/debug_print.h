#ifndef _BMSTU_REFC_DEBUG_PRINT_H_
#define _BMSTU_REFC_DEBUG_PRINT_H_

#include <stdio.h>

/// Вывод фргамента vterm'ов с переводом строки в файл.
void printFragmentLn(FILE* file, struct fragment_t* frag);

/// Выводит поле зрения в файл.
void printFieldOfView(FILE* file, struct lterm_t* fov);

/// Выводит вызов функции.
void printFuncCallLn(FILE* file, struct func_call_t* funcCall);

void printUStr(FILE* file, struct vstring_t* str);

#endif
