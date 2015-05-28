#ifndef _BMSTU_REFC_STACK_BUILTINS_H_
#define _BMSTU_REFC_STACK_BUILTINS_H_

#include <stdint.h>

#include <gmp.h>

#include <lterm.h>
#include <vterm.h>
#include <memory_manager.h>
#include <defines/errors_str.h>
#include <defines/gc_macros.h>

#define HASH_TABLE_SIZE UINT64_C(157)

struct stacks_holder_t
{
    struct stacks_holder_t* prev;
    struct stacks_holder_t* next;
    char* name;
    struct stack_t* stack;
};

struct stack_t
{
    struct stack_t* next;
    struct fragment_t* obj;
};

struct stacks_holder_t* _stacksHolders;


#endif


