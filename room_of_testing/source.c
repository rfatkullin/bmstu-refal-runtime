// file:source.ref

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
struct func_result_t func_1(int entryStatus);
struct func_result_t func_2(int entryStatus);
struct func_result_t func_3(int entryStatus);
struct func_result_t func_4(int entryStatus);
struct func_result_t func_5(int entryStatus);
struct func_result_t func_6(int entryStatus);

void initLiteralData()
{
	_memMngr.vterms[0] = (struct vterm_t){.tag = V_CHAR_TAG, .ch = 0}; // dummy-vterm.
	_memMngr.vterms[1] = (struct vterm_t){.tag = V_IDENT_TAG, .str = allocateVStringLiteral((uint32_t[]){80,114,111,117,116}, UINT64_C(5))};
	_memMngr.vterms[2] = (struct vterm_t){.tag = V_IDENT_TAG, .str = allocateVStringLiteral((uint32_t[]){70,117,110,99,49}, UINT64_C(5))};
	_memMngr.vterms[3] = (struct vterm_t){.tag = V_IDENT_TAG, .str = allocateVStringLiteral((uint32_t[]){97}, UINT64_C(1))};
	_memMngr.vterms[4] = (struct vterm_t){.tag = V_IDENT_TAG, .str = allocateVStringLiteral((uint32_t[]){98}, UINT64_C(1))};
	_memMngr.vterms[5] = (struct vterm_t){.tag = V_IDENT_TAG, .str = allocateVStringLiteral((uint32_t[]){99}, UINT64_C(1))};
	_memMngr.vterms[6] = (struct vterm_t){.tag = V_IDENT_TAG, .str = allocateVStringLiteral((uint32_t[]){100}, UINT64_C(1))};
	_memMngr.vterms[7] = (struct vterm_t){.tag = V_CHAR_TAG, .ch = 10};
	_memMngr.vterms[8] = (struct vterm_t){.tag = V_IDENT_TAG, .str = allocateVStringLiteral((uint32_t[]){70,117,110,99,50}, UINT64_C(5))};
	_memMngr.vterms[9] = (struct vterm_t){.tag = V_IDENT_TAG, .str = allocateVStringLiteral((uint32_t[]){97}, UINT64_C(1))};
	_memMngr.vterms[10] = (struct vterm_t){.tag = V_IDENT_TAG, .str = allocateVStringLiteral((uint32_t[]){98}, UINT64_C(1))};
	_memMngr.vterms[11] = (struct vterm_t){.tag = V_IDENT_TAG, .str = allocateVStringLiteral((uint32_t[]){99}, UINT64_C(1))};
	_memMngr.vterms[12] = (struct vterm_t){.tag = V_IDENT_TAG, .str = allocateVStringLiteral((uint32_t[]){100}, UINT64_C(1))};
	_memMngr.vterms[13] = (struct vterm_t){.tag = V_CHAR_TAG, .ch = 10};
	_memMngr.vterms[14] = (struct vterm_t){.tag = V_IDENT_TAG, .str = allocateVStringLiteral((uint32_t[]){70,117,110,99,51}, UINT64_C(5))};
	_memMngr.vterms[15] = (struct vterm_t){.tag = V_IDENT_TAG, .str = allocateVStringLiteral((uint32_t[]){97}, UINT64_C(1))};
	_memMngr.vterms[16] = (struct vterm_t){.tag = V_IDENT_TAG, .str = allocateVStringLiteral((uint32_t[]){98}, UINT64_C(1))};
	_memMngr.vterms[17] = (struct vterm_t){.tag = V_IDENT_TAG, .str = allocateVStringLiteral((uint32_t[]){99}, UINT64_C(1))};
	_memMngr.vterms[18] = (struct vterm_t){.tag = V_IDENT_TAG, .str = allocateVStringLiteral((uint32_t[]){100}, UINT64_C(1))};
	_memMngr.vterms[19] = (struct vterm_t){.tag = V_CHAR_TAG, .ch = 10};
	_memMngr.vterms[20] = (struct vterm_t){.tag = V_IDENT_TAG, .str = allocateVStringLiteral((uint32_t[]){70,117,110,99,52}, UINT64_C(5))};
	_memMngr.vterms[21] = (struct vterm_t){.tag = V_IDENT_TAG, .str = allocateVStringLiteral((uint32_t[]){97}, UINT64_C(1))};
	_memMngr.vterms[22] = (struct vterm_t){.tag = V_IDENT_TAG, .str = allocateVStringLiteral((uint32_t[]){98}, UINT64_C(1))};
	_memMngr.vterms[23] = (struct vterm_t){.tag = V_IDENT_TAG, .str = allocateVStringLiteral((uint32_t[]){99}, UINT64_C(1))};
	_memMngr.vterms[24] = (struct vterm_t){.tag = V_IDENT_TAG, .str = allocateVStringLiteral((uint32_t[]){100}, UINT64_C(1))};
	_memMngr.vterms[25] = (struct vterm_t){.tag = V_CHAR_TAG, .ch = 10};
	_memMngr.vterms[26] = (struct vterm_t){.tag = V_IDENT_TAG, .str = allocateVStringLiteral((uint32_t[]){70,117,110,99,53}, UINT64_C(5))};
	_memMngr.vterms[27] = (struct vterm_t){.tag = V_IDENT_TAG, .str = allocateVStringLiteral((uint32_t[]){97}, UINT64_C(1))};
	_memMngr.vterms[28] = (struct vterm_t){.tag = V_IDENT_TAG, .str = allocateVStringLiteral((uint32_t[]){98}, UINT64_C(1))};
	_memMngr.vterms[29] = (struct vterm_t){.tag = V_IDENT_TAG, .str = allocateVStringLiteral((uint32_t[]){99}, UINT64_C(1))};
	_memMngr.vterms[30] = (struct vterm_t){.tag = V_IDENT_TAG, .str = allocateVStringLiteral((uint32_t[]){100}, UINT64_C(1))};
	_memMngr.vterms[31] = (struct vterm_t){.tag = V_CHAR_TAG, .ch = 10};
	_memMngr.vterms[32] = (struct vterm_t){.tag = V_IDENT_TAG, .str = allocateVStringLiteral((uint32_t[]){70,117,110,99,54}, UINT64_C(5))};
	_memMngr.vterms[33] = (struct vterm_t){.tag = V_IDENT_TAG, .str = allocateVStringLiteral((uint32_t[]){97}, UINT64_C(1))};
	_memMngr.vterms[34] = (struct vterm_t){.tag = V_IDENT_TAG, .str = allocateVStringLiteral((uint32_t[]){98}, UINT64_C(1))};
	_memMngr.vterms[35] = (struct vterm_t){.tag = V_IDENT_TAG, .str = allocateVStringLiteral((uint32_t[]){99}, UINT64_C(1))};
	_memMngr.vterms[36] = (struct vterm_t){.tag = V_IDENT_TAG, .str = allocateVStringLiteral((uint32_t[]){100}, UINT64_C(1))};

} // initLiteralData()

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
		checkAndCleanHeaps(0, ENV_SIZE(1, 1, 1));
		initEnvData(CURR_FUNC_CALL->env, 1, 1, 1);
	}
	else if (entryStatus == ROLL_BACK)
		stretching = 1;
	while(CURR_FUNC_CALL->entryPoint >= 0)
	{
		switch (CURR_FUNC_CALL->entryPoint)
		{
		//Sentence: 0, Pattern: 0
		case 0:
		{
			if (!stretching)
			{
				ASSEMBLY_FIELD(0, CURR_FUNC_CALL->fieldOfView);
			} // !stretching
			currFrag = VTERM_BRACKETS(CURR_FUNC_CALL->env->assembled[0]);
			rightBound = RIGHT_BOUND(CURR_FUNC_CALL->env->assembled[0]);
			CURR_FUNC_CALL->env->bracketsOffset[0] = CURR_FUNC_CALL->env->assembled[0];
			fragmentOffset = currFrag->offset;
			stretchingVarNumber = CURR_FUNC_CALL->env->stretchVarsNumber[0];
			while (stretchingVarNumber >= 0)
			{
				//From what stretchable variable start?
				switch (stretchingVarNumber)
				{
				//Matching e.1 variable
				case 0:
					if (!stretching) // Just init values
					{
						(CURR_FUNC_CALL->env->locals + 0)->offset = fragmentOffset;
						(CURR_FUNC_CALL->env->locals + 0)->length = 0;
					}
					else // stretching
					{
						stretching = 0;
						CURR_FUNC_CALL->env->stretchVarsNumber[0] = 0;
						rightBound = RIGHT_BOUND(CURR_FUNC_CALL->env->bracketsOffset[0]);
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
				CURR_FUNC_CALL->entryPoint = -1;
				break;
			}
			//Start construction assembly action.
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
				helper = chAllocateChainKeeperLTerm(UINT64_C(1), &status);
				if (status == GC_NEED_CLEAN)
				{
					continue;
				}
				//Start construction parenthesis.
				currTerm = chAllocateFragmentLTerm(1, &status);
				if (status == GC_NEED_CLEAN)
				{
					continue;
				}
				currTerm->fragment->offset = (CURR_FUNC_CALL->env->locals + 0)->offset;
				currTerm->fragment->length = (CURR_FUNC_CALL->env->locals + 0)->length;
				//Adding term to field chain -- Just concat.
				ADD_TO_CHAIN(helper[0].chain, currTerm);
				//Finished construction parenthesis. Save in currTerm.
				currTerm = &helper[0];
				CURR_FUNC_CALL->env->workFieldOfView = currTerm->chain;
				funcRes = (struct func_result_t){.status = OK_RESULT, .fieldChain = currTerm->chain, .callChain = funcCallChain};
			} while (status != GC_OK); // GC block
			CURR_FUNC_CALL->entryPoint = -1;
			break; //Successful end of sentence
		} // Pattern case end
		} // Entry point switch end
	} // Main while end
	return funcRes;
} // func Func1:func_1

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
		checkAndCleanHeaps(0, ENV_SIZE(2, 1, 1));
		initEnvData(CURR_FUNC_CALL->env, 2, 1, 1);
	}
	else if (entryStatus == ROLL_BACK)
		stretching = 1;
	while(CURR_FUNC_CALL->entryPoint >= 0)
	{
		switch (CURR_FUNC_CALL->entryPoint)
		{
		//Sentence: 0, Pattern: 0
		case 0:
		{
			if (!stretching)
			{
				ASSEMBLY_FIELD(0, CURR_FUNC_CALL->fieldOfView);
			} // !stretching
			currFrag = VTERM_BRACKETS(CURR_FUNC_CALL->env->assembled[0]);
			rightBound = RIGHT_BOUND(CURR_FUNC_CALL->env->assembled[0]);
			CURR_FUNC_CALL->env->bracketsOffset[0] = CURR_FUNC_CALL->env->assembled[0];
			fragmentOffset = currFrag->offset;
			stretchingVarNumber = CURR_FUNC_CALL->env->stretchVarsNumber[0];
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
				//Matching e.1 variable
				case 1:
					if (!stretching) // Just init values
					{
						(CURR_FUNC_CALL->env->locals + 1)->offset = fragmentOffset;
						(CURR_FUNC_CALL->env->locals + 1)->length = 0;
					}
					else // stretching
					{
						stretching = 0;
						CURR_FUNC_CALL->env->stretchVarsNumber[0] = 1;
						rightBound = RIGHT_BOUND(CURR_FUNC_CALL->env->bracketsOffset[0]);
						//Restore last offset at this point
						fragmentOffset = (CURR_FUNC_CALL->env->locals + 1)->offset +  (CURR_FUNC_CALL->env->locals + 1)->length;
						if (fragmentOffset >= rightBound)
						{
							stretching = 1;
							stretchingVarNumber = -1;
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
						stretchingVarNumber = 1;
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
			//Start construction assembly action.
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
				helper = chAllocateChainKeeperLTerm(UINT64_C(1), &status);
				if (status == GC_NEED_CLEAN)
				{
					continue;
				}
				//Start construction parenthesis.
				currTerm = chAllocateFragmentLTerm(1, &status);
				if (status == GC_NEED_CLEAN)
				{
					continue;
				}
				currTerm->fragment->offset = (CURR_FUNC_CALL->env->locals + 1)->offset;
				currTerm->fragment->length = (CURR_FUNC_CALL->env->locals + 1)->length;
				//Adding term to field chain -- Just concat.
				ADD_TO_CHAIN(helper[0].chain, currTerm);
				//Finished construction parenthesis. Save in currTerm.
				currTerm = &helper[0];
				CURR_FUNC_CALL->env->workFieldOfView = currTerm->chain;
				funcRes = (struct func_result_t){.status = OK_RESULT, .fieldChain = currTerm->chain, .callChain = funcCallChain};
			} while (status != GC_OK); // GC block
			CURR_FUNC_CALL->entryPoint = -1;
			break; //Successful end of sentence
		} // Pattern case end
		} // Entry point switch end
	} // Main while end
	return funcRes;
} // func Func2:func_2

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
		checkAndCleanHeaps(0, ENV_SIZE(3, 1, 1));
		initEnvData(CURR_FUNC_CALL->env, 3, 1, 1);
	}
	else if (entryStatus == ROLL_BACK)
		stretching = 1;
	while(CURR_FUNC_CALL->entryPoint >= 0)
	{
		switch (CURR_FUNC_CALL->entryPoint)
		{
		//Sentence: 0, Pattern: 0
		case 0:
		{
			if (!stretching)
			{
				ASSEMBLY_FIELD(0, CURR_FUNC_CALL->fieldOfView);
			} // !stretching
			currFrag = VTERM_BRACKETS(CURR_FUNC_CALL->env->assembled[0]);
			rightBound = RIGHT_BOUND(CURR_FUNC_CALL->env->assembled[0]);
			CURR_FUNC_CALL->env->bracketsOffset[0] = CURR_FUNC_CALL->env->assembled[0];
			fragmentOffset = currFrag->offset;
			stretchingVarNumber = CURR_FUNC_CALL->env->stretchVarsNumber[0];
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
				//Matching e.1 variable
				case 1:
					if (!stretching) // Just init values
					{
						(CURR_FUNC_CALL->env->locals + 2)->offset = fragmentOffset;
						(CURR_FUNC_CALL->env->locals + 2)->length = 0;
					}
					else // stretching
					{
						stretching = 0;
						CURR_FUNC_CALL->env->stretchVarsNumber[0] = 1;
						rightBound = RIGHT_BOUND(CURR_FUNC_CALL->env->bracketsOffset[0]);
						//Restore last offset at this point
						fragmentOffset = (CURR_FUNC_CALL->env->locals + 2)->offset +  (CURR_FUNC_CALL->env->locals + 2)->length;
						if (fragmentOffset >= rightBound)
						{
							stretching = 1;
							stretchingVarNumber = -1;
							break;
						}
						else
						{
							fragmentOffset++;
							(CURR_FUNC_CALL->env->locals + 2)->length++;
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
				//First pattern of last sentence -> nothing to stretch -> fail!
				funcRes = (struct func_result_t){.status = FAIL_RESULT, .fieldChain = 0, .callChain = 0};
				CURR_FUNC_CALL->entryPoint = -1;
				break;
			}
			//Start construction assembly action.
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
				helper = chAllocateChainKeeperLTerm(UINT64_C(1), &status);
				if (status == GC_NEED_CLEAN)
				{
					continue;
				}
				//Start construction parenthesis.
				currTerm = chAllocateFragmentLTerm(1, &status);
				if (status == GC_NEED_CLEAN)
				{
					continue;
				}
				currTerm->fragment->offset = (CURR_FUNC_CALL->env->locals + 2)->offset;
				currTerm->fragment->length = (CURR_FUNC_CALL->env->locals + 2)->length;
				//Adding term to field chain -- Just concat.
				ADD_TO_CHAIN(helper[0].chain, currTerm);
				//Finished construction parenthesis. Save in currTerm.
				currTerm = &helper[0];
				CURR_FUNC_CALL->env->workFieldOfView = currTerm->chain;
				funcRes = (struct func_result_t){.status = OK_RESULT, .fieldChain = currTerm->chain, .callChain = funcCallChain};
			} while (status != GC_OK); // GC block
			CURR_FUNC_CALL->entryPoint = -1;
			break; //Successful end of sentence
		} // Pattern case end
		} // Entry point switch end
	} // Main while end
	return funcRes;
} // func Func3:func_3

struct func_result_t func_4(int entryStatus) 
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
		checkAndCleanHeaps(0, ENV_SIZE(5, 1, 1));
		initEnvData(CURR_FUNC_CALL->env, 5, 1, 1);
	}
	else if (entryStatus == ROLL_BACK)
		stretching = 1;
	while(CURR_FUNC_CALL->entryPoint >= 0)
	{
		switch (CURR_FUNC_CALL->entryPoint)
		{
		//Sentence: 0, Pattern: 0
		case 0:
		{
			if (!stretching)
			{
				ASSEMBLY_FIELD(0, CURR_FUNC_CALL->fieldOfView);
			} // !stretching
			currFrag = VTERM_BRACKETS(CURR_FUNC_CALL->env->assembled[0]);
			rightBound = RIGHT_BOUND(CURR_FUNC_CALL->env->assembled[0]);
			CURR_FUNC_CALL->env->bracketsOffset[0] = CURR_FUNC_CALL->env->assembled[0];
			fragmentOffset = currFrag->offset;
			stretchingVarNumber = CURR_FUNC_CALL->env->stretchVarsNumber[0];
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
				//Matching s.3 variable
					if (fragmentOffset >= rightBound || _memMngr.vterms[fragmentOffset].tag == V_BRACKETS_TAG)
					{
						stretching = 1;
						stretchingVarNumber = -1;
						break;
					}
					else
					{
						(CURR_FUNC_CALL->env->locals + 2)->offset = fragmentOffset;
						(CURR_FUNC_CALL->env->locals + 2)->length = 1;
						fragmentOffset++;
					}
				//Matching s.4 variable
					if (fragmentOffset >= rightBound || _memMngr.vterms[fragmentOffset].tag == V_BRACKETS_TAG)
					{
						stretching = 1;
						stretchingVarNumber = -1;
						break;
					}
					else
					{
						(CURR_FUNC_CALL->env->locals + 3)->offset = fragmentOffset;
						(CURR_FUNC_CALL->env->locals + 3)->length = 1;
						fragmentOffset++;
					}
				//Matching e.1 variable
				case 1:
					if (!stretching) // Just init values
					{
						(CURR_FUNC_CALL->env->locals + 4)->offset = fragmentOffset;
						(CURR_FUNC_CALL->env->locals + 4)->length = 0;
					}
					else // stretching
					{
						stretching = 0;
						CURR_FUNC_CALL->env->stretchVarsNumber[0] = 1;
						rightBound = RIGHT_BOUND(CURR_FUNC_CALL->env->bracketsOffset[0]);
						//Restore last offset at this point
						fragmentOffset = (CURR_FUNC_CALL->env->locals + 4)->offset +  (CURR_FUNC_CALL->env->locals + 4)->length;
						if (fragmentOffset >= rightBound)
						{
							stretching = 1;
							stretchingVarNumber = -1;
							break;
						}
						else
						{
							fragmentOffset++;
							(CURR_FUNC_CALL->env->locals + 4)->length++;
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
				//First pattern of last sentence -> nothing to stretch -> fail!
				funcRes = (struct func_result_t){.status = FAIL_RESULT, .fieldChain = 0, .callChain = 0};
				CURR_FUNC_CALL->entryPoint = -1;
				break;
			}
			//Start construction assembly action.
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
				helper = chAllocateChainKeeperLTerm(UINT64_C(1), &status);
				if (status == GC_NEED_CLEAN)
				{
					continue;
				}
				//Start construction parenthesis.
				currTerm = chAllocateFragmentLTerm(1, &status);
				if (status == GC_NEED_CLEAN)
				{
					continue;
				}
				currTerm->fragment->offset = (CURR_FUNC_CALL->env->locals + 4)->offset;
				currTerm->fragment->length = (CURR_FUNC_CALL->env->locals + 4)->length;
				//Adding term to field chain -- Just concat.
				ADD_TO_CHAIN(helper[0].chain, currTerm);
				//Finished construction parenthesis. Save in currTerm.
				currTerm = &helper[0];
				CURR_FUNC_CALL->env->workFieldOfView = currTerm->chain;
				funcRes = (struct func_result_t){.status = OK_RESULT, .fieldChain = currTerm->chain, .callChain = funcCallChain};
			} while (status != GC_OK); // GC block
			CURR_FUNC_CALL->entryPoint = -1;
			break; //Successful end of sentence
		} // Pattern case end
		} // Entry point switch end
	} // Main while end
	return funcRes;
} // func Func4:func_4

struct func_result_t func_5(int entryStatus) 
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
		checkAndCleanHeaps(0, ENV_SIZE(3, 1, 1));
		initEnvData(CURR_FUNC_CALL->env, 3, 1, 1);
	}
	else if (entryStatus == ROLL_BACK)
		stretching = 1;
	while(CURR_FUNC_CALL->entryPoint >= 0)
	{
		switch (CURR_FUNC_CALL->entryPoint)
		{
		//Sentence: 0, Pattern: 0
		case 0:
		{
			if (!stretching)
			{
				ASSEMBLY_FIELD(0, CURR_FUNC_CALL->fieldOfView);
			} // !stretching
			currFrag = VTERM_BRACKETS(CURR_FUNC_CALL->env->assembled[0]);
			rightBound = RIGHT_BOUND(CURR_FUNC_CALL->env->assembled[0]);
			CURR_FUNC_CALL->env->bracketsOffset[0] = CURR_FUNC_CALL->env->assembled[0];
			fragmentOffset = currFrag->offset;
			stretchingVarNumber = CURR_FUNC_CALL->env->stretchVarsNumber[0];
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
				//Matching e.1 variable
				case 1:
					if (!stretching) // Just init values
					{
						(CURR_FUNC_CALL->env->locals + 1)->offset = fragmentOffset;
						(CURR_FUNC_CALL->env->locals + 1)->length = 0;
					}
					else // stretching
					{
						stretching = 0;
						CURR_FUNC_CALL->env->stretchVarsNumber[0] = 1;
						rightBound = RIGHT_BOUND(CURR_FUNC_CALL->env->bracketsOffset[0]);
						//Restore last offset at this point
						fragmentOffset = (CURR_FUNC_CALL->env->locals + 1)->offset +  (CURR_FUNC_CALL->env->locals + 1)->length;
						if (fragmentOffset >= rightBound)
						{
							stretching = 1;
							stretchingVarNumber = -1;
							break;
						}
						else
						{
							fragmentOffset++;
							(CURR_FUNC_CALL->env->locals + 1)->length++;
						}
					}
				//Matching s.2 variable
					if (fragmentOffset >= rightBound || _memMngr.vterms[fragmentOffset].tag == V_BRACKETS_TAG)
					{
						stretching = 1;
						stretchingVarNumber = 1;
						break;
					}
					else
					{
						(CURR_FUNC_CALL->env->locals + 2)->offset = fragmentOffset;
						(CURR_FUNC_CALL->env->locals + 2)->length = 1;
						fragmentOffset++;
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
				//First pattern of last sentence -> nothing to stretch -> fail!
				funcRes = (struct func_result_t){.status = FAIL_RESULT, .fieldChain = 0, .callChain = 0};
				CURR_FUNC_CALL->entryPoint = -1;
				break;
			}
			//Start construction assembly action.
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
				helper = chAllocateChainKeeperLTerm(UINT64_C(1), &status);
				if (status == GC_NEED_CLEAN)
				{
					continue;
				}
				//Start construction parenthesis.
				currTerm = chAllocateFragmentLTerm(1, &status);
				if (status == GC_NEED_CLEAN)
				{
					continue;
				}
				currTerm->fragment->offset = (CURR_FUNC_CALL->env->locals + 1)->offset;
				currTerm->fragment->length = (CURR_FUNC_CALL->env->locals + 1)->length;
				//Adding term to field chain -- Just concat.
				ADD_TO_CHAIN(helper[0].chain, currTerm);
				//Finished construction parenthesis. Save in currTerm.
				currTerm = &helper[0];
				CURR_FUNC_CALL->env->workFieldOfView = currTerm->chain;
				funcRes = (struct func_result_t){.status = OK_RESULT, .fieldChain = currTerm->chain, .callChain = funcCallChain};
			} while (status != GC_OK); // GC block
			CURR_FUNC_CALL->entryPoint = -1;
			break; //Successful end of sentence
		} // Pattern case end
		} // Entry point switch end
	} // Main while end
	return funcRes;
} // func Func5:func_5

struct func_result_t func_6(int entryStatus) 
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
		checkAndCleanHeaps(0, ENV_SIZE(5, 1, 1));
		initEnvData(CURR_FUNC_CALL->env, 5, 1, 1);
	}
	else if (entryStatus == ROLL_BACK)
		stretching = 1;
	while(CURR_FUNC_CALL->entryPoint >= 0)
	{
		switch (CURR_FUNC_CALL->entryPoint)
		{
		//Sentence: 0, Pattern: 0
		case 0:
		{
			if (!stretching)
			{
				ASSEMBLY_FIELD(0, CURR_FUNC_CALL->fieldOfView);
			} // !stretching
			currFrag = VTERM_BRACKETS(CURR_FUNC_CALL->env->assembled[0]);
			rightBound = RIGHT_BOUND(CURR_FUNC_CALL->env->assembled[0]);
			CURR_FUNC_CALL->env->bracketsOffset[0] = CURR_FUNC_CALL->env->assembled[0];
			fragmentOffset = currFrag->offset;
			stretchingVarNumber = CURR_FUNC_CALL->env->stretchVarsNumber[0];
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
				//Matching s.3 variable
					if (fragmentOffset >= rightBound || _memMngr.vterms[fragmentOffset].tag == V_BRACKETS_TAG)
					{
						stretching = 1;
						stretchingVarNumber = -1;
						break;
					}
					else
					{
						(CURR_FUNC_CALL->env->locals + 2)->offset = fragmentOffset;
						(CURR_FUNC_CALL->env->locals + 2)->length = 1;
						fragmentOffset++;
					}
				//Matching s.4 variable
					if (fragmentOffset >= rightBound || _memMngr.vterms[fragmentOffset].tag == V_BRACKETS_TAG)
					{
						stretching = 1;
						stretchingVarNumber = -1;
						break;
					}
					else
					{
						(CURR_FUNC_CALL->env->locals + 3)->offset = fragmentOffset;
						(CURR_FUNC_CALL->env->locals + 3)->length = 1;
						fragmentOffset++;
					}
				//Matching e.1 variable
				case 1:
					if (!stretching) // Just init values
					{
						(CURR_FUNC_CALL->env->locals + 4)->offset = fragmentOffset;
						(CURR_FUNC_CALL->env->locals + 4)->length = 0;
					}
					else // stretching
					{
						stretching = 0;
						CURR_FUNC_CALL->env->stretchVarsNumber[0] = 1;
						rightBound = RIGHT_BOUND(CURR_FUNC_CALL->env->bracketsOffset[0]);
						//Restore last offset at this point
						fragmentOffset = (CURR_FUNC_CALL->env->locals + 4)->offset +  (CURR_FUNC_CALL->env->locals + 4)->length;
						if (fragmentOffset >= rightBound)
						{
							stretching = 1;
							stretchingVarNumber = -1;
							break;
						}
						else
						{
							fragmentOffset++;
							(CURR_FUNC_CALL->env->locals + 4)->length++;
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
				//First pattern of last sentence -> nothing to stretch -> fail!
				funcRes = (struct func_result_t){.status = FAIL_RESULT, .fieldChain = 0, .callChain = 0};
				CURR_FUNC_CALL->entryPoint = -1;
				break;
			}
			//Start construction assembly action.
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
				helper = chAllocateChainKeeperLTerm(UINT64_C(1), &status);
				if (status == GC_NEED_CLEAN)
				{
					continue;
				}
				//Start construction parenthesis.
				currTerm = chAllocateFragmentLTerm(1, &status);
				if (status == GC_NEED_CLEAN)
				{
					continue;
				}
				currTerm->fragment->offset = (CURR_FUNC_CALL->env->locals + 1)->offset;
				currTerm->fragment->length = (CURR_FUNC_CALL->env->locals + 1)->length;
				//Adding term to field chain -- Just concat.
				ADD_TO_CHAIN(helper[0].chain, currTerm);
				currTerm = chAllocateFragmentLTerm(1, &status);
				if (status == GC_NEED_CLEAN)
				{
					continue;
				}
				currTerm->fragment->offset = (CURR_FUNC_CALL->env->locals + 3)->offset;
				currTerm->fragment->length = (CURR_FUNC_CALL->env->locals + 3)->length;
				//Adding term to field chain -- Just concat.
				ADD_TO_CHAIN(helper[0].chain, currTerm);
				currTerm = chAllocateFragmentLTerm(1, &status);
				if (status == GC_NEED_CLEAN)
				{
					continue;
				}
				currTerm->fragment->offset = (CURR_FUNC_CALL->env->locals + 0)->offset;
				currTerm->fragment->length = (CURR_FUNC_CALL->env->locals + 0)->length;
				//Adding term to field chain -- Just concat.
				ADD_TO_CHAIN(helper[0].chain, currTerm);
				currTerm = chAllocateFragmentLTerm(1, &status);
				if (status == GC_NEED_CLEAN)
				{
					continue;
				}
				currTerm->fragment->offset = (CURR_FUNC_CALL->env->locals + 2)->offset;
				currTerm->fragment->length = (CURR_FUNC_CALL->env->locals + 2)->length;
				//Adding term to field chain -- Just concat.
				ADD_TO_CHAIN(helper[0].chain, currTerm);
				//Finished construction parenthesis. Save in currTerm.
				currTerm = &helper[0];
				CURR_FUNC_CALL->env->workFieldOfView = currTerm->chain;
				funcRes = (struct func_result_t){.status = OK_RESULT, .fieldChain = currTerm->chain, .callChain = funcCallChain};
			} while (status != GC_OK); // GC block
			CURR_FUNC_CALL->entryPoint = -1;
			break; //Successful end of sentence
		} // Pattern case end
		} // Entry point switch end
	} // Main while end
	return funcRes;
} // func Func6:func_6

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
		//Sentence: 0, Pattern: 0
		case 0:
		{
			if (!stretching)
			{
				ASSEMBLY_FIELD(0, CURR_FUNC_CALL->fieldOfView);
			} // !stretching
			currFrag = VTERM_BRACKETS(CURR_FUNC_CALL->env->assembled[0]);
			rightBound = RIGHT_BOUND(CURR_FUNC_CALL->env->assembled[0]);
			CURR_FUNC_CALL->env->bracketsOffset[0] = CURR_FUNC_CALL->env->assembled[0];
			fragmentOffset = currFrag->offset;
			stretchingVarNumber = CURR_FUNC_CALL->env->stretchVarsNumber[0];
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
				CURR_FUNC_CALL->entryPoint = -1;
				break;
			}
			//Start construction assembly action.
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
				helper = chAllocateChainKeeperLTerm(UINT64_C(8), &status);
				if (status == GC_NEED_CLEAN)
				{
					continue;
				}
				//Start construction parenthesis.
				//Start construction func call.
				//Start construction parenthesis.
				//Start construction func term.
				currTerm = chAllocateFragmentLTerm(1, &status);
				if (status == GC_NEED_CLEAN)
				{
					continue;
				}
				currTerm->fragment->offset = chAllocateClosureVTerm(&status);
				if (status == GC_NEED_CLEAN)
				{
					continue;
				}
				_memMngr.vterms[currTerm->fragment->offset].closure = chAllocateClosureStruct(Prout, 0, _memMngr.vterms[1].str, 0, &status);
				if (status == GC_NEED_CLEAN)
				{
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
				if (status == GC_NEED_CLEAN)
				{
					continue;
				}
				currTerm->fragment->offset = chAllocateClosureVTerm(&status);
				if (status == GC_NEED_CLEAN)
				{
					continue;
				}
				_memMngr.vterms[currTerm->fragment->offset].closure = chAllocateClosureStruct(func_1, 0, _memMngr.vterms[2].str, 0, &status);
				if (status == GC_NEED_CLEAN)
				{
					continue;
				}
				currTerm->fragment->length = 1;
				//Finish construction func term.
				//Adding term to field chain -- Just concat.
				ADD_TO_CHAIN(helper[2].chain, currTerm);
				//Start construction fragment term.
				currTerm = chAllocateFragmentLTerm(1, &status);
				if (status == GC_NEED_CLEAN)
				{
					continue;
				}
				currTerm->fragment->offset = 3;
				currTerm->fragment->length = 4;
				//Adding term to field chain -- Just concat.
				ADD_TO_CHAIN(helper[2].chain, currTerm);
				//Finished construction parenthesis. Save in currTerm.
				currTerm = &helper[2];
				funcTerm = chAllocateFuncCallLTerm(&status);
				if (status == GC_NEED_CLEAN)
				{
					continue;
				}
				funcTerm->funcCall->failEntryPoint = -1;
				funcTerm->funcCall->fieldOfView = currTerm->chain;
				//Finished construction func call
				//First call in call chain -- Initialization.
				funcCallChain = chAllocateSimpleChainLTerm(&status);
				if (status == GC_NEED_CLEAN)
				{
					continue;
				}
				funcCallChain->next = funcTerm;
				funcCallChain->prev = funcTerm;
				currTerm = funcTerm;
				//Adding term to field chain -- Just concat.
				ADD_TO_CHAIN(helper[1].chain, currTerm);
				//Start construction fragment term.
				currTerm = chAllocateFragmentLTerm(1, &status);
				if (status == GC_NEED_CLEAN)
				{
					continue;
				}
				currTerm->fragment->offset = 7;
				currTerm->fragment->length = 1;
				//Adding term to field chain -- Just concat.
				ADD_TO_CHAIN(helper[1].chain, currTerm);
				//Start construction func call.
				//Start construction parenthesis.
				//Start construction func term.
				currTerm = chAllocateFragmentLTerm(1, &status);
				if (status == GC_NEED_CLEAN)
				{
					continue;
				}
				currTerm->fragment->offset = chAllocateClosureVTerm(&status);
				if (status == GC_NEED_CLEAN)
				{
					continue;
				}
				_memMngr.vterms[currTerm->fragment->offset].closure = chAllocateClosureStruct(func_2, 0, _memMngr.vterms[8].str, 0, &status);
				if (status == GC_NEED_CLEAN)
				{
					continue;
				}
				currTerm->fragment->length = 1;
				//Finish construction func term.
				//Adding term to field chain -- Just concat.
				ADD_TO_CHAIN(helper[3].chain, currTerm);
				//Start construction fragment term.
				currTerm = chAllocateFragmentLTerm(1, &status);
				if (status == GC_NEED_CLEAN)
				{
					continue;
				}
				currTerm->fragment->offset = 9;
				currTerm->fragment->length = 4;
				//Adding term to field chain -- Just concat.
				ADD_TO_CHAIN(helper[3].chain, currTerm);
				//Finished construction parenthesis. Save in currTerm.
				currTerm = &helper[3];
				funcTerm = chAllocateFuncCallLTerm(&status);
				if (status == GC_NEED_CLEAN)
				{
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
				ADD_TO_CHAIN(helper[1].chain, currTerm);
				//Start construction fragment term.
				currTerm = chAllocateFragmentLTerm(1, &status);
				if (status == GC_NEED_CLEAN)
				{
					continue;
				}
				currTerm->fragment->offset = 13;
				currTerm->fragment->length = 1;
				//Adding term to field chain -- Just concat.
				ADD_TO_CHAIN(helper[1].chain, currTerm);
				//Start construction func call.
				//Start construction parenthesis.
				//Start construction func term.
				currTerm = chAllocateFragmentLTerm(1, &status);
				if (status == GC_NEED_CLEAN)
				{
					continue;
				}
				currTerm->fragment->offset = chAllocateClosureVTerm(&status);
				if (status == GC_NEED_CLEAN)
				{
					continue;
				}
				_memMngr.vterms[currTerm->fragment->offset].closure = chAllocateClosureStruct(func_3, 0, _memMngr.vterms[14].str, 0, &status);
				if (status == GC_NEED_CLEAN)
				{
					continue;
				}
				currTerm->fragment->length = 1;
				//Finish construction func term.
				//Adding term to field chain -- Just concat.
				ADD_TO_CHAIN(helper[4].chain, currTerm);
				//Start construction fragment term.
				currTerm = chAllocateFragmentLTerm(1, &status);
				if (status == GC_NEED_CLEAN)
				{
					continue;
				}
				currTerm->fragment->offset = 15;
				currTerm->fragment->length = 4;
				//Adding term to field chain -- Just concat.
				ADD_TO_CHAIN(helper[4].chain, currTerm);
				//Finished construction parenthesis. Save in currTerm.
				currTerm = &helper[4];
				funcTerm = chAllocateFuncCallLTerm(&status);
				if (status == GC_NEED_CLEAN)
				{
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
				ADD_TO_CHAIN(helper[1].chain, currTerm);
				//Start construction fragment term.
				currTerm = chAllocateFragmentLTerm(1, &status);
				if (status == GC_NEED_CLEAN)
				{
					continue;
				}
				currTerm->fragment->offset = 19;
				currTerm->fragment->length = 1;
				//Adding term to field chain -- Just concat.
				ADD_TO_CHAIN(helper[1].chain, currTerm);
				//Start construction func call.
				//Start construction parenthesis.
				//Start construction func term.
				currTerm = chAllocateFragmentLTerm(1, &status);
				if (status == GC_NEED_CLEAN)
				{
					continue;
				}
				currTerm->fragment->offset = chAllocateClosureVTerm(&status);
				if (status == GC_NEED_CLEAN)
				{
					continue;
				}
				_memMngr.vterms[currTerm->fragment->offset].closure = chAllocateClosureStruct(func_4, 0, _memMngr.vterms[20].str, 0, &status);
				if (status == GC_NEED_CLEAN)
				{
					continue;
				}
				currTerm->fragment->length = 1;
				//Finish construction func term.
				//Adding term to field chain -- Just concat.
				ADD_TO_CHAIN(helper[5].chain, currTerm);
				//Start construction fragment term.
				currTerm = chAllocateFragmentLTerm(1, &status);
				if (status == GC_NEED_CLEAN)
				{
					continue;
				}
				currTerm->fragment->offset = 21;
				currTerm->fragment->length = 4;
				//Adding term to field chain -- Just concat.
				ADD_TO_CHAIN(helper[5].chain, currTerm);
				//Finished construction parenthesis. Save in currTerm.
				currTerm = &helper[5];
				funcTerm = chAllocateFuncCallLTerm(&status);
				if (status == GC_NEED_CLEAN)
				{
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
				ADD_TO_CHAIN(helper[1].chain, currTerm);
				//Start construction fragment term.
				currTerm = chAllocateFragmentLTerm(1, &status);
				if (status == GC_NEED_CLEAN)
				{
					continue;
				}
				currTerm->fragment->offset = 25;
				currTerm->fragment->length = 1;
				//Adding term to field chain -- Just concat.
				ADD_TO_CHAIN(helper[1].chain, currTerm);
				//Start construction func call.
				//Start construction parenthesis.
				//Start construction func term.
				currTerm = chAllocateFragmentLTerm(1, &status);
				if (status == GC_NEED_CLEAN)
				{
					continue;
				}
				currTerm->fragment->offset = chAllocateClosureVTerm(&status);
				if (status == GC_NEED_CLEAN)
				{
					continue;
				}
				_memMngr.vterms[currTerm->fragment->offset].closure = chAllocateClosureStruct(func_5, 0, _memMngr.vterms[26].str, 0, &status);
				if (status == GC_NEED_CLEAN)
				{
					continue;
				}
				currTerm->fragment->length = 1;
				//Finish construction func term.
				//Adding term to field chain -- Just concat.
				ADD_TO_CHAIN(helper[6].chain, currTerm);
				//Start construction fragment term.
				currTerm = chAllocateFragmentLTerm(1, &status);
				if (status == GC_NEED_CLEAN)
				{
					continue;
				}
				currTerm->fragment->offset = 27;
				currTerm->fragment->length = 4;
				//Adding term to field chain -- Just concat.
				ADD_TO_CHAIN(helper[6].chain, currTerm);
				//Finished construction parenthesis. Save in currTerm.
				currTerm = &helper[6];
				funcTerm = chAllocateFuncCallLTerm(&status);
				if (status == GC_NEED_CLEAN)
				{
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
				ADD_TO_CHAIN(helper[1].chain, currTerm);
				//Start construction fragment term.
				currTerm = chAllocateFragmentLTerm(1, &status);
				if (status == GC_NEED_CLEAN)
				{
					continue;
				}
				currTerm->fragment->offset = 31;
				currTerm->fragment->length = 1;
				//Adding term to field chain -- Just concat.
				ADD_TO_CHAIN(helper[1].chain, currTerm);
				//Start construction func call.
				//Start construction parenthesis.
				//Start construction func term.
				currTerm = chAllocateFragmentLTerm(1, &status);
				if (status == GC_NEED_CLEAN)
				{
					continue;
				}
				currTerm->fragment->offset = chAllocateClosureVTerm(&status);
				if (status == GC_NEED_CLEAN)
				{
					continue;
				}
				_memMngr.vterms[currTerm->fragment->offset].closure = chAllocateClosureStruct(func_6, 0, _memMngr.vterms[32].str, 0, &status);
				if (status == GC_NEED_CLEAN)
				{
					continue;
				}
				currTerm->fragment->length = 1;
				//Finish construction func term.
				//Adding term to field chain -- Just concat.
				ADD_TO_CHAIN(helper[7].chain, currTerm);
				//Start construction fragment term.
				currTerm = chAllocateFragmentLTerm(1, &status);
				if (status == GC_NEED_CLEAN)
				{
					continue;
				}
				currTerm->fragment->offset = 33;
				currTerm->fragment->length = 4;
				//Adding term to field chain -- Just concat.
				ADD_TO_CHAIN(helper[7].chain, currTerm);
				//Finished construction parenthesis. Save in currTerm.
				currTerm = &helper[7];
				funcTerm = chAllocateFuncCallLTerm(&status);
				if (status == GC_NEED_CLEAN)
				{
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
				ADD_TO_CHAIN(helper[1].chain, currTerm);
				//Finished construction parenthesis. Save in currTerm.
				currTerm = &helper[1];
				funcTerm = chAllocateFuncCallLTerm(&status);
				if (status == GC_NEED_CLEAN)
				{
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
				CURR_FUNC_CALL->env->workFieldOfView = currTerm->chain;
				funcRes = (struct func_result_t){.status = OK_RESULT, .fieldChain = currTerm->chain, .callChain = funcCallChain};
			} while (status != GC_OK); // GC block
			CURR_FUNC_CALL->entryPoint = -1;
			break; //Successful end of sentence
		} // Pattern case end
		} // Entry point switch end
	} // Main while end
	return funcRes;
} // func Go:func_0

int main(int argc, char** argv)
{
	initAllocator(getHeapSizeFromCmdArgs(argc, argv));
	initLiteralData();
	uint64_t vtermOffset = initArgsData(UINT64_C(37), argc, argv);
	initHeaps(vtermOffset);
	mainLoop("Go", func_0);
	return 0;
}
