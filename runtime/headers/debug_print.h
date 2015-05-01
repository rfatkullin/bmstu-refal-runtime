#ifndef _BMSTU_REFC_DEBUG_PRINT_H_
#define _BMSTU_REFC_DEBUG_PRINT_H_

/// Вывод фргамента vterm'ов с переводом строки в файл.
void printFragmentLn(FILE* file, struct fragment_t* frag);

/// Выводит поле зрения в файл.
void printFieldOfView(FILE* file, struct lterm_t* fov);

#endif
