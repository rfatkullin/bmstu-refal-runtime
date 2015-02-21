#ifndef __V_TERM_H__
#define __V_TERM_H__

#define V_CHAR_TAG		0
#define V_IDENT_TAG		1
#define V_INT_NUM_TAG	2
#define V_FLOAT_NUM_TAG	3
#define V_CLOSURE_TAG	4
#define V_BRACKET_TAG	5

#include <stdint.h>

#include "lterm.h"
#include "func_call.h"
#include "vmachine.h"

struct v_closure
{
	RefalFunc funcPtr;
	struct lterm_t* env;
	const char* ident;
};

struct v_term
{
	int tag;

	union
	{
		char* str;
		uint32_t ch;
		float floatNum;
		struct v_closure* closure;
		uint32_t intNum;

		// Хранит длину всей скобочной последовательности. Для () равна 2, для (1 2) равна 4 и т.д.
		uint32_t inBracketLength;
	};
};

#endif
