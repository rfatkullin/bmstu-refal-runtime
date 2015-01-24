// file:source.ref

#include <stdlib.h>

#include <memory_manager.h>
#include <vmachine.h>
#include <builtins.h>

void __initLiteralData()
{
	initAllocator(1024 * 1024 * 1024);
	memMngr.vterms[0] = (struct v_term){.tag = V_IDENT_TAG, .str = "Prout"};
	memMngr.vterms[1] = (struct v_term){.tag = V_IDENT_TAG, .str = "A"};
	memMngr.vterms[2] = (struct v_term){.tag = V_IDENT_TAG, .str = "S"};
	memMngr.vterms[3] = (struct v_term){.tag = V_IDENT_TAG, .str = "D"};
	memMngr.vterms[4] = (struct v_term){.tag = V_IDENT_TAG, .str = "Daasd"};
	memMngr.vterms[5] = (struct v_term){.tag = V_CHAR_TAG, .ch = 'c'};
	memMngr.vterms[6] = (struct v_term){.tag = V_CHAR_TAG, .ch = 'h'};
	memMngr.vterms[7] = (struct v_term){.tag = V_CHAR_TAG, .ch = 'e'};
	memMngr.vterms[8] = (struct v_term){.tag = V_CHAR_TAG, .ch = 'c'};
	memMngr.vterms[9] = (struct v_term){.tag = V_CHAR_TAG, .ch = 'k'};
	memMngr.vterms[10] = (struct v_term){.tag = V_IDENT_TAG, .str = "ASD"};
	memMngr.vterms[11] = (struct v_term){.tag = V_CHAR_TAG, .ch = 'q'};
	memMngr.vterms[12] = (struct v_term){.tag = V_CHAR_TAG, .ch = 'w'};
	memMngr.vterms[13] = (struct v_term){.tag = V_CHAR_TAG, .ch = 'e'};
	memMngr.vterms[14] = (struct v_term){.tag = V_CHAR_TAG, .ch = 'q'};
	memMngr.vterms[15] = (struct v_term){.tag = V_INT_NUM_TAG, .intNum = 2};
	memMngr.vterms[16] = (struct v_term){.tag = V_CHAR_TAG, .ch = 's'};
	memMngr.vterms[17] = (struct v_term){.tag = V_CHAR_TAG, .ch = 'a'};
	memMngr.vterms[18] = (struct v_term){.tag = V_CHAR_TAG, .ch = 'd'};
	memMngr.vterms[19] = (struct v_term){.tag = V_CHAR_TAG, .ch = 'c'};
	memMngr.vterms[20] = (struct v_term){.tag = V_IDENT_TAG, .str = "asdasd"};
	memMngr.vterms[21] = (struct v_term){.tag = V_CHAR_TAG, .ch = 's'};
	memMngr.vterms[22] = (struct v_term){.tag = V_CHAR_TAG, .ch = 'd'};
	memMngr.vterms[23] = (struct v_term){.tag = V_CHAR_TAG, .ch = 'a'};
	memMngr.vterms[24] = (struct v_term){.tag = V_CHAR_TAG, .ch = 's'};
	memMngr.vterms[25] = (struct v_term){.tag = V_INT_NUM_TAG, .intNum = 1};
	memMngr.vterms[26] = (struct v_term){.tag = V_IDENT_TAG, .str = "Ha-ha-ha"};
	memMngr.vterms[27] = (struct v_term){.tag = V_INT_NUM_TAG, .intNum = 2};
	memMngr.vterms[28] = (struct v_term){.tag = V_INT_NUM_TAG, .intNum = 3};
	memMngr.vterms[29] = (struct v_term){.tag = V_INT_NUM_TAG, .intNum = 12};
	memMngr.vterms[30] = (struct v_term){.tag = V_INT_NUM_TAG, .intNum = 3};
	memMngr.vterms[31] = (struct v_term){.tag = V_CHAR_TAG, .ch = 'a'};
	memMngr.vterms[32] = (struct v_term){.tag = V_CHAR_TAG, .ch = 's'};
	memMngr.vterms[33] = (struct v_term){.tag = V_CHAR_TAG, .ch = 'd'};
	memMngr.vterms[34] = (struct v_term){.tag = V_IDENT_TAG, .str = "Hello"};
	memMngr.vterms[35] = (struct v_term){.tag = V_IDENT_TAG, .str = "Aaa"};

	initHeaps(2, 36);
} // __initLiteralData()

struct func_result_t Go(int entryPoint, struct env_t* env, struct field_view_t* fieldOfView) 
{
	struct func_result_t funcRes;
	if (entryPoint == 0)
	{
		env->locals = (struct lterm_t*)malloc(1 * sizeof(struct lterm_t));
		fieldOfView->backups = (struct lterm_chain_t*)malloc(1 * sizeof(struct lterm_chain_t));
	}
	switch (entryPoint)
	{
		case 0: 
		{
			struct lterm_chain_t* funcCallChain = 0;
			struct lterm_t** helper = (struct lterm_t**)malloc(9 * sizeof(struct lterm_t*));
			int i;
			for (i = 0; i < 9; ++i)
			{
				helper[i] = (struct lterm_t*)malloc(sizeof(struct lterm_t));
				helper[i]->chain = (struct lterm_chain_t*)malloc(sizeof(struct lterm_chain_t));
			}
			struct lterm_t* currTerm = 0;
			currTerm = (struct lterm_t*)malloc(sizeof(struct lterm_t));
			currTerm->tag = L_TERM_FRAGMENT_TAG;
			currTerm->fragment = (struct fragment_t*)malloc(sizeof(struct fragment_t));
			currTerm->fragment->offset = 0;
			currTerm->fragment->length = 19;
			helper[1]->chain->begin = currTerm;
			helper[1]->chain->end = currTerm;
			currTerm = (struct lterm_t*)malloc(sizeof(struct lterm_t));
			currTerm->tag = L_TERM_FRAGMENT_TAG;
			currTerm->fragment = (struct fragment_t*)malloc(sizeof(struct fragment_t));
			currTerm->fragment->offset = 19;
			currTerm->fragment->length = 2;
			helper[3]->chain->begin = currTerm;
			helper[3]->chain->end = currTerm;
			currTerm = helper[3];
			currTerm->tag = L_TERM_CHAIN_TAG;
			currTerm->chain->begin->prev = 0;
			currTerm->chain->end->next = 0;
			helper[2]->chain->begin = currTerm;
			helper[2]->chain->end = currTerm;
			currTerm = (struct lterm_t*)malloc(sizeof(struct lterm_t));
			currTerm->tag = L_TERM_FRAGMENT_TAG;
			currTerm->fragment = (struct fragment_t*)malloc(sizeof(struct fragment_t));
			currTerm->fragment->offset = 21;
			currTerm->fragment->length = 4;
			helper[2]->chain->end->next = currTerm;
			currTerm->prev = helper[2]->chain->end;
			helper[2]->chain->end = currTerm;
			currTerm = (struct lterm_t*)malloc(sizeof(struct lterm_t));
			currTerm->tag = L_TERM_FRAGMENT_TAG;
			currTerm->fragment = (struct fragment_t*)malloc(sizeof(struct fragment_t));
			currTerm->fragment->offset = 25;
			currTerm->fragment->length = 4;
			helper[4]->chain->begin = currTerm;
			helper[4]->chain->end = currTerm;
			currTerm = helper[4];
			currTerm->tag = L_TERM_CHAIN_TAG;
			currTerm->chain->begin->prev = 0;
			currTerm->chain->end->next = 0;
			helper[2]->chain->end->next = currTerm;
			currTerm->prev = helper[2]->chain->end;
			helper[2]->chain->end = currTerm;
			currTerm = helper[2];
			currTerm->tag = L_TERM_CHAIN_TAG;
			currTerm->chain->begin->prev = 0;
			currTerm->chain->end->next = 0;
			helper[1]->chain->end->next = currTerm;
			currTerm->prev = helper[1]->chain->end;
			helper[1]->chain->end = currTerm;
			currTerm = (struct lterm_t*)malloc(sizeof(struct lterm_t));
			currTerm->tag = L_TERM_FRAGMENT_TAG;
			currTerm->fragment = (struct fragment_t*)malloc(sizeof(struct fragment_t));
			currTerm->fragment->offset = 29;
			currTerm->fragment->length = 2;
			helper[5]->chain->begin = currTerm;
			helper[5]->chain->end = currTerm;
			currTerm = helper[5];
			currTerm->tag = L_TERM_CHAIN_TAG;
			currTerm->chain->begin->prev = 0;
			currTerm->chain->end->next = 0;
			helper[1]->chain->end->next = currTerm;
			currTerm->prev = helper[1]->chain->end;
			helper[1]->chain->end = currTerm;
			currTerm = helper[6];
			currTerm->tag = L_TERM_CHAIN_TAG;
			currTerm->chain->begin = 0;
			currTerm->chain->end = 0;
			helper[1]->chain->end->next = currTerm;
			currTerm->prev = helper[1]->chain->end;
			helper[1]->chain->end = currTerm;
			currTerm = helper[7];
			currTerm->tag = L_TERM_CHAIN_TAG;
			currTerm->chain->begin = 0;
			currTerm->chain->end = 0;
			helper[1]->chain->end->next = currTerm;
			currTerm->prev = helper[1]->chain->end;
			helper[1]->chain->end = currTerm;
			currTerm = (struct lterm_t*)malloc(sizeof(struct lterm_t));
			currTerm->tag = L_TERM_FRAGMENT_TAG;
			currTerm->fragment = (struct fragment_t*)malloc(sizeof(struct fragment_t));
			currTerm->fragment->offset = 31;
			currTerm->fragment->length = 3;
			helper[8]->chain->begin = currTerm;
			helper[8]->chain->end = currTerm;
			currTerm = helper[8];
			currTerm->tag = L_TERM_CHAIN_TAG;
			currTerm->chain->begin->prev = 0;
			currTerm->chain->end->next = 0;
			helper[1]->chain->end->next = currTerm;
			currTerm->prev = helper[1]->chain->end;
			helper[1]->chain->end = currTerm;
			currTerm = (struct lterm_t*)malloc(sizeof(struct lterm_t));
			currTerm->tag = L_TERM_FRAGMENT_TAG;
			currTerm->fragment = (struct fragment_t*)malloc(sizeof(struct fragment_t));
			currTerm->fragment->offset = 34;
			currTerm->fragment->length = 2;
			helper[1]->chain->end->next = currTerm;
			currTerm->prev = helper[1]->chain->end;
			helper[1]->chain->end = currTerm;
			currTerm = helper[1];
			currTerm->tag = L_TERM_CHAIN_TAG;
			currTerm->chain->begin->prev = 0;
			currTerm->chain->end->next = 0;
			currTerm = (struct lterm_t*)malloc(sizeof(struct lterm_t));
			currTerm->tag = L_TERM_FUNC_CALL;
			currTerm->funcCall = (struct func_call_t*)malloc(sizeof(struct func_call_t));
			currTerm->funcCall->funcName = "Prout";
			currTerm->funcCall->funcPtr = Prout;
			currTerm->funcCall->entryPoint = 0;
			currTerm->funcCall->fieldOfView = (struct field_view_t*)malloc(sizeof(struct field_view_t));
			currTerm->funcCall->fieldOfView->current = helper[1]->chain;
			currTerm->funcCall->inField = helper[1];
			funcCallChain = (struct lterm_chain_t*)malloc(sizeof(struct lterm_chain_t));
			currTerm->prev = 0;
			currTerm->next = 0;
			funcCallChain->begin = currTerm;
			funcCallChain->end = currTerm;
			helper[0]->chain->begin = currTerm;
			helper[0]->chain->end = currTerm;
			currTerm = helper[0];
			currTerm->tag = L_TERM_CHAIN_TAG;
			currTerm->chain->begin->prev = 0;
			currTerm->chain->end->next = 0;
			funcRes = (struct func_result_t){.status = OK_RESULT, .fieldChain = currTerm->chain, .callChain = funcCallChain};
			break;
		}
	} // switch block end
	if (funcRes.status != CALL_RESULT)
	{
		free(env->locals);
		free(fieldOfView->backups);
	}
	return funcRes;
} // Go

int main()
{
	__initLiteralData();
	mainLoop(Go);
	return 0;
}
