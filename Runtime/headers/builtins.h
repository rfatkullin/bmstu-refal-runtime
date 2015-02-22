#ifndef __BUILTINS_H__
#define __BUILTINS_H__

#include <unicode/ustdio.h>
#include <unicode/uchar.h>

#include <gmp.h>

#include "lterm.h"
#include "memory_manager.h"
#include "func_call.h"

struct func_result_t Prout(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView);
struct func_result_t Card(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView);

struct func_result_t Add(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView);
struct func_result_t Sub(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView);
struct func_result_t Mul(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView);
struct func_result_t Div(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView);
struct func_result_t Mod(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView);

void initBuiltins();
void deinitBuiltins();

UFILE* input;
UFILE* output;
mpz_t base;

#endif
