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

mpz_t MaxCharCodePoint;

struct file_info
{
    FILE*   file;
    uint8_t mode;
};

/// Количесто аргументов командной строки.
int _refalProgramArgsCount;

/// Счетчик для встроенной функции Step.
mpz_t _step;

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
uint8_t getDescr(struct fragment_t* frag);
void gcOpenDefaultFile(uint8_t descr, uint8_t mode);

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

/// Возвращает общее количество вызванных функций.
struct func_result_t Step(int entryStatus);

/// Оборачивает свой аргумент в скобки активации.
struct func_result_t Mu(int entryStatus);

/// Возвращает информацию о первом символе.
struct func_result_t Type(int entryStatus);

struct func_result_t Chr(int entryStatus);
struct func_result_t Ord(int entryStatus);

struct func_result_t First(int entryStatus);
struct func_result_t Last(int entryStatus);

struct func_result_t Explode(int entryStatus);
struct func_result_t Implode(int entryStatus);

struct func_result_t Explode_Ext(int entryStatus);
struct func_result_t Implode_Ext(int entryStatus);

/* Вспомогательные функции.*/

/// Обрабатывает аргументы командной строки и заполняет _refalProgramArgs.
uint64_t initArgsData(uint64_t offset, int argc, char** argv);

/// Проверка на равенство двух строк. 1 - успех, 0 - неудача.
int ustrEq(struct vstring_t* a, struct vstring_t* b);

/// Проверка на равенство двух чисел. 1: a > b, -1 a < b, 0 a == b
int intCmp(struct vint_t* a, struct vint_t* b);

/// Проверка на равенство двух чисел. 1: a > b, -1 a < b, 0 a == b
int doubleCmp(double a, double b);

/// Собирает из целочисленнго числа результат функции.
struct lterm_t* gcConstructSingleIntNumBuiltinResult(mpz_t num);

/// Конвертирует mpz_t в int.
int ConvertToInt(struct vint_t* numData);

/// Выполняет подготовительные операции, общие для всех встроенных функций.
void gcInitBuiltinEnv();

/// Инициализация встроенных функций.
void initBuiltins();

#endif
