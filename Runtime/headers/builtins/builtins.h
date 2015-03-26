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
#define BAD_DESCR                   "Bad file descriptor! Descriptor must be in range [1,%d)! Descr: %d\n"
#define BAD_FILE_OPEN_MODE          "Bad file open mode! Expected 'W','w', 'R' or 'r'\n"
#define FILE_OPEN_ERROR             "Can't open file %s in '%s' mode! The reason: %s\n"
#define DESCR_ALREADY_IN_USE        "Descriptor %u already in use!\n"
#define BAD_VTERM                   "Bad vterm type!\n"
#define TRY_TO_TAKE_TERMINAL_DESCR  "Trying to bind file to terminal descriptor! Descriptor must be in range [1,%d)!\n"
#define GET_WRONG_ARG_NUM           "Get take 1 argument - file descriptor!\n"
#define ARG_FUNC_BAD_ARG_NUM        "Arg take 1 argument - number of argument!\'n"
#define BAD_PROGRAM_ARG_NUM         "Bad program argument number! Program args count: %d. Passed arg num: %d. But must in range [1, %d).\n"
#define SYMB_BAD_ARG                "Symb takes 1 argument - int number!\n"
#define NUMB_BAD_ARG                "Numb can't parse int from string!\n"

#define BUILTIN_FRAG   (CURR_FUNC_CALL->env->assembledFOVs[0]->fragment)

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
struct func_result_t Prout(int entryStatus);
struct func_result_t Card(int entryStatus);
struct func_result_t Print(int entryStatus);

struct func_result_t Open(int entryStatus);

struct func_result_t Get(int entryStatus);
struct func_result_t Put(int entryStatus);
struct func_result_t Putout(int entryStatus);

/// Arithmetics.
struct func_result_t Add(int entryStatus);
struct func_result_t Sub(int entryStatus);
struct func_result_t Mul(int entryStatus);
struct func_result_t Div(int entryStatus);
struct func_result_t Mod(int entryStatus);
struct func_result_t Compare(int entryStatus);

/// Case switching
struct func_result_t Upper(int entryStatus);
struct func_result_t Lower(int entryStatus);

/// Others
uint64_t initArgsData(uint64_t offset, int argc, char** argv);
struct func_result_t Arg(int entryStatus);

struct func_result_t Symb(int entryStatus);
struct func_result_t Numb(int entryStatus);

struct func_result_t Lenw(int entryStatus);

void printFragment(FILE* file, struct fragment_t* frag);

struct lterm_t* gcConstructIntNumBuiltinResult(mpz_t num);

void printFieldOfView(FILE* file, struct lterm_t* fov);

/// Get int from mpz_t.
int ConvertToInt(struct vint_t* numData);

void gcInitBuiltin();

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

#endif
