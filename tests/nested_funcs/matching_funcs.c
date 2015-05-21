#include <stdlib.h>
#include <stdio.h>

#include <vmachine.h>
#include <memory_manager.h>
#include <defines/gc_macros.h>
#include <builtins/builtins.h>
#include <allocators/data_alloc.h>
#include <allocators/vterm_alloc.h>
#include <defines/data_struct_sizes.h>

struct func_result_t func_3(int entryStatus);
struct func_result_t func_0(int entryStatus);
struct func_result_t func_1(int entryStatus);
struct func_result_t func_2(int entryStatus);

void initLiteralData()
{
	_memMngr.vterms[0] = (struct vterm_t){.tag = V_CHAR_TAG, .ch = 0}; // dummy-vterm.
	_memMngr.vterms[1] = (struct vterm_t){.tag = V_IDENT_TAG, .str = allocateVStringLiteral((uint32_t[]){83,117,99,99,101,115,115}, UINT64_C(7))};
	_memMngr.vterms[2] = (struct vterm_t){.tag = V_IDENT_TAG, .str = allocateVStringLiteral((uint32_t[]){80,114,111,117,116}, UINT64_C(5))};
	_memMngr.vterms[3] = (struct vterm_t){.tag = V_IDENT_TAG, .str = allocateVStringLiteral((uint32_t[]){70,117,110,99}, UINT64_C(4))};
	_memMngr.vterms[4] = (struct vterm_t){.tag = V_IDENT_TAG, .str = allocateVStringLiteral((uint32_t[]){65,110,111,110,121,109,70,117,110,99,95,52}, UINT64_C(12))};
	_memMngr.vterms[5] = (struct vterm_t){.tag = V_IDENT_TAG, .str = allocateVStringLiteral((uint32_t[]){65,110,111,110,121,109,70,117,110,99,95,53}, UINT64_C(12))};
} // initLiteralData()

// Func
struct func_result_t func_3(int entryStatus) 
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
		checkAndCleanHeaps(0, ENV_SIZE(2, 2, 1));
		initEnvData(CURR_FUNC_CALL->env, 2, 2, 1);
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
				if (rightBound - fragmentOffset < 2)
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
						//Matching s.1 variable
							if (fragmentOffset >= rightBound || _memMngr.vterms[fragmentOffset].tag == V_BRACKETS_TAG)
							{
								stretching = 1;
								stretchingVarNumber = -1;
								break;
							}
							else
							{
								(CURR_FUNC_CALL->env->locals + 0)->offset = fragmentOffset;
								(CURR_FUNC_CALL->env->locals + 0)->length = 1;
								fragmentOffset++;
							}
						//Matching s.2 variable
							if (fragmentOffset >= rightBound || _memMngr.vterms[fragmentOffset].tag == V_BRACKETS_TAG)
							{
								stretching = 1;
								stretchingVarNumber = -1;
								break;
							}
							else
							{
								(CURR_FUNC_CALL->env->locals + 1)->offset = fragmentOffset;
								(CURR_FUNC_CALL->env->locals + 1)->length = 1;
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
					//First pattern of last sentence -> nothing to stretch -> fail!
					funcRes = (struct func_result_t){.status = FAIL_RESULT, .fieldChain = 0, .callChain = 0};
					CURR_FUNC_CALL->entryPoint = -1;
					break;
				}
			}

			//Sentence: 0, Action index: 1, Type: ':'
			case 1:
			{
				if (!stretching)
				{
					CURR_FUNC_CALL->env->assembled[1] = CURR_FUNC_CALL->env->assembled[0];
				} // !stretching
				currFrag = VTERM_BRACKETS(CURR_FUNC_CALL->env->assembled[1]);
				rightBound = RIGHT_BOUND(CURR_FUNC_CALL->env->assembled[1]);
					fragmentOffset = currFrag->offset;
				CURR_FUNC_CALL->env->bracketsOffset[0] = CURR_FUNC_CALL->env->assembled[1];
					stretchingVarNumber = CURR_FUNC_CALL->env->stretchVarsNumber[1];
				if (rightBound - fragmentOffset < 2)
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
						//Matching s.2 variable
							if (fragmentOffset >= rightBound 
							|| (_memMngr.vterms[fragmentOffset].tag == V_BRACKETS_TAG) || (!eqSymbol(fragmentOffset, (CURR_FUNC_CALL->env->locals + 1)->offset)))
							{
								stretching = 1;
								stretchingVarNumber = -1;
								break;
							}
							fragmentOffset++;
						//Matching s.1 variable
							if (fragmentOffset >= rightBound 
							|| (_memMngr.vterms[fragmentOffset].tag == V_BRACKETS_TAG) || (!eqSymbol(fragmentOffset, (CURR_FUNC_CALL->env->locals + 0)->offset)))
							{
								stretching = 1;
								stretchingVarNumber = -1;
								break;
							}
							fragmentOffset++;
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
					//Jump to previouse pattern of same sentence!
					CURR_FUNC_CALL->entryPoint = 0;
					break;
				}
			}

			//Sentence: 0, Action index: 2, Type: '='
			case 2:
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
				CHECK_ALLOCATION_CONTINUE(helper, chAllocateChainKeeperLTerm(UINT64_C(1), &status), status);
				//Start construction parenthesis.
				//Start construction fragment term.
				CHECK_ALLOCATION_CONTINUE(currTerm, chAllocateFragmentLTerm(1, &status), status);
				currTerm->fragment->offset = 1;
				currTerm->fragment->length = 1;
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
} // Func

// Go
struct func_result_t func_0(int entryStatus) 
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
		checkAndCleanHeaps(0, ENV_SIZE(0, 1, 1));
		initEnvData(CURR_FUNC_CALL->env, 0, 1, 1);
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
				if (rightBound != fragmentOffset)
				{
					stretching = 1;
					stretchingVarNumber = -1;
				}
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
				CHECK_ALLOCATION_CONTINUE(helper, chAllocateChainKeeperLTerm(UINT64_C(3), &status), status);
				//Start construction parenthesis.
				//Start construction func call.
				//Start construction parenthesis.
				//Start construction func term.
				CHECK_ALLOCATION_CONTINUE(currTerm, chAllocateFragmentLTerm(1, &status), status);
				CHECK_ALLOCATION_CONTINUE(currTerm->fragment->offset, chAllocateClosureVTerm(&status), status);
				CHECK_ALLOCATION_CONTINUE(_memMngr.vterms[currTerm->fragment->offset].closure, chAllocateClosureStruct(Prout, 0, _memMngr.vterms[2].str, 0, &status), status);
				currTerm->fragment->length = 1;
				//Finish construction func term.
				//Adding term to field chain -- Just concat.
				ADD_TO_CHAIN(helper[1].chain, currTerm);
				//Start construction func call.
				//Start construction parenthesis.
				//Start construction func term.
				CHECK_ALLOCATION_CONTINUE(currTerm, chAllocateFragmentLTerm(1, &status), status);
				CHECK_ALLOCATION_CONTINUE(currTerm->fragment->offset, chAllocateClosureVTerm(&status), status);
				CHECK_ALLOCATION_CONTINUE(_memMngr.vterms[currTerm->fragment->offset].closure, chAllocateClosureStruct(func_3, 0, _memMngr.vterms[3].str, 0, &status), status);
				currTerm->fragment->length = 1;
				//Finish construction func term.
				//Adding term to field chain -- Just concat.
				ADD_TO_CHAIN(helper[2].chain, currTerm);
				//Start construction func term.
				CHECK_ALLOCATION_CONTINUE(currTerm, chAllocateFragmentLTerm(1, &status), status);
				CHECK_ALLOCATION_CONTINUE(currTerm->fragment->offset, chAllocateClosureVTerm(&status), status);
				CHECK_ALLOCATION_CONTINUE(_memMngr.vterms[currTerm->fragment->offset].closure, chAllocateClosureStruct(func_1, 0, _memMngr.vterms[4].str, 0, &status), status);
				currTerm->fragment->length = 1;
				//Finish construction func term.
				//Adding term to field chain -- Just concat.
				ADD_TO_CHAIN(helper[2].chain, currTerm);
				//Start construction func term.
				CHECK_ALLOCATION_CONTINUE(currTerm, chAllocateFragmentLTerm(1, &status), status);
				CHECK_ALLOCATION_CONTINUE(currTerm->fragment->offset, chAllocateClosureVTerm(&status), status);
				CHECK_ALLOCATION_CONTINUE(_memMngr.vterms[currTerm->fragment->offset].closure, chAllocateClosureStruct(func_2, 0, _memMngr.vterms[5].str, 0, &status), status);
				currTerm->fragment->length = 1;
				//Finish construction func term.
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
				CHECK_ALLOCATION_CONTINUE(funcTerm, chAllocateFuncCallLTerm(&status), status);
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
				CURR_FUNC_CALL->env->workFieldOfView = currTerm->chain;
				funcRes = (struct func_result_t){.status = OK_RESULT, .fieldChain = currTerm->chain, .callChain = funcCallChain};
			} while (status != GC_OK); // GC block
			}

			CURR_FUNC_CALL->entryPoint = -1;
			break; //Successful end of sentence
		} // Entry point switch end
	} // Main while end
	return funcRes;
} // Go

// AnonymFunc_4
struct func_result_t func_1(int entryStatus) 
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
		checkAndCleanHeaps(0, ENV_SIZE(0, 1, 1));
		initEnvData(CURR_FUNC_CALL->env, 0, 1, 1);
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
				if (rightBound != fragmentOffset)
				{
					stretching = 1;
					stretchingVarNumber = -1;
				}
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
				funcRes = (struct func_result_t){.status = OK_RESULT, .fieldChain = 0, .callChain = 0};
			}

			CURR_FUNC_CALL->entryPoint = -1;
			break; //Successful end of sentence
		} // Entry point switch end
	} // Main while end
	return funcRes;
} // AnonymFunc_4

// AnonymFunc_5
struct func_result_t func_2(int entryStatus) 
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
		checkAndCleanHeaps(0, ENV_SIZE(0, 1, 1));
		initEnvData(CURR_FUNC_CALL->env, 0, 1, 1);
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
				if (rightBound != fragmentOffset)
				{
					stretching = 1;
					stretchingVarNumber = -1;
				}
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
				funcRes = (struct func_result_t){.status = OK_RESULT, .fieldChain = 0, .callChain = 0};
			}

			CURR_FUNC_CALL->entryPoint = -1;
			break; //Successful end of sentence
		} // Entry point switch end
	} // Main while end
	return funcRes;
} // AnonymFunc_5

int main(int argc, char** argv)
{
	initBuiltins();
	initAllocator(getHeapSizeFromCmdArgs(argc, argv));
	initLiteralData();
	uint64_t vtermOffset = initArgsData(UINT64_C(6), argc, argv);
	initHeaps(vtermOffset);
	mainLoop("Go", func_0);
	return 0;
}
