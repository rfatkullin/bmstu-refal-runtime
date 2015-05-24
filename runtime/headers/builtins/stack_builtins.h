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

struct stack_node_t
{
    struct stack_node_t* next;
    struct lterm_t* obj;
};

struct stack_node_t _hashTable[HASH_TABLE_SIZE];

void initHash();

uint64_t getKeyForVTermsFragment(uint64_t offset, uint64_t length);

struct lterm_t* getObj(uint64_t key);

#endif


