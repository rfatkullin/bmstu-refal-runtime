#ifndef __BUILTINS_H__
#define __BUILTINS_H__

#include <gmp.h>

#include "lterm.h"
#include "memory_manager.h"
#include "func_call.h"

#define MAX_FILE_DESCR              20
#define READ_MODE_STR               "r"
#define WRITE_MODE_STR              "w"
#define READ_MODE                   1
#define WRITE_MODE                  2
#define FILE_NAME_PATTERN           "REFAL%u.DAT"
#define PATTERN_FILE_NAME_LENGHT    12

#define TOO_FEW_ARGUMENTS           "Too few arguments to function!\n"
#define BAD_ARGUMENTS               "Bad file descriptor! Expected 0 <= descr < %d\n"
#define BAD_FILE_OPEN_MODE          "Bad file open mode! Expected 'W','w', 'R' or 'r'\n"
#define FILE_OPEN_ERROR             "Can't open file %s for %s!"
#define DESCR_ALREADY_IN_USE        "Descriptor %u already in use!"
#define BAD_VTERM                   "Bad vterm!"

struct file_info
{
    FILE*   file;
    uint8_t mode;
};

/// Информация об открытых файлах.
struct file_info files[MAX_FILE_DESCR];

struct func_result_t Prout(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView, int firstCall);
struct func_result_t Card(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView, int firstCall);

struct func_result_t Open(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView, int firstCall);
struct func_result_t Get(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView, int firstCall);
struct func_result_t Put(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView, int firstCall);

struct func_result_t Add(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView, int firstCall);
struct func_result_t Sub(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView, int firstCall);
struct func_result_t Mul(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView, int firstCall);
struct func_result_t Div(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView, int firstCall);
struct func_result_t Mod(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView, int firstCall);

/// Конвертирует в обычный uint8_t. В случае успеха вернет 1, 0 в противном случае.
int convertToFileDescr(struct v_int* bigInt, uint8_t* descr);

/// Проверка на равенство двух строк. 1 - успех, 0 - неудача.
int ustrEq(struct v_string* a, struct v_string* b);

/// Вывод Unicode строки.
void printUStr(struct v_string* str);

/// Проверка на равенство двух чисел. 1: a > b, -1 a < b, 0 a == b
int intCmp(struct v_int* a, struct v_int* b);

/// Проверка на равенство двух чисел. 1: a > b, -1 a < b, 0 a == b
int doubleCmp(double a, double b);

#endif