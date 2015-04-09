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

#define BUILTIN_FRAG   (VTERM_BRACKETS(CURR_FUNC_CALL->env->assembled[0]))

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
void printFragmentLn(FILE* file, struct fragment_t* frag);

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
