// file:source.ref

#include <stdlib.h>
#include <stdio.h>

#include <vmachine.h>
#include <memory_manager.h>
#include <builtins/builtins.h>
#include <allocators/data_alloc.h>
#include <allocators/vterm_alloc.h>
#include <defines/data_struct_sizes.h>

struct func_result_t func_0(int entryStatus);

void initLiteralData()
{
	_memMngr.vterms[0] = (struct vterm_t){.tag = V_IDENT_TAG, .str = allocateVStringLiteral((uint32_t[]){80,114,111,117,116}, UINT64_C(5))};
	_memMngr.vterms[1] = (struct vterm_t){.tag = V_IDENT_TAG, .str = allocateVStringLiteral((uint32_t[]){83,117,98}, UINT64_C(3))};
	_memMngr.vterms[2] = (struct vterm_t){.tag = V_INT_NUM_TAG, .intNum = allocateIntNumberLiteral((uint8_t[]){40,14,95,201,182,38,252,176,13,66,196,195,112,177,182,213,20,166,32,28,177,175,11,23}, 0, UINT64_C(24))};
	_memMngr.vterms[3] = (struct vterm_t){.tag = V_INT_NUM_TAG, .intNum = allocateIntNumberLiteral((uint8_t[]){5,41,51,155,167,115,81,187,159,66,195}, 0, UINT64_C(11))};

} // initLiteralData()

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
				helper = chAllocateChainKeeperLTerm(UINT64_C(3), &status);
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
				_memMngr.vterms[currTerm->fragment->offset].closure = chAllocateClosureStruct(Prout, 0, _memMngr.vterms[0].str, 0, &status);
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
				_memMngr.vterms[currTerm->fragment->offset].closure = chAllocateClosureStruct(Sub, 0, _memMngr.vterms[1].str, 0, &status);
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
				currTerm->fragment->offset = 2;
				currTerm->fragment->length = 2;
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
	initAllocator(getHeapSize(argc, argv));
	initLiteralData();
	uint64_t vtermOffset = initArgsData(UINT64_C(4), argc, argv);
	initHeaps(vtermOffset);
	mainLoop("Go", func_0);
	return 0;
}
