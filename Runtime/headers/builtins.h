#ifndef __BUILTINS_H__
#define __BUILTINS_H__

#include <gmp.h>

#include "lterm.h"
#include "memory_manager.h"
#include "func_call.h"

struct func_result_t Prout(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView, int firstCall);
struct func_result_t Card(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView, int firstCall);

struct func_result_t Add(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView, int firstCall);
struct func_result_t Sub(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView, int firstCall);
struct func_result_t Mul(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView, int firstCall);
struct func_result_t Div(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView, int firstCall);
struct func_result_t Mod(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView, int firstCall);

int doubleCmp(double a, double b);

/// Проверка на равенство двух строк. 1 - успех, 0 - неудача.
int UStrCmp(struct v_string* a, struct v_string* b);

/// Проверка на равенство двух чисел. 1 - успех, 0 - неудача.
int IntCmp(struct v_int* a, struct v_int* b);

/// Вывод Unicode строки.
void printUStr(struct v_string* str);

#endif
