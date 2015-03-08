#ifndef __L_TERM_H__
#define __L_TERM_H__

#include <stdint.h>

#include "func_call.h"

#define L_TERM_FRAGMENT_TAG	0
#define L_TERM_CHAIN_TAG	1
#define L_TERM_FUNC_CALL	2

#define GC_MOVED            3

struct lterm_t;
struct lterm_chain_t;

struct fragment_t
{
    uint64_t offset;
    uint64_t length;
};

struct lterm_t
{
	struct lterm_t* prev;
	struct lterm_t* next;

	int tag;

	union
	{
		struct lterm_t* chain;
		struct fragment_t* fragment;
		struct func_call_t* funcCall;
	};
};

#endif
