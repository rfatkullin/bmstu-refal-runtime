#ifndef _BMSTU_REFC_BUILTINS_H_
#define _BMSTU_REFC_BUILTINS_H_

#include <gmp.h>

#include <lterm.h>
#include <memory_manager.h>
#include <func_call.h>

#define MAX_FILE_DESCR              20
#define READ_MODE                   0
#define WRITE_MODE                  1
#define FILE_NAME_PATTERN           "REFAL%u.DAT"
#define PATTERN_FILE_NAME_LENGHT    12

#define BUILTIN_FRAG   (VTERM_BRACKETS(CURR_FUNC_CALL->env->assembled[0]))

static const char* modeStr[2] = {"r", "w"};

struct file_info
{
    FILE*   file;
    uint8_t mode;
};

/// Количесто аргументов командной строки.
int _refalProgramArgsCount;

/// Аргументы командной строки. Представлены в виде фрагментов vterm'ов.
struct fragment_t* _refalProgramArgs;

/// Информация по файловым дескрипторам.
struct file_info _files[MAX_FILE_DESCR];

/* Встроенные функции.*/

/// Функции ввода/вывода
struct func_result_t Prout(int entryStatus);
struct func_result_t Card(int entryStatus);
struct func_result_t Print(int entryStatus);

struct func_result_t Open(int entryStatus);

struct func_result_t Get(int entryStatus);
struct func_result_t Put(int entryStatus);
struct func_result_t Putout(int entryStatus);

/// Арифметические функции.
struct func_result_t Add(int entryStatus);
struct func_result_t Sub(int entryStatus);
struct func_result_t Mul(int entryStatus);
struct func_result_t Div(int entryStatus);
struct func_result_t Mod(int entryStatus);
struct func_result_t Compare(int entryStatus);

/// Функции для смены регистра символа.
struct func_result_t Upper(int entryStatus);
struct func_result_t Lower(int entryStatus);

/// Возвращает аргумент командной строки по номеру.
struct func_result_t Arg(int entryStatus);

/// Возвращает целочисленное число, представленное в виде строки.
struct func_result_t Symb(int entryStatus);

/// Переводит целочисленное число в строку.
struct func_result_t Numb(int entryStatus);

/// Возвращает длину в термах переданного аргумента.
struct func_result_t Lenw(int entryStatus);

/* Вспомогательные функции.*/

/// Обрабатывает аргументы командной строки и заполняет _refalProgramArgs.
uint64_t initArgsData(uint64_t offset, int argc, char** argv);

/// Проверка на равенство двух строк. 1 - успех, 0 - неудача.
int ustrEq(struct vstring_t* a, struct vstring_t* b);

/// Вывод Unicode строки.
void printUStr(FILE* file, struct vstring_t* str);

/// Вывод целого числа.
void printIntNumber(FILE* file, struct vint_t* intNum);

/// Проверка на равенство двух чисел. 1: a > b, -1 a < b, 0 a == b
int intCmp(struct vint_t* a, struct vint_t* b);

/// Проверка на равенство двух чисел. 1: a > b, -1 a < b, 0 a == b
int doubleCmp(double a, double b);

/// Вывод фргамента vterm'ов в файл.
void printFragment(FILE* file, struct fragment_t* frag);

/// Вывод фргамента vterm'ов с переводом строки в файл.
void printFragmentLn(FILE* file, struct fragment_t* frag);

/// Собирает из целочисленнго числа результат функции.
struct lterm_t* gcConstructIntNumBuiltinResult(mpz_t num);

/// Выводит поле зрения в файл.
void printFieldOfView(FILE* file, struct lterm_t* fov);

/// Конвертирует mpz_t в int.
int ConvertToInt(struct vint_t* numData);

/// Выполняет подготовительные операции, общие для всех встроенных функций.
void gcInitBuiltin();


#endif
