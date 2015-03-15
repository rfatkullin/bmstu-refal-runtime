#ifndef __BUILTINS_H__
#define __BUILTINS_H__

#include <gmp.h>

#include "lterm.h"
#include "memory_manager.h"
#include "func_call.h"

#define MAX_FILE_DESCR              20
#define READ_MODE                   0
#define WRITE_MODE                  1
#define FILE_NAME_PATTERN           "REFAL%u.DAT"
#define PATTERN_FILE_NAME_LENGHT    12

#define TOO_FEW_ARGUMENTS           "Too few arguments to function!\n"
#define BAD_DESCR                   "Bad file descriptor! Descriptor must be in range [1,%d)!\n"
#define BAD_FILE_OPEN_MODE          "Bad file open mode! Expected 'W','w', 'R' or 'r'\n"
#define FILE_OPEN_ERROR             "Can't open file %s in '%s' mode! The reason: %s\n"
#define DESCR_ALREADY_IN_USE        "Descriptor %u already in use!\n"
#define BAD_VTERM                   "Bad vterm type!\n"
#define TRY_TO_TAKE_TERMINAL_DESCR  "Trying to bind file to terminal descriptor! Descriptor must be in range [1,%d)!\n"
#define GET_WRONG_ARG_NUM           "Get take 1 argument - file descriptor!\'n"
#define ARG_WRONG_ARG_NUM           "Arg take 1 argument - argument number!\'n"
#define BAD_PROGRAM_ARG_NUM         "Bad program argument number! Program args count: %d. Passed arg num: %d. But must in range [0, %d).\n"

static const char* modeStr[2] = {"r", "w"};

struct file_info
{
    FILE*   file;
    uint8_t mode;
};

int refalProgramArgsCount;
struct fragment_t* refalProgramArgs;

/// Opend files info.
struct file_info files[MAX_FILE_DESCR];

/// Input/Output
struct func_result_t Prout(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView, int firstCall);
struct func_result_t Card(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView, int firstCall);
struct func_result_t Print(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView, int firstCall);

struct func_result_t Open(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView, int firstCall);

struct func_result_t Get(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView, int firstCall);
struct func_result_t Put(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView, int firstCall);
struct func_result_t Putout(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView, int firstCall);

/// Arithmetics.
struct func_result_t Add(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView, int firstCall);
struct func_result_t Sub(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView, int firstCall);
struct func_result_t Mul(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView, int firstCall);
struct func_result_t Div(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView, int firstCall);
struct func_result_t Mod(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView, int firstCall);

/// Get int from mpz_t.
int ConvertToInt(struct v_int* numData);

/// Others
uint64_t initArgsData(uint64_t offset, int argc, char** argv);
struct func_result_t Arg(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView, int firstCall);

/// Проверка на равенство двух строк. 1 - успех, 0 - неудача.
int ustrEq(struct v_string* a, struct v_string* b);

/// Вывод Unicode строки.
void printUStr(FILE* file, struct v_string* str);

/// Проверка на равенство двух чисел. 1: a > b, -1 a < b, 0 a == b
int intCmp(struct v_int* a, struct v_int* b);

/// Проверка на равенство двух чисел. 1: a > b, -1 a < b, 0 a == b
int doubleCmp(double a, double b);

#endif
