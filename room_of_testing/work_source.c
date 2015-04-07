// file:source.ref

#include <stdlib.h>
#include <stdio.h>

#include <memory_manager.h>
#include <allocators/vterm_alloc.h>
#include <allocators/data_alloc.h>
#include <vmachine.h>
#include <builtins/builtins.h>

struct func_result_t func_0(int entryStatus);
struct func_result_t func_1(int entryStatus);
struct func_result_t func_2(int entryStatus);

void initLiteralData()
{
	initAllocator(1024 * 1024 * 1024);
	memMngr.vterms[0] = (struct v_term){.tag = V_IDENT_TAG, .str = allocateVStringLiteral((uint32_t[]){67,111,109,112,97,114,101}, UINT64_C(7))};
	memMngr.vterms[1] = (struct v_term){.tag = V_CHAR_TAG, .ch = 43};
	memMngr.vterms[2] = (struct v_term){.tag = V_CHAR_TAG, .ch = 76};
	memMngr.vterms[3] = (struct v_term){.tag = V_CHAR_TAG, .ch = 97};
	memMngr.vterms[4] = (struct v_term){.tag = V_CHAR_TAG, .ch = 114};
	memMngr.vterms[5] = (struct v_term){.tag = V_CHAR_TAG, .ch = 103};
	memMngr.vterms[6] = (struct v_term){.tag = V_CHAR_TAG, .ch = 101};
	memMngr.vterms[7] = (struct v_term){.tag = V_CHAR_TAG, .ch = 114};
	memMngr.vterms[8] = (struct v_term){.tag = V_CHAR_TAG, .ch = 45};
	memMngr.vterms[9] = (struct v_term){.tag = V_CHAR_TAG, .ch = 76};
	memMngr.vterms[10] = (struct v_term){.tag = V_CHAR_TAG, .ch = 101};
	memMngr.vterms[11] = (struct v_term){.tag = V_CHAR_TAG, .ch = 115};
	memMngr.vterms[12] = (struct v_term){.tag = V_CHAR_TAG, .ch = 115};
	memMngr.vterms[13] = (struct v_term){.tag = V_CHAR_TAG, .ch = 48};
	memMngr.vterms[14] = (struct v_term){.tag = V_CHAR_TAG, .ch = 69};
	memMngr.vterms[15] = (struct v_term){.tag = V_CHAR_TAG, .ch = 113};
	memMngr.vterms[16] = (struct v_term){.tag = V_CHAR_TAG, .ch = 117};
	memMngr.vterms[17] = (struct v_term){.tag = V_CHAR_TAG, .ch = 97};
	memMngr.vterms[18] = (struct v_term){.tag = V_CHAR_TAG, .ch = 108};
	memMngr.vterms[19] = (struct v_term){.tag = V_IDENT_TAG, .str = allocateVStringLiteral((uint32_t[]){80,114,111,117,116}, UINT64_C(5))};
	memMngr.vterms[20] = (struct v_term){.tag = V_IDENT_TAG, .str = allocateVStringLiteral((uint32_t[]){70,117,110,99}, UINT64_C(4))};
	memMngr.vterms[21] = (struct v_term){.tag = V_INT_NUM_TAG, .intNum = allocateIntNumberLiteral((uint8_t[]){100}, 0, UINT64_C(1))};
	memMngr.vterms[22] = (struct v_term){.tag = V_INT_NUM_TAG, .intNum = allocateIntNumberLiteral((uint8_t[]){200}, 0, UINT64_C(1))};
	memMngr.vterms[23] = (struct v_term){.tag = V_IDENT_TAG, .str = allocateVStringLiteral((uint32_t[]){80,114,111,117,116}, UINT64_C(5))};
	memMngr.vterms[24] = (struct v_term){.tag = V_IDENT_TAG, .str = allocateVStringLiteral((uint32_t[]){70,117,110,99}, UINT64_C(4))};
	memMngr.vterms[25] = (struct v_term){.tag = V_INT_NUM_TAG, .intNum = allocateIntNumberLiteral((uint8_t[]){200}, 0, UINT64_C(1))};
	memMngr.vterms[26] = (struct v_term){.tag = V_INT_NUM_TAG, .intNum = allocateIntNumberLiteral((uint8_t[]){100}, 0, UINT64_C(1))};
	memMngr.vterms[27] = (struct v_term){.tag = V_IDENT_TAG, .str = allocateVStringLiteral((uint32_t[]){80,114,111,117,116}, UINT64_C(5))};
	memMngr.vterms[28] = (struct v_term){.tag = V_IDENT_TAG, .str = allocateVStringLiteral((uint32_t[]){70,117,110,99}, UINT64_C(4))};
	memMngr.vterms[29] = (struct v_term){.tag = V_INT_NUM_TAG, .intNum = allocateIntNumberLiteral((uint8_t[]){0}, 0, UINT64_C(1))};
	memMngr.vterms[30] = (struct v_term){.tag = V_INT_NUM_TAG, .intNum = allocateIntNumberLiteral((uint8_t[]){0}, 0, UINT64_C(1))};

} // initLiteralData()

struct func_result_t func_1(int entryStatus) 
{
	struct func_result_t funcRes;
	struct fragment_t* currFrag = 0;
	struct lterm_t* workFieldOfView = 0;
	uint64_t fragmentOffset = 0;
	uint64_t rightCheckOffset = 0;
	int stretchingVarNumber = 0;
	int stretching = 0;
	int success = 1;
	int i = 0;
	int j = 0;
	if (entryStatus == FIRST_CALL)
		gcAllocateEnvData(_currFuncCall->env, 2, 1);
	else if (entryStatus == ROLL_BACK)
		stretching = 1;
	while(_currFuncCall->entryPoint >= 0)
	{
		switch (_currFuncCall->entryPoint)
		{
		//Sentence: 0, Pattern: 0
		case 0:
		{
			if (!stretching)
			{
				if (_currFuncCall->fieldOfView)
				{
					_currFuncCall->env->fovs[0] = _currFuncCall->fieldOfView;
					_currFuncCall->env->assembledFOVs[0] = gcGetAssembliedChain(_currFuncCall->fieldOfView);
					_currFuncCall->fieldOfView = 0;
				}
			} // !stretching
			currFrag = _currFuncCall->env->assembledFOVs[0]->fragment;
			rightCheckOffset = currFrag->offset + currFrag->length;
			fragmentOffset = currFrag->offset;
			stretchingVarNumber = _currFuncCall->env->stretchVarsNumber[0];
			while (stretchingVarNumber >= 0)
			{
				//From what stretchable variable start?
				switch (stretchingVarNumber)
				{
				case 0:
				//Matching s.1 variable
					if (fragmentOffset >= rightCheckOffset || memMngr.vterms[fragmentOffset].tag == V_BRACKET_OPEN_TAG || memMngr.vterms[fragmentOffset].tag == V_BRACKET_CLOSE_TAG)
					{
						stretching = 1;
						stretchingVarNumber = -1;
						break;
					}
					else
					{
						_currFuncCall->env->locals[0].fragment->offset = fragmentOffset;
						_currFuncCall->env->locals[0].fragment->length = 1;
						fragmentOffset++;
					}
				//Matching s.2 variable
					if (fragmentOffset >= rightCheckOffset || memMngr.vterms[fragmentOffset].tag == V_BRACKET_OPEN_TAG || memMngr.vterms[fragmentOffset].tag == V_BRACKET_CLOSE_TAG)
					{
						stretching = 1;
						stretchingVarNumber = -1;
						break;
					}
					else
					{
						_currFuncCall->env->locals[1].fragment->offset = fragmentOffset;
						_currFuncCall->env->locals[1].fragment->length = 1;
						fragmentOffset++;
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
				//First pattern of current sentence -> jump to first pattern of next sentence!
				stretching = 0;
				_currFuncCall->entryPoint = 1;
				_currFuncCall->env->stretchVarsNumber[0] = 0;
				for (i = 1; i < 1; ++i )
				{
					_currFuncCall->env->stretchVarsNumber[i] = 0;
					_currFuncCall->env->fovs[i] = 0;
					_currFuncCall->env->assembledFOVs[i] = 0;
				}
				break;
			}
			//Start construction assembly action.
			do { // GC block
				if(!success)
					collectGarbage();
				success = 1;
				allocate_result status = OK;
				struct lterm_t* funcCallChain = 0;
				struct lterm_t* helper = 0;
				struct lterm_t* currTerm = 0;
				struct lterm_t* funcTerm = 0;
				helper = chAllocateChainLTerm(UINT64_C(2), &status);
				if (status == NEED_CLEAN)
				{
					success = 0;
					continue;
				}
				//Start construction parenthesis.
				//Start construction func call.
				//Start construction parenthesis.
				//Start construction func term.
				currTerm = chAllocateFragmentLTerm(1, &status);
				if (status == NEED_CLEAN)
				{
					success = 0;
					continue;
				}
				currTerm->fragment->offset = chAllocateClosureVTerm(&status);
				if (status == NEED_CLEAN)
				{
					success = 0;
					continue;
				}
				memMngr.vterms[currTerm->fragment->offset].closure = chAllocateClosureStruct(Compare, 0, memMngr.vterms[0].str, 0, &status);
				if (status == NEED_CLEAN)
				{
					success = 0;
					continue;
				}
				currTerm->fragment->length = 1;
				//Finish construction func term.
				//Adding term to field chain -- Just concat.
				ADD_TO_CHAIN(helper[1].chain, currTerm);
				currTerm = chAllocateFragmentLTerm(1, &status);
				if (status == NEED_CLEAN)
				{
					success = 0;
					continue;
				}
				currTerm->fragment->offset = _currFuncCall->env->locals[0].fragment->offset;
				currTerm->fragment->length = _currFuncCall->env->locals[0].fragment->length;
				//Adding term to field chain -- Just concat.
				ADD_TO_CHAIN(helper[1].chain, currTerm);
				currTerm = chAllocateFragmentLTerm(1, &status);
				if (status == NEED_CLEAN)
				{
					success = 0;
					continue;
				}
				currTerm->fragment->offset = _currFuncCall->env->locals[1].fragment->offset;
				currTerm->fragment->length = _currFuncCall->env->locals[1].fragment->length;
				//Adding term to field chain -- Just concat.
				ADD_TO_CHAIN(helper[1].chain, currTerm);
				//Finished construction parenthesis. Save in currTerm.
				currTerm = &helper[1];
				funcTerm = chAllocateFuncCallLTerm(&status);
				if (status == NEED_CLEAN)
				{
					success = 0;
					continue;
				}
				funcTerm->funcCall->failEntryPoint = 0;
				funcTerm->funcCall->fieldOfView = currTerm->chain;
				//Finished construction func call
				//First call in call chain -- Initialization.
				funcCallChain = chAllocateChainLTerm(1, &status);
				if (status == NEED_CLEAN)
				{
					success = 0;
					continue;
				}
				funcCallChain->next = funcTerm;
				funcCallChain->prev = funcTerm;
				currTerm = funcTerm;
				//Adding term to field chain -- Just concat.
				ADD_TO_CHAIN(helper[0].chain, currTerm);
				//Finished construction parenthesis. Save in currTerm.
				currTerm = &helper[0];
				_currFuncCall->entryPoint = 1;
				return (struct func_result_t){.status = CALL_RESULT, .fieldChain = currTerm->chain, .callChain = funcCallChain};
			} while (!success); // GC block
		} // Pattern or Call Action case end

		//Sentence: 0, Call Action
		case 1:
		{
			//Start construction func call action.
			do { // GC block
				if(!success)
					collectGarbage();
				success = 1;
				allocate_result status = OK;
				struct lterm_t* funcCallChain = 0;
				struct lterm_t* helper = 0;
				struct lterm_t* currTerm = 0;
				struct lterm_t* funcTerm = 0;
				helper = chAllocateChainLTerm(UINT64_C(2), &status);
				if (status == NEED_CLEAN)
				{
					success = 0;
					continue;
				}
				//Start construction func call.
				//Start construction parenthesis.
				//Start construction func term.
				currTerm = chAllocateFragmentLTerm(1, &status);
				if (status == NEED_CLEAN)
				{
					success = 0;
					continue;
				}
				currTerm->fragment->offset = chAllocateClosureVTerm(&status);
				if (status == NEED_CLEAN)
				{
					success = 0;
					continue;
				}
				memMngr.vterms[currTerm->fragment->offset].closure = chAllocateClosureStruct(func_2, 0, 0, 0, &status);
				if (status == NEED_CLEAN)
				{
					success = 0;
					continue;
				}
				currTerm->fragment->length = 1;
				//Finish construction func term.
				//Adding term to field chain -- Just concat.
				ADD_TO_CHAIN(helper[1].chain, currTerm);
				//Finished construction parenthesis. Save in currTerm.
				currTerm = &helper[1];
				funcTerm = chAllocateFuncCallLTerm(&status);
				if (status == NEED_CLEAN)
				{
					success = 0;
					continue;
				}
				funcTerm->funcCall->failEntryPoint = -1;
				funcTerm->funcCall->fieldOfView = currTerm->chain;
				//Finished construction func call
				//First call in call chain -- Initialization.
				funcCallChain = chAllocateChainLTerm(1, &status);
				if (status == NEED_CLEAN)
				{
					success = 0;
					continue;
				}
				funcCallChain->next = funcTerm;
				funcCallChain->prev = funcTerm;
				currTerm = funcTerm;
				//Adding term to field chain -- Just concat.
				ADD_TO_CHAIN(helper[0].chain, currTerm);
				if (_currFuncCall->fieldOfView)
				{
					// There is assembly with func calls -> get this result.
					CONCAT_CHAINS(funcTerm->funcCall->fieldOfView, _currFuncCall->fieldOfView);
					_currFuncCall->fieldOfView = 0;
				}
				else if (workFieldOfView != 0)
				{
					// There is assembly action in previous actions -> get this result.
					CONCAT_CHAINS(funcTerm->funcCall->fieldOfView, workFieldOfView);
				}
				else
				{
					CHECK_ALLOCATION_CONTINUE(workFieldOfView, chCopyFieldOfView(_currFuncCall->env->fovs[0], &status), status);
					CONCAT_CHAINS(funcTerm->funcCall->fieldOfView, workFieldOfView);
				}
				_currFuncCall->entryPoint = 2;
				return (struct func_result_t){.status = CALL_RESULT, .fieldChain = helper[0].chain, .callChain = funcCallChain};
			} while (!success); // GC block
		} // Pattern or Call Action case end

		case 2:
		{
			workFieldOfView = _currFuncCall->fieldOfView;
			_currFuncCall->fieldOfView = 0;
			funcRes = (struct func_result_t){.status = OK_RESULT, .fieldChain = workFieldOfView, .callChain = 0};
			_currFuncCall->entryPoint = -1;
			break; //Successful end of sentence
		} // Pattern case end
		//Sentence: 1, Pattern: 0
		case 3:
		{
			if (!stretching)
			{
				if (_currFuncCall->fieldOfView)
				{
					_currFuncCall->env->fovs[0] = _currFuncCall->fieldOfView;
					_currFuncCall->env->assembledFOVs[0] = gcGetAssembliedChain(_currFuncCall->fieldOfView);
					_currFuncCall->fieldOfView = 0;
				}
			} // !stretching
			currFrag = _currFuncCall->env->assembledFOVs[0]->fragment;
			rightCheckOffset = currFrag->offset + currFrag->length;
			fragmentOffset = currFrag->offset;
			stretchingVarNumber = _currFuncCall->env->stretchVarsNumber[0];
			while (stretchingVarNumber >= 0)
			{
				//From what stretchable variable start?
				switch (stretchingVarNumber)
				{
				//Matching e.1 variable
				case 0:
					if (!stretching) // Just init values
					{
						_currFuncCall->env->locals[0].fragment->offset = fragmentOffset;
						_currFuncCall->env->locals[0].fragment->length = 0;
					}
					else // stretching
					{
						stretching = 0;
						_currFuncCall->env->stretchVarsNumber[0] = 0;
						//Restore last offset at this point
						fragmentOffset = _currFuncCall->env->locals[0].fragment->offset +  _currFuncCall->env->locals[0].fragment->length;
						if (fragmentOffset >= rightCheckOffset)
						{
							stretching = 1;
							stretchingVarNumber = -1;
							break;
						}
						if (memMngr.vterms[fragmentOffset].tag == V_BRACKET_OPEN_TAG)
						{
							_currFuncCall->env->locals[0].fragment->length += memMngr.vterms[fragmentOffset].inBracketLength;
							fragmentOffset += memMngr.vterms[fragmentOffset].inBracketLength;
						}
						else
						{
							fragmentOffset++;
							_currFuncCall->env->locals[0].fragment->length++;
						}
					}
				} //pattern switch

				if (!stretching)
				{
					if (fragmentOffset - currFrag->offset < currFrag->length)
					{
						stretching = 1;
						stretchingVarNumber = 0;
					}
					else
						break; // Success!
				}
			} // Pattern while

			if (stretchingVarNumber < 0)
			{
				//First pattern of last sentence -> nothing to stretch -> fail!
				funcRes = (struct func_result_t){.status = FAIL_RESULT, .fieldChain = 0, .callChain = 0};
				_currFuncCall->entryPoint = -1;
				break;
			}
			//Start construction assembly action.
			do { // GC block
				if(!success)
					collectGarbage();
				success = 1;
				allocate_result status = OK;
				struct lterm_t* funcCallChain = 0;
				struct lterm_t* helper = 0;
				struct lterm_t* currTerm = 0;
				struct lterm_t* funcTerm = 0;
				helper = chAllocateChainLTerm(UINT64_C(1), &status);
				if (status == NEED_CLEAN)
				{
					success = 0;
					continue;
				}
				//Start construction parenthesis.
				currTerm = chAllocateFragmentLTerm(1, &status);
				if (status == NEED_CLEAN)
				{
					success = 0;
					continue;
				}
				currTerm->fragment->offset = _currFuncCall->env->locals[0].fragment->offset;
				currTerm->fragment->length = _currFuncCall->env->locals[0].fragment->length;
				//Adding term to field chain -- Just concat.
				ADD_TO_CHAIN(helper[0].chain, currTerm);
				//Finished construction parenthesis. Save in currTerm.
				currTerm = &helper[0];
				funcRes = (struct func_result_t){.status = OK_RESULT, .fieldChain = currTerm->chain, .callChain = funcCallChain};
			} while (!success); // GC block
			_currFuncCall->entryPoint = -1;
			break; //Successful end of sentence
		} // Pattern case end
		} // Entry point switch end
	} // Main while end
	return funcRes;
} // func Func:func_1

struct func_result_t func_0(int entryStatus) 
{
	struct func_result_t funcRes;
	struct fragment_t* currFrag = 0;
	struct lterm_t* workFieldOfView = 0;
	uint64_t fragmentOffset = 0;
	uint64_t rightCheckOffset = 0;
	int stretchingVarNumber = 0;
	int stretching = 0;
	int success = 1;
	int i = 0;
	int j = 0;
	if (entryStatus == FIRST_CALL)
		gcAllocateEnvData(_currFuncCall->env, 0, 1);
	else if (entryStatus == ROLL_BACK)
		stretching = 1;
	while(_currFuncCall->entryPoint >= 0)
	{
		switch (_currFuncCall->entryPoint)
		{
		//Sentence: 0, Pattern: 0
		case 0:
		{
			if (!stretching)
			{
				if (_currFuncCall->fieldOfView)
				{
					_currFuncCall->env->fovs[0] = _currFuncCall->fieldOfView;
					_currFuncCall->env->assembledFOVs[0] = gcGetAssembliedChain(_currFuncCall->fieldOfView);
					_currFuncCall->fieldOfView = 0;
				}
			} // !stretching
			currFrag = _currFuncCall->env->assembledFOVs[0]->fragment;
			rightCheckOffset = currFrag->offset + currFrag->length;
			fragmentOffset = currFrag->offset;
			stretchingVarNumber = _currFuncCall->env->stretchVarsNumber[0];
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
				_currFuncCall->entryPoint = -1;
				break;
			}
			//Start construction assembly action.
			do { // GC block
				if(!success)
					collectGarbage();
				success = 1;
				allocate_result status = OK;
				struct lterm_t* funcCallChain = 0;
				struct lterm_t* helper = 0;
				struct lterm_t* currTerm = 0;
				struct lterm_t* funcTerm = 0;
				helper = chAllocateChainLTerm(UINT64_C(7), &status);
				if (status == NEED_CLEAN)
				{
					success = 0;
					continue;
				}
				//Start construction parenthesis.
				//Start construction func call.
				//Start construction parenthesis.
				//Start construction func term.
				currTerm = chAllocateFragmentLTerm(1, &status);
				if (status == NEED_CLEAN)
				{
					success = 0;
					continue;
				}
				currTerm->fragment->offset = chAllocateClosureVTerm(&status);
				if (status == NEED_CLEAN)
				{
					success = 0;
					continue;
				}
				memMngr.vterms[currTerm->fragment->offset].closure = chAllocateClosureStruct(Prout, 0, memMngr.vterms[19].str, 0, &status);
				if (status == NEED_CLEAN)
				{
					success = 0;
					continue;
				}
				currTerm->fragment->length = 1;
				//Finish construction func term.
				//Adding term to field chain -- Just concat.
				ADD_TO_CHAIN(helper[1].chain, currTerm);
				//Start construction func call.
				//Start construction parenthesis.
				//Start construction func term.
				currTerm = chAllocateFragmentLTerm(1, &status);
				if (status == NEED_CLEAN)
				{
					success = 0;
					continue;
				}
				currTerm->fragment->offset = chAllocateClosureVTerm(&status);
				if (status == NEED_CLEAN)
				{
					success = 0;
					continue;
				}
				memMngr.vterms[currTerm->fragment->offset].closure = chAllocateClosureStruct(func_1, 0, memMngr.vterms[20].str, 0, &status);
				if (status == NEED_CLEAN)
				{
					success = 0;
					continue;
				}
				currTerm->fragment->length = 1;
				//Finish construction func term.
				//Adding term to field chain -- Just concat.
				ADD_TO_CHAIN(helper[2].chain, currTerm);
				//Start construction fragment term.
				currTerm = chAllocateFragmentLTerm(1, &status);
				if (status == NEED_CLEAN)
				{
					success = 0;
					continue;
				}
				currTerm->fragment->offset = 21;
				currTerm->fragment->length = 2;
				//Adding term to field chain -- Just concat.
				ADD_TO_CHAIN(helper[2].chain, currTerm);
				//Finished construction parenthesis. Save in currTerm.
				currTerm = &helper[2];
				funcTerm = chAllocateFuncCallLTerm(&status);
				if (status == NEED_CLEAN)
				{
					success = 0;
					continue;
				}
				funcTerm->funcCall->failEntryPoint = -1;
				funcTerm->funcCall->fieldOfView = currTerm->chain;
				//Finished construction func call
				//First call in call chain -- Initialization.
				funcCallChain = chAllocateChainLTerm(1, &status);
				if (status == NEED_CLEAN)
				{
					success = 0;
					continue;
				}
				funcCallChain->next = funcTerm;
				funcCallChain->prev = funcTerm;
				currTerm = funcTerm;
				//Adding term to field chain -- Just concat.
				ADD_TO_CHAIN(helper[1].chain, currTerm);
				//Finished construction parenthesis. Save in currTerm.
				currTerm = &helper[1];
				funcTerm = chAllocateFuncCallLTerm(&status);
				if (status == NEED_CLEAN)
				{
					success = 0;
					continue;
				}
				funcTerm->funcCall->failEntryPoint = -1;
				funcTerm->funcCall->fieldOfView = currTerm->chain;
				//Finished construction func call
				//Adding call to call chain -- Just concat.
				funcCallChain->prev->funcCall->next = funcTerm;
				funcCallChain->prev = funcTerm;
				currTerm = funcTerm;
				//Adding term to field chain -- Just concat.
				ADD_TO_CHAIN(helper[0].chain, currTerm);
				//Start construction func call.
				//Start construction parenthesis.
				//Start construction func term.
				currTerm = chAllocateFragmentLTerm(1, &status);
				if (status == NEED_CLEAN)
				{
					success = 0;
					continue;
				}
				currTerm->fragment->offset = chAllocateClosureVTerm(&status);
				if (status == NEED_CLEAN)
				{
					success = 0;
					continue;
				}
				memMngr.vterms[currTerm->fragment->offset].closure = chAllocateClosureStruct(Prout, 0, memMngr.vterms[23].str, 0, &status);
				if (status == NEED_CLEAN)
				{
					success = 0;
					continue;
				}
				currTerm->fragment->length = 1;
				//Finish construction func term.
				//Adding term to field chain -- Just concat.
				ADD_TO_CHAIN(helper[3].chain, currTerm);
				//Start construction func call.
				//Start construction parenthesis.
				//Start construction func term.
				currTerm = chAllocateFragmentLTerm(1, &status);
				if (status == NEED_CLEAN)
				{
					success = 0;
					continue;
				}
				currTerm->fragment->offset = chAllocateClosureVTerm(&status);
				if (status == NEED_CLEAN)
				{
					success = 0;
					continue;
				}
				memMngr.vterms[currTerm->fragment->offset].closure = chAllocateClosureStruct(func_1, 0, memMngr.vterms[24].str, 0, &status);
				if (status == NEED_CLEAN)
				{
					success = 0;
					continue;
				}
				currTerm->fragment->length = 1;
				//Finish construction func term.
				//Adding term to field chain -- Just concat.
				ADD_TO_CHAIN(helper[4].chain, currTerm);
				//Start construction fragment term.
				currTerm = chAllocateFragmentLTerm(1, &status);
				if (status == NEED_CLEAN)
				{
					success = 0;
					continue;
				}
				currTerm->fragment->offset = 25;
				currTerm->fragment->length = 2;
				//Adding term to field chain -- Just concat.
				ADD_TO_CHAIN(helper[4].chain, currTerm);
				//Finished construction parenthesis. Save in currTerm.
				currTerm = &helper[4];
				funcTerm = chAllocateFuncCallLTerm(&status);
				if (status == NEED_CLEAN)
				{
					success = 0;
					continue;
				}
				funcTerm->funcCall->failEntryPoint = -1;
				funcTerm->funcCall->fieldOfView = currTerm->chain;
				//Finished construction func call
				//Adding call to call chain -- Just concat.
				funcCallChain->prev->funcCall->next = funcTerm;
				funcCallChain->prev = funcTerm;
				currTerm = funcTerm;
				//Adding term to field chain -- Just concat.
				ADD_TO_CHAIN(helper[3].chain, currTerm);
				//Finished construction parenthesis. Save in currTerm.
				currTerm = &helper[3];
				funcTerm = chAllocateFuncCallLTerm(&status);
				if (status == NEED_CLEAN)
				{
					success = 0;
					continue;
				}
				funcTerm->funcCall->failEntryPoint = -1;
				funcTerm->funcCall->fieldOfView = currTerm->chain;
				//Finished construction func call
				//Adding call to call chain -- Just concat.
				funcCallChain->prev->funcCall->next = funcTerm;
				funcCallChain->prev = funcTerm;
				currTerm = funcTerm;
				//Adding term to field chain -- Just concat.
				ADD_TO_CHAIN(helper[0].chain, currTerm);
				//Start construction func call.
				//Start construction parenthesis.
				//Start construction func term.
				currTerm = chAllocateFragmentLTerm(1, &status);
				if (status == NEED_CLEAN)
				{
					success = 0;
					continue;
				}
				currTerm->fragment->offset = chAllocateClosureVTerm(&status);
				if (status == NEED_CLEAN)
				{
					success = 0;
					continue;
				}
				memMngr.vterms[currTerm->fragment->offset].closure = chAllocateClosureStruct(Prout, 0, memMngr.vterms[27].str, 0, &status);
				if (status == NEED_CLEAN)
				{
					success = 0;
					continue;
				}
				currTerm->fragment->length = 1;
				//Finish construction func term.
				//Adding term to field chain -- Just concat.
				ADD_TO_CHAIN(helper[5].chain, currTerm);
				//Start construction func call.
				//Start construction parenthesis.
				//Start construction func term.
				currTerm = chAllocateFragmentLTerm(1, &status);
				if (status == NEED_CLEAN)
				{
					success = 0;
					continue;
				}
				currTerm->fragment->offset = chAllocateClosureVTerm(&status);
				if (status == NEED_CLEAN)
				{
					success = 0;
					continue;
				}
				memMngr.vterms[currTerm->fragment->offset].closure = chAllocateClosureStruct(func_1, 0, memMngr.vterms[28].str, 0, &status);
				if (status == NEED_CLEAN)
				{
					success = 0;
					continue;
				}
				currTerm->fragment->length = 1;
				//Finish construction func term.
				//Adding term to field chain -- Just concat.
				ADD_TO_CHAIN(helper[6].chain, currTerm);
				//Start construction fragment term.
				currTerm = chAllocateFragmentLTerm(1, &status);
				if (status == NEED_CLEAN)
				{
					success = 0;
					continue;
				}
				currTerm->fragment->offset = 29;
				currTerm->fragment->length = 2;
				//Adding term to field chain -- Just concat.
				ADD_TO_CHAIN(helper[6].chain, currTerm);
				//Finished construction parenthesis. Save in currTerm.
				currTerm = &helper[6];
				funcTerm = chAllocateFuncCallLTerm(&status);
				if (status == NEED_CLEAN)
				{
					success = 0;
					continue;
				}
				funcTerm->funcCall->failEntryPoint = -1;
				funcTerm->funcCall->fieldOfView = currTerm->chain;
				//Finished construction func call
				//Adding call to call chain -- Just concat.
				funcCallChain->prev->funcCall->next = funcTerm;
				funcCallChain->prev = funcTerm;
				currTerm = funcTerm;
				//Adding term to field chain -- Just concat.
				ADD_TO_CHAIN(helper[5].chain, currTerm);
				//Finished construction parenthesis. Save in currTerm.
				currTerm = &helper[5];
				funcTerm = chAllocateFuncCallLTerm(&status);
				if (status == NEED_CLEAN)
				{
					success = 0;
					continue;
				}
				funcTerm->funcCall->failEntryPoint = -1;
				funcTerm->funcCall->fieldOfView = currTerm->chain;
				//Finished construction func call
				//Adding call to call chain -- Just concat.
				funcCallChain->prev->funcCall->next = funcTerm;
				funcCallChain->prev = funcTerm;
				currTerm = funcTerm;
				//Adding term to field chain -- Just concat.
				ADD_TO_CHAIN(helper[0].chain, currTerm);
				//Finished construction parenthesis. Save in currTerm.
				currTerm = &helper[0];
				funcRes = (struct func_result_t){.status = OK_RESULT, .fieldChain = currTerm->chain, .callChain = funcCallChain};
			} while (!success); // GC block
			_currFuncCall->entryPoint = -1;
			break; //Successful end of sentence
		} // Pattern case end
		} // Entry point switch end
	} // Main while end
	return funcRes;
} // func Go:func_0

struct func_result_t func_2(int entryStatus) 
{
	struct func_result_t funcRes;
	struct fragment_t* currFrag = 0;
	struct lterm_t* workFieldOfView = 0;
	uint64_t fragmentOffset = 0;
	uint64_t rightCheckOffset = 0;
	int stretchingVarNumber = 0;
	int stretching = 0;
	int success = 1;
	int i = 0;
	int j = 0;
	if (entryStatus == FIRST_CALL)
		gcAllocateEnvData(_currFuncCall->env, 0, 1);
	else if (entryStatus == ROLL_BACK)
		stretching = 1;
	while(_currFuncCall->entryPoint >= 0)
	{
		switch (_currFuncCall->entryPoint)
		{
		//Sentence: 0, Pattern: 0
		case 0:
		{
			if (!stretching)
			{
				if (_currFuncCall->fieldOfView)
				{
					_currFuncCall->env->fovs[0] = _currFuncCall->fieldOfView;
					_currFuncCall->env->assembledFOVs[0] = gcGetAssembliedChain(_currFuncCall->fieldOfView);
					_currFuncCall->fieldOfView = 0;
				}
			} // !stretching
			currFrag = _currFuncCall->env->assembledFOVs[0]->fragment;
			rightCheckOffset = currFrag->offset + currFrag->length;
			fragmentOffset = currFrag->offset;
			stretchingVarNumber = _currFuncCall->env->stretchVarsNumber[0];
			while (stretchingVarNumber >= 0)
			{
				//From what stretchable variable start?
				switch (stretchingVarNumber)
				{
				case 0:
				//Matching string literal
					if (fragmentOffset + UINT64_C(1) - 1 >= rightCheckOffset)
					{
						stretching = 1;
						stretchingVarNumber = -1;
						break;
					}
					for (i = 0; i < UINT64_C(1); i++)
					{
						if (memMngr.vterms[fragmentOffset + i].tag != V_CHAR_TAG || memMngr.vterms[fragmentOffset + i].ch != memMngr.vterms[UINT64_C(1) + i].ch)
						{
							stretching = 1;
							stretchingVarNumber = -1;
							break;
						}
					}
					if (i < 1) // If check is failed
						break;
					fragmentOffset += UINT64_C(1);
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
				//First pattern of current sentence -> jump to first pattern of next sentence!
				stretching = 0;
				_currFuncCall->entryPoint = 1;
				_currFuncCall->env->stretchVarsNumber[0] = 0;
				for (i = 1; i < 1; ++i )
				{
					_currFuncCall->env->stretchVarsNumber[i] = 0;
					_currFuncCall->env->fovs[i] = 0;
					_currFuncCall->env->assembledFOVs[i] = 0;
				}
				break;
			}
			//Start construction assembly action.
			do { // GC block
				if(!success)
					collectGarbage();
				success = 1;
				allocate_result status = OK;
				struct lterm_t* funcCallChain = 0;
				struct lterm_t* helper = 0;
				struct lterm_t* currTerm = 0;
				struct lterm_t* funcTerm = 0;
				helper = chAllocateChainLTerm(UINT64_C(1), &status);
				if (status == NEED_CLEAN)
				{
					success = 0;
					continue;
				}
				//Start construction parenthesis.
				//Start construction fragment term.
				currTerm = chAllocateFragmentLTerm(1, &status);
				if (status == NEED_CLEAN)
				{
					success = 0;
					continue;
				}
				currTerm->fragment->offset = 2;
				currTerm->fragment->length = 6;
				//Adding term to field chain -- Just concat.
				ADD_TO_CHAIN(helper[0].chain, currTerm);
				//Finished construction parenthesis. Save in currTerm.
				currTerm = &helper[0];
				funcRes = (struct func_result_t){.status = OK_RESULT, .fieldChain = currTerm->chain, .callChain = funcCallChain};
			} while (!success); // GC block
			_currFuncCall->entryPoint = -1;
			break; //Successful end of sentence
		} // Pattern case end
		//Sentence: 1, Pattern: 0
		case 1:
		{
			if (!stretching)
			{
				if (_currFuncCall->fieldOfView)
				{
					_currFuncCall->env->fovs[0] = _currFuncCall->fieldOfView;
					_currFuncCall->env->assembledFOVs[0] = gcGetAssembliedChain(_currFuncCall->fieldOfView);
					_currFuncCall->fieldOfView = 0;
				}
			} // !stretching
			currFrag = _currFuncCall->env->assembledFOVs[0]->fragment;
			rightCheckOffset = currFrag->offset + currFrag->length;
			fragmentOffset = currFrag->offset;
			stretchingVarNumber = _currFuncCall->env->stretchVarsNumber[0];
			while (stretchingVarNumber >= 0)
			{
				//From what stretchable variable start?
				switch (stretchingVarNumber)
				{
				case 0:
				//Matching string literal
					if (fragmentOffset + UINT64_C(1) - 1 >= rightCheckOffset)
					{
						stretching = 1;
						stretchingVarNumber = -1;
						break;
					}
					for (i = 0; i < UINT64_C(1); i++)
					{
						if (memMngr.vterms[fragmentOffset + i].tag != V_CHAR_TAG || memMngr.vterms[fragmentOffset + i].ch != memMngr.vterms[UINT64_C(8) + i].ch)
						{
							stretching = 1;
							stretchingVarNumber = -1;
							break;
						}
					}
					if (i < 1) // If check is failed
						break;
					fragmentOffset += UINT64_C(1);
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
				//First pattern of current sentence -> jump to first pattern of next sentence!
				stretching = 0;
				_currFuncCall->entryPoint = 2;
				_currFuncCall->env->stretchVarsNumber[0] = 0;
				for (i = 1; i < 1; ++i )
				{
					_currFuncCall->env->stretchVarsNumber[i] = 0;
					_currFuncCall->env->fovs[i] = 0;
					_currFuncCall->env->assembledFOVs[i] = 0;
				}
				break;
			}
			//Start construction assembly action.
			do { // GC block
				if(!success)
					collectGarbage();
				success = 1;
				allocate_result status = OK;
				struct lterm_t* funcCallChain = 0;
				struct lterm_t* helper = 0;
				struct lterm_t* currTerm = 0;
				struct lterm_t* funcTerm = 0;
				helper = chAllocateChainLTerm(UINT64_C(1), &status);
				if (status == NEED_CLEAN)
				{
					success = 0;
					continue;
				}
				//Start construction parenthesis.
				//Start construction fragment term.
				currTerm = chAllocateFragmentLTerm(1, &status);
				if (status == NEED_CLEAN)
				{
					success = 0;
					continue;
				}
				currTerm->fragment->offset = 9;
				currTerm->fragment->length = 4;
				//Adding term to field chain -- Just concat.
				ADD_TO_CHAIN(helper[0].chain, currTerm);
				//Finished construction parenthesis. Save in currTerm.
				currTerm = &helper[0];
				funcRes = (struct func_result_t){.status = OK_RESULT, .fieldChain = currTerm->chain, .callChain = funcCallChain};
			} while (!success); // GC block
			_currFuncCall->entryPoint = -1;
			break; //Successful end of sentence
		} // Pattern case end
		//Sentence: 2, Pattern: 0
		case 2:
		{
			if (!stretching)
			{
				if (_currFuncCall->fieldOfView)
				{
					_currFuncCall->env->fovs[0] = _currFuncCall->fieldOfView;
					_currFuncCall->env->assembledFOVs[0] = gcGetAssembliedChain(_currFuncCall->fieldOfView);
					_currFuncCall->fieldOfView = 0;
				}
			} // !stretching
			currFrag = _currFuncCall->env->assembledFOVs[0]->fragment;
			rightCheckOffset = currFrag->offset + currFrag->length;
			fragmentOffset = currFrag->offset;
			stretchingVarNumber = _currFuncCall->env->stretchVarsNumber[0];
			while (stretchingVarNumber >= 0)
			{
				//From what stretchable variable start?
				switch (stretchingVarNumber)
				{
				case 0:
				//Matching string literal
					if (fragmentOffset + UINT64_C(1) - 1 >= rightCheckOffset)
					{
						stretching = 1;
						stretchingVarNumber = -1;
						break;
					}
					for (i = 0; i < UINT64_C(1); i++)
					{
						if (memMngr.vterms[fragmentOffset + i].tag != V_CHAR_TAG || memMngr.vterms[fragmentOffset + i].ch != memMngr.vterms[UINT64_C(13) + i].ch)
						{
							stretching = 1;
							stretchingVarNumber = -1;
							break;
						}
					}
					if (i < 1) // If check is failed
						break;
					fragmentOffset += UINT64_C(1);
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
				_currFuncCall->entryPoint = -1;
				break;
			}
			//Start construction assembly action.
			do { // GC block
				if(!success)
					collectGarbage();
				success = 1;
				allocate_result status = OK;
				struct lterm_t* funcCallChain = 0;
				struct lterm_t* helper = 0;
				struct lterm_t* currTerm = 0;
				struct lterm_t* funcTerm = 0;
				helper = chAllocateChainLTerm(UINT64_C(1), &status);
				if (status == NEED_CLEAN)
				{
					success = 0;
					continue;
				}
				//Start construction parenthesis.
				//Start construction fragment term.
				currTerm = chAllocateFragmentLTerm(1, &status);
				if (status == NEED_CLEAN)
				{
					success = 0;
					continue;
				}
				currTerm->fragment->offset = 14;
				currTerm->fragment->length = 5;
				//Adding term to field chain -- Just concat.
				ADD_TO_CHAIN(helper[0].chain, currTerm);
				//Finished construction parenthesis. Save in currTerm.
				currTerm = &helper[0];
				funcRes = (struct func_result_t){.status = OK_RESULT, .fieldChain = currTerm->chain, .callChain = funcCallChain};
			} while (!success); // GC block
			_currFuncCall->entryPoint = -1;
			break; //Successful end of sentence
		} // Pattern case end
		} // Entry point switch end
	} // Main while end
	return funcRes;
} // func AnonymousFunc_2:func_2

int main(int argc, char** argv)
{
	initLiteralData();
	uint64_t vtermOffset = initArgsData(UINT64_C(31), argc, argv);
	initHeaps(vtermOffset);
	mainLoop("Go", func_0);
	return 0;
}
