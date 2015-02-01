#ifndef __V_TERM_H__
#define __V_TERM_H__

#define V_CHAR_TAG		0
#define V_IDENT_TAG		1
#define V_INT_NUM_TAG	2
#define V_FLOAT_NUM_TAG	3
#define V_CLOSURE_TAG	4
#define V_BRACKET_TAG	5

#include <stdint.h>

struct v_closure
{
	//const char* funcName;
	//struct func_result_t (*funcPtr)(int entryPoint, struct env_t* env, struct lterm_chain_t* fieldOfView);
	//struct lterm_t* params;
};

struct v_term
{
	int tag;

	union
	{
		char* str;
		char ch;
		int intNum;
		float floatNum;
		struct v_closure* closure;
		uint32_t inBracketLength;
	};
};

void printSymbol(struct v_term* term);

#endif
