#include <stdlib.h>
#include <stdio.h>

#include <vmachine.h>
#include <memory_manager.h>
#include <defines/gc_macros.h>
#include <builtins/builtins.h>
#include <allocators/data_alloc.h>
#include <allocators/vterm_alloc.h>
#include <defines/data_struct_sizes.h>

struct func_result_t func_0(int entryStatus);

void initLiteralData()
{
	_memMngr.vterms[0] = (struct vterm_t){.tag = V_CHAR_TAG, .ch = 0}; // dummy-vterm.
	_memMngr.vterms[1] = (struct vterm_t){.tag = V_IDENT_TAG, .str = allocateVStringLiteral((uint32_t[]){68,105,118}, UINT64_C(3))};
	_memMngr.vterms[2] = (struct vterm_t){.tag = V_IDENT_TAG, .str = allocateVStringLiteral((uint32_t[]){77,111,100}, UINT64_C(3))};
} // initLiteralData()

// Divmod
struct func_result_t Divmod(int entryStatus) 
{
	struct func_result_t funcRes;
	struct fragment_t* currFrag = 0;
	uint64_t fragmentOffset = 0;
	uint64_t rightBound = 0;
	int stretchingVarNumber = 0;
	int stretching = 0;
	int status = GC_OK;
	int prevStatus = GC_OK;
	int i = 0;
	int j = 0;
	if (entryStatus == FIRST_CALL)
	{
		checkAndCleanHeaps(0, ENV_SIZE(2, 1, 2));
		initEnvData(CURR_FUNC_CALL->env, 2, 1, 2);
	}
	else if (entryStatus == ROLL_BACK)
		stretching = 1;
	while(CURR_FUNC_CALL->entryPoint >= 0)
	{
		switch (CURR_FUNC_CALL->entryPoint)
		{
			//Sentence: 0, Action index: 0, Type: ':'
			case 0:
			{
				if (!stretching)
				{
					ASSEMBLY_FIELD(0, CURR_FUNC_CALL->fieldOfView);
				} // !stretching
				currFrag = VTERM_BRACKETS(CURR_FUNC_CALL->env->assembled[0]);
				rightBound = RIGHT_BOUND(CURR_FUNC_CALL->env->assembled[0]);
					fragmentOffset = currFrag->offset;
				CURR_FUNC_CALL->env->bracketsOffset[0] = CURR_FUNC_CALL->env->assembled[0];
					stretchingVarNumber = CURR_FUNC_CALL->env->stretchVarsNumber[0];
				if (rightBound - fragmentOffset < 1)
				{
					stretching = 1;
					stretchingVarNumber = -1;
				}
				else
					while (stretchingVarNumber >= 0)
					{
						//From what stretchable variable start?
						switch (stretchingVarNumber)
						{
						case 0:
							//Check ().
							if (fragmentOffset >= rightBound || _memMngr.vterms[fragmentOffset].tag != V_BRACKETS_TAG)
							{
								stretching = 1;
								stretchingVarNumber = -1;
								break;
							}
							CURR_FUNC_CALL->env->bracketsOffset[1] = fragmentOffset;
							rightBound = RIGHT_BOUND(fragmentOffset);
							fragmentOffset = VTERM_BRACKETS(fragmentOffset)->offset;
							if (rightBound - fragmentOffset < 0)
							{
								stretching = 1;
								stretchingVarNumber = -1;
								break;
							}
							//Start check in () terms.
						//Matching e.1 variable
						case 1:
							if (!stretching) // Just init values
							{
								(CURR_FUNC_CALL->env->locals + 0)->offset = fragmentOffset;
								(CURR_FUNC_CALL->env->locals + 0)->length = 0;
							}
							else // stretching
							{
								stretching = 0;
								CURR_FUNC_CALL->env->stretchVarsNumber[0] = 1;
								rightBound = RIGHT_BOUND(CURR_FUNC_CALL->env->bracketsOffset[1]);
								//Restore last offset at this point
								fragmentOffset = (CURR_FUNC_CALL->env->locals + 0)->offset +  (CURR_FUNC_CALL->env->locals + 0)->length;
								if (fragmentOffset >= rightBound)
								{
									stretching = 1;
									stretchingVarNumber = -1;
									break;
								}
								else
								{
									fragmentOffset++;
									(CURR_FUNC_CALL->env->locals + 0)->length++;
								}
							}
							if (fragmentOffset != rightBound)
							{
								stretching = 1;
								stretchingVarNumber = 1;
								break;
							}
							rightBound = RIGHT_BOUND(CURR_FUNC_CALL->env->bracketsOffset[0]);
							fragmentOffset = CURR_FUNC_CALL->env->bracketsOffset[1] + 1;
							//End check in () terms.
						//Matching e.2 variable
						case 2:
							if (!stretching) // Just init values
							{
								(CURR_FUNC_CALL->env->locals + 1)->offset = fragmentOffset;
								(CURR_FUNC_CALL->env->locals + 1)->length = 0;
							}
							else // stretching
							{
								stretching = 0;
								CURR_FUNC_CALL->env->stretchVarsNumber[0] = 2;
								rightBound = RIGHT_BOUND(CURR_FUNC_CALL->env->bracketsOffset[0]);
								//Restore last offset at this point
								fragmentOffset = (CURR_FUNC_CALL->env->locals + 1)->offset +  (CURR_FUNC_CALL->env->locals + 1)->length;
								if (fragmentOffset >= rightBound)
								{
									stretching = 1;
									stretchingVarNumber = 1;
									break;
								}
								else
								{
									fragmentOffset++;
									(CURR_FUNC_CALL->env->locals + 1)->length++;
								}
							}
						} //pattern switch

						if (!stretching)
						{
							if (fragmentOffset - currFrag->offset < currFrag->length)
							{
								stretching = 1;
								stretchingVarNumber = 2;
							}
							else
								break; // Success!
						}
					} // Pattern while

				if (stretchingVarNumber < 0)
				{
					//First pattern of last sentence -> nothing to stretch -> fail!
					funcRes = (struct func_result_t){.status = FAIL_RESULT, .fieldChain = 0, .callChain = 0};
					CURR_FUNC_CALL->entryPoint = -1;
					break;
				}
			}

			//Sentence: 0, Action index: 1, Type: '='
			case 1:
			{
			do { // GC block
				if(prevStatus == GC_NEED_CLEAN)
					PRINT_AND_EXIT(GC_MEMORY_OVERFLOW_MSG);
				if(status == GC_NEED_CLEAN)
				{
					collectGarbage();
					prevStatus = GC_NEED_CLEAN;
					status = GC_OK;
				}
				struct lterm_t* funcCallChain = 0;
				struct lterm_t* helper = 0;
				struct lterm_t* currTerm = 0;
				struct lterm_t* funcTerm = 0;
				CHECK_ALLOCATION_CONTINUE(helper, chAllocateChainKeeperLTerm(UINT64_C(7), &status), status);
				//Start construction parenthesis.
				//Start construction parenthesis.
				//Start construction func call.
				//Start construction parenthesis.
				//Start construction func term.
				CHECK_ALLOCATION_CONTINUE(currTerm, chAllocateFragmentLTerm(1, &status), status);
				CHECK_ALLOCATION_CONTINUE(currTerm->fragment->offset, chAllocateClosureVTerm(&status), status);
				CHECK_ALLOCATION_CONTINUE(_memMngr.vterms[currTerm->fragment->offset].closure, chAllocateClosureStruct(Div, 0, _memMngr.vterms[1].str, 0, &status), status);
				currTerm->fragment->length = 1;
				//Finish construction func term.
				//Adding term to field chain -- Just concat.
				ADD_TO_CHAIN(helper[2].chain, currTerm);
				//Start construction parenthesis.
				CHECK_ALLOCATION_CONTINUE(currTerm, chAllocateFragmentLTerm(1, &status), status);
				currTerm->fragment->offset = (CURR_FUNC_CALL->env->locals + 0)->offset;
				currTerm->fragment->length = (CURR_FUNC_CALL->env->locals + 0)->length;
				//Adding term to field chain -- Just concat.
				ADD_TO_CHAIN(helper[3].chain, currTerm);
				//Finished construction parenthesis. Save in currTerm.
				currTerm = &helper[3];
				//Adding term to field chain -- Just concat.
				ADD_TO_CHAIN(helper[2].chain, currTerm);
				CHECK_ALLOCATION_CONTINUE(currTerm, chAllocateFragmentLTerm(1, &status), status);
				currTerm->fragment->offset = (CURR_FUNC_CALL->env->locals + 1)->offset;
				currTerm->fragment->length = (CURR_FUNC_CALL->env->locals + 1)->length;
				//Adding term to field chain -- Just concat.
				ADD_TO_CHAIN(helper[2].chain, currTerm);
				//Finished construction parenthesis. Save in currTerm.
				currTerm = &helper[2];
				CHECK_ALLOCATION_CONTINUE(funcTerm, chAllocateFuncCallLTerm(&status), status);
				funcTerm->funcCall->failEntryPoint = -1;
				funcTerm->funcCall->fieldOfView = currTerm->chain;
				//Finished construction func call
				//First call in call chain -- Initialization.
				CHECK_ALLOCATION_CONTINUE(funcCallChain, chAllocateSimpleChainLTerm(&status), status);
				funcCallChain->next = funcTerm;
				funcCallChain->prev = funcTerm;
				currTerm = funcTerm;
				//Adding term to field chain -- Just concat.
				ADD_TO_CHAIN(helper[1].chain, currTerm);
				//Finished construction parenthesis. Save in currTerm.
				currTerm = &helper[1];
				//Adding term to field chain -- Just concat.
				ADD_TO_CHAIN(helper[0].chain, currTerm);
				//Start construction parenthesis.
				//Start construction func call.
				//Start construction parenthesis.
				//Start construction func term.
				CHECK_ALLOCATION_CONTINUE(currTerm, chAllocateFragmentLTerm(1, &status), status);
				CHECK_ALLOCATION_CONTINUE(currTerm->fragment->offset, chAllocateClosureVTerm(&status), status);
				CHECK_ALLOCATION_CONTINUE(_memMngr.vterms[currTerm->fragment->offset].closure, chAllocateClosureStruct(Mod, 0, _memMngr.vterms[2].str, 0, &status), status);
				currTerm->fragment->length = 1;
				//Finish construction func term.
				//Adding term to field chain -- Just concat.
				ADD_TO_CHAIN(helper[5].chain, currTerm);
				//Start construction parenthesis.
				CHECK_ALLOCATION_CONTINUE(currTerm, chAllocateFragmentLTerm(1, &status), status);
				currTerm->fragment->offset = (CURR_FUNC_CALL->env->locals + 0)->offset;
				currTerm->fragment->length = (CURR_FUNC_CALL->env->locals + 0)->length;
				//Adding term to field chain -- Just concat.
				ADD_TO_CHAIN(helper[6].chain, currTerm);
				//Finished construction parenthesis. Save in currTerm.
				currTerm = &helper[6];
				//Adding term to field chain -- Just concat.
				ADD_TO_CHAIN(helper[5].chain, currTerm);
				CHECK_ALLOCATION_CONTINUE(currTerm, chAllocateFragmentLTerm(1, &status), status);
				currTerm->fragment->offset = (CURR_FUNC_CALL->env->locals + 1)->offset;
				currTerm->fragment->length = (CURR_FUNC_CALL->env->locals + 1)->length;
				//Adding term to field chain -- Just concat.
				ADD_TO_CHAIN(helper[5].chain, currTerm);
				//Finished construction parenthesis. Save in currTerm.
				currTerm = &helper[5];
				CHECK_ALLOCATION_CONTINUE(funcTerm, chAllocateFuncCallLTerm(&status), status);
				funcTerm->funcCall->failEntryPoint = -1;
				funcTerm->funcCall->fieldOfView = currTerm->chain;
				//Finished construction func call
				//Adding call to call chain -- Just concat.
				funcCallChain->prev->funcCall->next = funcTerm;
				funcCallChain->prev = funcTerm;
				currTerm = funcTerm;
				//Adding term to field chain -- Just concat.
				ADD_TO_CHAIN(helper[4].chain, currTerm);
				//Finished construction parenthesis. Save in currTerm.
				currTerm = &helper[4];
				//Adding term to field chain -- Just concat.
				ADD_TO_CHAIN(helper[0].chain, currTerm);
				//Finished construction parenthesis. Save in currTerm.
				currTerm = &helper[0];
				CURR_FUNC_CALL->env->workFieldOfView = currTerm->chain;
				funcRes = (struct func_result_t){.status = OK_RESULT, .fieldChain = currTerm->chain, .callChain = funcCallChain};
			} while (status != GC_OK); // GC block
			}

			CURR_FUNC_CALL->entryPoint = -1;
			break; //Successful end of sentence
		} // Entry point switch end
	} // Main while end
	return funcRes;
} // Divmod

int main(int argc, char** argv)
{
	return 0;
}
