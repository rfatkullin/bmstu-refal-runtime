// file:../Tests/NestedFuncs/Env3.ref

#include <stdlib.h>

#include <memory_manager.h>
#include <vmachine.h>
#include <builtins.h>

struct func_result_t Fxy(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView);
struct func_result_t F(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView);
struct func_result_t Go(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView);

void __initLiteralData()
{
	initAllocator(1024 * 1024 * 1024);
	memMngr.vterms[0] = (struct v_term){.tag = V_IDENT_TAG, .str = "Prout"};
	memMngr.vterms[1] = (struct v_term){.tag = V_IDENT_TAG, .str = "Fxy"};
	memMngr.vterms[2] = (struct v_term){.tag = V_CHAR_TAG, .ch = 'a'};
	memMngr.vterms[3] = (struct v_term){.tag = V_CHAR_TAG, .ch = 'b'};
	memMngr.vterms[4] = (struct v_term){.tag = V_CHAR_TAG, .ch = 'a'};
	memMngr.vterms[5] = (struct v_term){.tag = V_CHAR_TAG, .ch = 'a'};
	memMngr.vterms[6] = (struct v_term){.tag = V_CHAR_TAG, .ch = 'a'};
	memMngr.vterms[7] = (struct v_term){.tag = V_CHAR_TAG, .ch = 'b'};
	memMngr.vterms[8] = (struct v_term){.tag = V_CHAR_TAG, .ch = 'b'};
	memMngr.vterms[9] = (struct v_term){.tag = V_CHAR_TAG, .ch = 'b'};
	memMngr.vterms[10] = (struct v_term){.tag = V_CHAR_TAG, .ch = 'z'};
	memMngr.vterms[11] = (struct v_term){.tag = V_CHAR_TAG, .ch = 'a'};
	memMngr.vterms[12] = (struct v_term){.tag = V_CHAR_TAG, .ch = 'a'};
	memMngr.vterms[13] = (struct v_term){.tag = V_CHAR_TAG, .ch = 'a'};

	memMngr.vterms[14] = (struct v_term){.tag = V_IDENT_TAG, .str = "F"};
	memMngr.vterms[15] = (struct v_term){.tag = V_IDENT_TAG, .str = "F"};

	initHeaps(2, 16);
} // __initLiteralData()

struct func_result_t Fxy(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView) 
{
	struct func_result_t funcRes;
	struct lterm_t* funcCallChain = 0;
	struct fragment_t* currFrag = 0;
	struct lterm_t** helper = 0;
	struct lterm_t* currTerm = 0;
	struct lterm_t* funcTerm = 0;
	int fragmentOffset = 0;
	int stretchingVarNumber = 0;
	int stretching = 0;
	int i = 0;
	int j = 0;
	if (*entryPoint == 0)
	{
		env->locals = (struct lterm_t**)malloc(1 * sizeof(struct lterm_t*));
		env->assembledFOVs = (struct lterm_t**)malloc(1 * sizeof(struct lterm_t*));
		env->_FOVs = (struct lterm_t**)malloc(1 * sizeof(struct lterm_t*));
		env->stretchVarsNumber = (int*)malloc(1 * sizeof(int));
		for (i = 0; i < 1; i++)
		{
			env->_FOVs[i] = 0;
			env->assembledFOVs[i] = 0;
			env->locals[i] = (struct lterm_t*)malloc(2 * sizeof(struct lterm_t));
			for (j = 0; j < 2; j++)
			{
				env->locals[i][j].tag = L_TERM_FRAGMENT_TAG;
				env->locals[i][j].fragment = (struct fragment_t*)malloc(sizeof(struct fragment_t));
			}
		}
		for (i = 0; i < 1; ++i )
			env->stretchVarsNumber[i] = 0;
	}
	while(*entryPoint >= 0)
	{
		switch (*entryPoint)
		{
		//Sentence: 0, Pattern: 0
		case 0:
		{
			if (!stretching)
			{
				if (env->_FOVs[0] != fieldOfView)
				{
					//WARN: Correct free env->_FOVs[0]
					env->_FOVs[0] = fieldOfView;
					env->assembledFOVs[0] = getAssembliedChain(fieldOfView);
				}
			}
			currFrag = env->assembledFOVs[0]->fragment;
			fragmentOffset = currFrag->offset;
			stretchingVarNumber = env->stretchVarsNumber[0];
			while (stretchingVarNumber >= 0)
			{
				//From what stretchable variable start?
				switch (stretchingVarNumber)
				{
				case 0:
				//Matching t.X variable
					if (fragmentOffset >= currFrag->offset + currFrag->length)
					{
						stretching = 1;
						stretchingVarNumber = -1;
						break;
					}
					else
					{
						env->locals[0][0].fragment->offset = fragmentOffset;
						if (memMngr.vterms[fragmentOffset].tag == V_BRACKET_TAG)
						{
							env->locals[0][0].fragment->length = memMngr.vterms[fragmentOffset].inBracketLength;
							fragmentOffset += memMngr.vterms[fragmentOffset].inBracketLength;
						}
						else
						{
							env->locals[0][0].fragment->length = 1;
							fragmentOffset++;
						}
					}
				//Matching t.Y variable
					if (fragmentOffset >= currFrag->offset + currFrag->length)
					{
						stretching = 1;
						stretchingVarNumber = -1;
						break;
					}
					else
					{
						env->locals[0][1].fragment->offset = fragmentOffset;
						if (memMngr.vterms[fragmentOffset].tag == V_BRACKET_TAG)
						{
							env->locals[0][1].fragment->length = memMngr.vterms[fragmentOffset].inBracketLength;
							fragmentOffset += memMngr.vterms[fragmentOffset].inBracketLength;
						}
						else
						{
							env->locals[0][1].fragment->length = 1;
							fragmentOffset++;
						}
					}
				} //pattern switch

				if (!stretching)
				{
					if (fragmentOffset - currFrag->offset < currFrag->length)
					{
						stretching = 1;
						stretchingVarNumber = -1;
					}
					else
						break; // Success!
				}
			} // Pattern while

			if (stretchingVarNumber < 0)
			{
				//First pattern of last sentence -> nothing to stretch -> fail!
				funcRes = (struct func_result_t){.status = FAIL_RESULT, .fieldChain = 0, .callChain = 0};
				*entryPoint = -1;
				break;
			}
			//WARN: Correct free funcCallChain.
			free(funcCallChain);
			funcCallChain = 0;
			//WARN: Correct free prev helper.
			helper = (struct lterm_t**)malloc(1 * sizeof(struct lterm_t*));
			for (i = 0; i < 1; ++i)
			{
				helper[i] = (struct lterm_t*)malloc(sizeof(struct lterm_t));
				helper[i]->tag = L_TERM_CHAIN_TAG;
				helper[i]->chain = (struct lterm_t*)malloc(sizeof(struct lterm_t));
				helper[i]->chain->prev = helper[i]->chain;
				helper[i]->chain->next = helper[i]->chain;
			}
			//Start construction parenthesis.
			//Start construction anonym func term term.
			currTerm = (struct lterm_t*)malloc(sizeof(struct lterm_t));
			currTerm->tag = L_TERM_FRAGMENT_TAG;
			currTerm->fragment = (struct fragment_t*)malloc(sizeof(struct fragment_t));
			currTerm->fragment->offset = allocateClosure(F, 4);
			currTerm->fragment->length = 1;
			memMngr.vterms[currTerm->fragment->offset].closure->env[0] = env->locals[0][0];
			memMngr.vterms[currTerm->fragment->offset].closure->env[2] = env->locals[0][1];
			memMngr.vterms[currTerm->fragment->offset].closure->env[3] = env->params[0];
			memMngr.vterms[currTerm->fragment->offset].closure->env[1] = env->params[0];
			//Finish construction anonym func term term.
			//Adding term to field chain -- Just concat.
			helper[0]->chain->prev->next = currTerm;
			currTerm->prev = helper[0]->chain->prev;
			helper[0]->chain->prev = currTerm;
			currTerm->next = helper[0]->chain;
			//Finished construction parenthesis. Save in currTerm.
			currTerm = helper[0];
			fieldOfView = currTerm->chain;
			funcRes = (struct func_result_t){.status = OK_RESULT, .fieldChain = currTerm->chain, .callChain = funcCallChain};
			*entryPoint = -1;
			break; //Successful end of sentence
		} // Pattern case end
		} // Entry point switch end
	} // Main while end
	if (funcRes.status != CALL_RESULT)
	{
		free(env->locals);
		free(env->stretchVarsNumber);
		free(env->assembledFOVs);
	}
	return funcRes;
} // func Fxy

struct func_result_t Go(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView) 
{
	struct func_result_t funcRes;
	struct lterm_t* funcCallChain = 0;
	struct fragment_t* currFrag = 0;
	struct lterm_t** helper = 0;
	struct lterm_t* currTerm = 0;
	struct lterm_t* funcTerm = 0;
	int fragmentOffset = 0;
	int stretchingVarNumber = 0;
	int stretching = 0;
	int i = 0;
	int j = 0;
	if (*entryPoint == 0)
	{
		env->locals = (struct lterm_t**)malloc(1 * sizeof(struct lterm_t*));
		env->assembledFOVs = (struct lterm_t**)malloc(1 * sizeof(struct lterm_t*));
		env->_FOVs = (struct lterm_t**)malloc(1 * sizeof(struct lterm_t*));
		env->stretchVarsNumber = (int*)malloc(1 * sizeof(int));
		for (i = 0; i < 1; i++)
		{
			env->_FOVs[i] = 0;
			env->assembledFOVs[i] = 0;
			env->locals[i] = (struct lterm_t*)malloc(0 * sizeof(struct lterm_t));
			for (j = 0; j < 0; j++)
			{
				env->locals[i][j].tag = L_TERM_FRAGMENT_TAG;
				env->locals[i][j].fragment = (struct fragment_t*)malloc(sizeof(struct fragment_t));
			}
		}
		for (i = 0; i < 1; ++i )
			env->stretchVarsNumber[i] = 0;
	}
	while(*entryPoint >= 0)
	{
		switch (*entryPoint)
		{
		//Sentence: 0, Pattern: 0
		case 0:
		{
			if (!stretching)
			{
				if (env->_FOVs[0] != fieldOfView)
				{
					//WARN: Correct free env->_FOVs[0]
					env->_FOVs[0] = fieldOfView;
					env->assembledFOVs[0] = getAssembliedChain(fieldOfView);
				}
			}
			currFrag = env->assembledFOVs[0]->fragment;
			fragmentOffset = currFrag->offset;
			stretchingVarNumber = env->stretchVarsNumber[0];
			while (stretchingVarNumber >= 0)
			{
				if (currFrag->length > 0)
				{
					stretching = 1;
					stretchingVarNumber = -1;
				}
				break;
			} // Pattern while

			if (stretchingVarNumber < 0)
			{
				//First pattern of last sentence -> nothing to stretch -> fail!
				funcRes = (struct func_result_t){.status = FAIL_RESULT, .fieldChain = 0, .callChain = 0};
				*entryPoint = -1;
				break;
			}
			//WARN: Correct free funcCallChain.
			free(funcCallChain);
			funcCallChain = 0;
			//WARN: Correct free prev helper.
			helper = (struct lterm_t**)malloc(4 * sizeof(struct lterm_t*));
			for (i = 0; i < 4; ++i)
			{
				helper[i] = (struct lterm_t*)malloc(sizeof(struct lterm_t));
				helper[i]->tag = L_TERM_CHAIN_TAG;
				helper[i]->chain = (struct lterm_t*)malloc(sizeof(struct lterm_t));
				helper[i]->chain->prev = helper[i]->chain;
				helper[i]->chain->next = helper[i]->chain;
			}
			//Start construction parenthesis.
			//Start construction func call.
			//Start construction parenthesis.
			//Start construction func call.
			//Start construction parenthesis.
			//Start construction func call.
			//Start construction parenthesis.
			//Start construction fragment term.
			currTerm = (struct lterm_t*)malloc(sizeof(struct lterm_t));
			currTerm->tag = L_TERM_FRAGMENT_TAG;
			currTerm->fragment = (struct fragment_t*)malloc(sizeof(struct fragment_t));
			currTerm->fragment->offset = 2;
			currTerm->fragment->length = 2;
			//Adding term to field chain -- Just concat.
			helper[3]->chain->prev->next = currTerm;
			currTerm->prev = helper[3]->chain->prev;
			helper[3]->chain->prev = currTerm;
			currTerm->next = helper[3]->chain;
			//Finished construction parenthesis. Save in currTerm.
			currTerm = helper[3];
			funcTerm = (struct lterm_t*)malloc(sizeof(struct lterm_t));
			funcTerm->tag = L_TERM_FUNC_CALL;
			funcTerm->funcCall = (struct func_call_t*)malloc(sizeof(struct func_call_t));
			funcTerm->funcCall->env = (struct env_t*)malloc(sizeof(struct env_t));
			funcTerm->funcCall->funcName = "Fxy";
			funcTerm->funcCall->funcPtr = Fxy;
			funcTerm->funcCall->entryPoint = 0;
			funcTerm->funcCall->fieldOfView = currTerm->chain;
			//WARN: Correct free currTerm.
			free(currTerm);
			//Finished construction func call
			//First call in call chain -- Initialization.
			funcCallChain = (struct lterm_t*)malloc(sizeof(struct lterm_t));
			funcCallChain->next = funcTerm;
			funcCallChain->prev = funcTerm;
			currTerm = funcTerm;
			//Adding term to field chain -- Just concat.
			helper[2]->chain->prev->next = currTerm;
			currTerm->prev = helper[2]->chain->prev;
			helper[2]->chain->prev = currTerm;
			currTerm->next = helper[2]->chain;
			//Start construction fragment term.
			currTerm = (struct lterm_t*)malloc(sizeof(struct lterm_t));
			currTerm->tag = L_TERM_FRAGMENT_TAG;
			currTerm->fragment = (struct fragment_t*)malloc(sizeof(struct fragment_t));
			currTerm->fragment->offset = 4;
			currTerm->fragment->length = 10;
			//Adding term to field chain -- Just concat.
			helper[2]->chain->prev->next = currTerm;
			currTerm->prev = helper[2]->chain->prev;
			helper[2]->chain->prev = currTerm;
			currTerm->next = helper[2]->chain;
			//Finished construction parenthesis. Save in currTerm.
			currTerm = helper[2];
			funcTerm = (struct lterm_t*)malloc(sizeof(struct lterm_t));
			funcTerm->tag = L_TERM_FUNC_CALL;
			funcTerm->funcCall = (struct func_call_t*)malloc(sizeof(struct func_call_t));
			funcTerm->funcCall->env = (struct env_t*)malloc(sizeof(struct env_t));
			funcTerm->funcCall->funcName = "FuncInVariable";
			funcTerm->funcCall->funcPtr = 0;
			funcTerm->funcCall->entryPoint = 0;
			funcTerm->funcCall->fieldOfView = currTerm->chain;
			//WARN: Correct free currTerm.
			free(currTerm);
			//Finished construction func call
			//Adding call to call chain -- Just concat.
			funcCallChain->prev->funcCall->next = funcTerm;
			funcCallChain->prev = funcTerm;
			currTerm = funcTerm;
			//Adding term to field chain -- Just concat.
			helper[1]->chain->prev->next = currTerm;
			currTerm->prev = helper[1]->chain->prev;
			helper[1]->chain->prev = currTerm;
			currTerm->next = helper[1]->chain;
			//Finished construction parenthesis. Save in currTerm.
			currTerm = helper[1];
			funcTerm = (struct lterm_t*)malloc(sizeof(struct lterm_t));
			funcTerm->tag = L_TERM_FUNC_CALL;
			funcTerm->funcCall = (struct func_call_t*)malloc(sizeof(struct func_call_t));
			funcTerm->funcCall->env = (struct env_t*)malloc(sizeof(struct env_t));
			funcTerm->funcCall->funcName = "Prout";
			funcTerm->funcCall->funcPtr = Prout;
			funcTerm->funcCall->entryPoint = 0;
			funcTerm->funcCall->fieldOfView = currTerm->chain;
			//WARN: Correct free currTerm.
			free(currTerm);
			//Finished construction func call
			//Adding call to call chain -- Just concat.
			funcCallChain->prev->funcCall->next = funcTerm;
			funcCallChain->prev = funcTerm;
			currTerm = funcTerm;
			//Adding term to field chain -- Just concat.
			helper[0]->chain->prev->next = currTerm;
			currTerm->prev = helper[0]->chain->prev;
			helper[0]->chain->prev = currTerm;
			currTerm->next = helper[0]->chain;
			//Finished construction parenthesis. Save in currTerm.
			currTerm = helper[0];
			fieldOfView = currTerm->chain;
			funcRes = (struct func_result_t){.status = OK_RESULT, .fieldChain = currTerm->chain, .callChain = funcCallChain};
			*entryPoint = -1;
			break; //Successful end of sentence
		} // Pattern case end
		} // Entry point switch end
	} // Main while end
	if (funcRes.status != CALL_RESULT)
	{
		free(env->locals);
		free(env->stretchVarsNumber);
		free(env->assembledFOVs);
	}
	return funcRes;
} // func Go

struct func_result_t F(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView) 
{
	struct func_result_t funcRes;
	struct lterm_t* funcCallChain = 0;
	struct fragment_t* currFrag = 0;
	struct lterm_t** helper = 0;
	struct lterm_t* currTerm = 0;
	struct lterm_t* funcTerm = 0;
	int fragmentOffset = 0;
	int stretchingVarNumber = 0;
	int stretching = 0;
	int i = 0;
	int j = 0;
	if (*entryPoint == 0)
	{
		env->locals = (struct lterm_t**)malloc(1 * sizeof(struct lterm_t*));
		env->assembledFOVs = (struct lterm_t**)malloc(1 * sizeof(struct lterm_t*));
		env->_FOVs = (struct lterm_t**)malloc(1 * sizeof(struct lterm_t*));
		env->stretchVarsNumber = (int*)malloc(1 * sizeof(int));
		for (i = 0; i < 1; i++)
		{
			env->_FOVs[i] = 0;
			env->assembledFOVs[i] = 0;
			env->locals[i] = (struct lterm_t*)malloc(2 * sizeof(struct lterm_t));
			for (j = 0; j < 2; j++)
			{
				env->locals[i][j].tag = L_TERM_FRAGMENT_TAG;
				env->locals[i][j].fragment = (struct fragment_t*)malloc(sizeof(struct fragment_t));
			}
		}
		for (i = 0; i < 1; ++i )
			env->stretchVarsNumber[i] = 0;
	}
	while(*entryPoint >= 0)
	{
		switch (*entryPoint)
		{
		//Sentence: 0, Pattern: 0
		case 0:
		{
			if (!stretching)
			{
				if (env->_FOVs[0] != fieldOfView)
				{
					//WARN: Correct free env->_FOVs[0]
					env->_FOVs[0] = fieldOfView;
					env->assembledFOVs[0] = getAssembliedChain(fieldOfView);
				}
			}
			currFrag = env->assembledFOVs[0]->fragment;
			fragmentOffset = currFrag->offset;
			stretchingVarNumber = env->stretchVarsNumber[0];
			while (stretchingVarNumber >= 0)
			{
				//From what stretchable variable start?
				switch (stretchingVarNumber)
				{
				case 0:
				//Matching t.X variable
					if (fragmentOffset >= currFrag->offset + currFrag->length)
					{
						stretching = 1;
						stretchingVarNumber = -1;
						break;
					}
					for (i = 0; i < env->params[0].fragment->length; i++)
					{
						if((memMngr.vterms[fragmentOffset + i].tag != memMngr.vterms[env->params[0].fragment->offset + i].tag)
						|| (memMngr.vterms[fragmentOffset + i].tag == V_CHAR_TAG && memMngr.vterms[fragmentOffset + i].ch != memMngr.vterms[env->params[0].fragment->offset + i].ch)
						|| (memMngr.vterms[fragmentOffset + i].tag == V_IDENT_TAG && strcmp(memMngr.vterms[fragmentOffset + i].str, memMngr.vterms[env->params[0].fragment->offset + i].str))
						|| (memMngr.vterms[fragmentOffset + i].tag == V_INT_NUM_TAG && memMngr.vterms[fragmentOffset + i].intNum != memMngr.vterms[env->params[0].fragment->offset + i].intNum)
						|| (memMngr.vterms[fragmentOffset + i].tag == V_FLOAT_NUM_TAG && memMngr.vterms[fragmentOffset + i].floatNum != memMngr.vterms[env->params[0].fragment->offset + i].floatNum)
						|| (memMngr.vterms[fragmentOffset + i].tag == V_BRACKET_TAG && memMngr.vterms[fragmentOffset + i].inBracketLength != memMngr.vterms[env->params[0].fragment->offset + i].inBracketLength))
						break;
					}
					if(i < env->params[0].fragment->length)
					{
						stretching = 1;
						stretchingVarNumber = -1;
						break;
					}
					fragmentOffset += env->params[0].fragment->length;
				//Matching e.1 variable
				case 1:
					if (!stretching) // Just init values
					{
						env->locals[0][0].fragment->offset = fragmentOffset;
						env->locals[0][0].fragment->length = 0;
					}
					else // stretching
					{
						stretching = 0;
						env->stretchVarsNumber[0] = 0;
						//Restore last offset at this point
						fragmentOffset = env->locals[0][0].fragment->offset + env->locals[0][0].fragment->length;
						if (fragmentOffset >= currFrag->offset + currFrag->length)
						{
							stretching = 1;
							stretchingVarNumber = -1;
							break;
						}
						if (memMngr.vterms[fragmentOffset].tag == V_BRACKET_TAG)
						{
							env->locals[0][0].fragment->length += memMngr.vterms[fragmentOffset].inBracketLength;
							fragmentOffset += memMngr.vterms[fragmentOffset].inBracketLength;
						}
						else
						{
							fragmentOffset += 1;
							env->locals[0][0].fragment->length += 1;
						}
					}
				} //pattern switch

				if (!stretching)
				{
					if (fragmentOffset - currFrag->offset < currFrag->length)
					{
						stretching = 1;
						stretchingVarNumber = 1;
					}
					else
						break; // Success!
				}
			} // Pattern while

			if (stretchingVarNumber < 0)
			{
				//First pattern of current sentence -> jump to first pattern of next sentence!
				stretching = 0;
				*entryPoint = 1;
				for (i = 0; i < 1; ++i )
					env->stretchVarsNumber[i] = 0;
				break;
			}
			//WARN: Correct free funcCallChain.
			free(funcCallChain);
			funcCallChain = 0;
			//WARN: Correct free prev helper.
			helper = (struct lterm_t**)malloc(2 * sizeof(struct lterm_t*));
			for (i = 0; i < 2; ++i)
			{
				helper[i] = (struct lterm_t*)malloc(sizeof(struct lterm_t));
				helper[i]->tag = L_TERM_CHAIN_TAG;
				helper[i]->chain = (struct lterm_t*)malloc(sizeof(struct lterm_t));
				helper[i]->chain->prev = helper[i]->chain;
				helper[i]->chain->next = helper[i]->chain;
			}
			//Start construction parenthesis.
			currTerm = &env->params[2];
			//Adding term to field chain -- Just concat.
			helper[0]->chain->prev->next = currTerm;
			currTerm->prev = helper[0]->chain->prev;
			helper[0]->chain->prev = currTerm;
			currTerm->next = helper[0]->chain;
			//Start construction func call.
			//Start construction parenthesis.
			currTerm = &env->locals[0][0];
			//Adding term to field chain -- Just concat.
			helper[1]->chain->prev->next = currTerm;
			currTerm->prev = helper[1]->chain->prev;
			helper[1]->chain->prev = currTerm;
			currTerm->next = helper[1]->chain;
			//Finished construction parenthesis. Save in currTerm.
			currTerm = helper[1];
			funcTerm = (struct lterm_t*)malloc(sizeof(struct lterm_t));
			funcTerm->tag = L_TERM_FUNC_CALL;
			funcTerm->funcCall = (struct func_call_t*)malloc(sizeof(struct func_call_t));
			funcTerm->funcCall->env = (struct env_t*)malloc(sizeof(struct env_t));
			funcTerm->funcCall->funcName = "F";
			funcTerm->funcCall->funcPtr = F;
			funcTerm->funcCall->entryPoint = 0;
			funcTerm->funcCall->fieldOfView = currTerm->chain;
			//WARN: Correct free currTerm.
			free(currTerm);
			//Finished construction func call
			//First call in call chain -- Initialization.
			funcCallChain = (struct lterm_t*)malloc(sizeof(struct lterm_t));
			funcCallChain->next = funcTerm;
			funcCallChain->prev = funcTerm;
			currTerm = funcTerm;
			//Adding term to field chain -- Just concat.
			helper[0]->chain->prev->next = currTerm;
			currTerm->prev = helper[0]->chain->prev;
			helper[0]->chain->prev = currTerm;
			currTerm->next = helper[0]->chain;
			//Finished construction parenthesis. Save in currTerm.
			currTerm = helper[0];
			fieldOfView = currTerm->chain;
			funcRes = (struct func_result_t){.status = OK_RESULT, .fieldChain = currTerm->chain, .callChain = funcCallChain};
			*entryPoint = -1;
			break; //Successful end of sentence
		} // Pattern case end
		//Sentence: 1, Pattern: 0
		case 1:
		{
			if (!stretching)
			{
				if (env->_FOVs[0] != fieldOfView)
				{
					//WARN: Correct free env->_FOVs[0]
					env->_FOVs[0] = fieldOfView;
					env->assembledFOVs[0] = getAssembliedChain(fieldOfView);
				}
			}
			currFrag = env->assembledFOVs[0]->fragment;
			fragmentOffset = currFrag->offset;
			stretchingVarNumber = env->stretchVarsNumber[1];
			while (stretchingVarNumber >= 0)
			{
				//From what stretchable variable start?
				switch (stretchingVarNumber)
				{
				case 0:
				//Matching t.Z variable
					if (fragmentOffset >= currFrag->offset + currFrag->length)
					{
						stretching = 1;
						stretchingVarNumber = -1;
						break;
					}
					else
					{
						env->locals[0][0].fragment->offset = fragmentOffset;
						if (memMngr.vterms[fragmentOffset].tag == V_BRACKET_TAG)
						{
							env->locals[0][0].fragment->length = memMngr.vterms[fragmentOffset].inBracketLength;
							fragmentOffset += memMngr.vterms[fragmentOffset].inBracketLength;
						}
						else
						{
							env->locals[0][0].fragment->length = 1;
							fragmentOffset++;
						}
					}
				//Matching e.1 variable
				case 1:
					if (!stretching) // Just init values
					{
						env->locals[0][1].fragment->offset = fragmentOffset;
						env->locals[0][1].fragment->length = 0;
					}
					else // stretching
					{
						stretching = 0;
						env->stretchVarsNumber[0] = 1;
						//Restore last offset at this point
						fragmentOffset = env->locals[0][1].fragment->offset + env->locals[0][1].fragment->length;
						if (fragmentOffset >= currFrag->offset + currFrag->length)
						{
							stretching = 1;
							stretchingVarNumber = -1;
							break;
						}
						if (memMngr.vterms[fragmentOffset].tag == V_BRACKET_TAG)
						{
							env->locals[0][1].fragment->length += memMngr.vterms[fragmentOffset].inBracketLength;
							fragmentOffset += memMngr.vterms[fragmentOffset].inBracketLength;
						}
						else
						{
							fragmentOffset += 1;
							env->locals[0][1].fragment->length += 1;
						}
					}
				} //pattern switch

				if (!stretching)
				{
					if (fragmentOffset - currFrag->offset < currFrag->length)
					{
						stretching = 1;
						stretchingVarNumber = 1;
					}
					else
						break; // Success!
				}
			} // Pattern while

			if (stretchingVarNumber < 0)
			{
				//First pattern of current sentence -> jump to first pattern of next sentence!
				stretching = 0;
				*entryPoint = 2;
				for (i = 0; i < 1; ++i )
					env->stretchVarsNumber[i] = 0;
				break;
			}
			//WARN: Correct free funcCallChain.
			free(funcCallChain);
			funcCallChain = 0;
			//WARN: Correct free prev helper.
			helper = (struct lterm_t**)malloc(2 * sizeof(struct lterm_t*));
			for (i = 0; i < 2; ++i)
			{
				helper[i] = (struct lterm_t*)malloc(sizeof(struct lterm_t));
				helper[i]->tag = L_TERM_CHAIN_TAG;
				helper[i]->chain = (struct lterm_t*)malloc(sizeof(struct lterm_t));
				helper[i]->chain->prev = helper[i]->chain;
				helper[i]->chain->next = helper[i]->chain;
			}
			//Start construction parenthesis.
			currTerm = &env->locals[1][0];
			//Adding term to field chain -- Just concat.
			helper[0]->chain->prev->next = currTerm;
			currTerm->prev = helper[0]->chain->prev;
			helper[0]->chain->prev = currTerm;
			currTerm->next = helper[0]->chain;
			//Start construction func call.
			//Start construction parenthesis.
			currTerm = &env->locals[1][1];
			//Adding term to field chain -- Just concat.
			helper[1]->chain->prev->next = currTerm;
			currTerm->prev = helper[1]->chain->prev;
			helper[1]->chain->prev = currTerm;
			currTerm->next = helper[1]->chain;
			//Finished construction parenthesis. Save in currTerm.
			currTerm = helper[1];
			funcTerm = (struct lterm_t*)malloc(sizeof(struct lterm_t));
			funcTerm->tag = L_TERM_FUNC_CALL;
			funcTerm->funcCall = (struct func_call_t*)malloc(sizeof(struct func_call_t));
			funcTerm->funcCall->env = (struct env_t*)malloc(sizeof(struct env_t));
			funcTerm->funcCall->funcName = "F";
			funcTerm->funcCall->funcPtr = F;
			funcTerm->funcCall->entryPoint = 0;
			funcTerm->funcCall->fieldOfView = currTerm->chain;
			//WARN: Correct free currTerm.
			free(currTerm);
			//Finished construction func call
			//First call in call chain -- Initialization.
			funcCallChain = (struct lterm_t*)malloc(sizeof(struct lterm_t));
			funcCallChain->next = funcTerm;
			funcCallChain->prev = funcTerm;
			currTerm = funcTerm;
			//Adding term to field chain -- Just concat.
			helper[0]->chain->prev->next = currTerm;
			currTerm->prev = helper[0]->chain->prev;
			helper[0]->chain->prev = currTerm;
			currTerm->next = helper[0]->chain;
			//Finished construction parenthesis. Save in currTerm.
			currTerm = helper[0];
			fieldOfView = currTerm->chain;
			funcRes = (struct func_result_t){.status = OK_RESULT, .fieldChain = currTerm->chain, .callChain = funcCallChain};
			*entryPoint = -1;
			break; //Successful end of sentence
		} // Pattern case end
		//Sentence: 2, Pattern: 0
		case 2:
		{
			if (!stretching)
			{
				if (env->_FOVs[0] != fieldOfView)
				{
					//WARN: Correct free env->_FOVs[0]
					env->_FOVs[0] = fieldOfView;
					env->assembledFOVs[0] = getAssembliedChain(fieldOfView);
				}
			}
			currFrag = env->assembledFOVs[0]->fragment;
			fragmentOffset = currFrag->offset;
			stretchingVarNumber = env->stretchVarsNumber[2];
			while (stretchingVarNumber >= 0)
			{
				if (currFrag->length > 0)
				{
					stretching = 1;
					stretchingVarNumber = -1;
				}
				break;
			} // Pattern while

			if (stretchingVarNumber < 0)
			{
				//First pattern of last sentence -> nothing to stretch -> fail!
				funcRes = (struct func_result_t){.status = FAIL_RESULT, .fieldChain = 0, .callChain = 0};
				*entryPoint = -1;
				break;
			}
			funcRes = (struct func_result_t){.status = OK_RESULT, .fieldChain = 0, .callChain = 0};
			*entryPoint = -1;
			break; //Successful end of sentence
		} // Pattern case end
		} // Entry point switch end
	} // Main while end
	if (funcRes.status != CALL_RESULT)
	{
		free(env->locals);
		free(env->stretchVarsNumber);
		free(env->assembledFOVs);
	}
	return funcRes;
} // func F

int main()
{
	__initLiteralData();
	mainLoop(Go);
	return 0;
}
