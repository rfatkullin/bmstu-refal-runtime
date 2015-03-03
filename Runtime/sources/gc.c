#include <stdio.h>
#include <time.h>

#include "memory_manager.h"

static void swapBuffers();
static void markVTerms(struct lterm_t* expr);

void collectGarbage(struct lterm_t* expr)
{
    clock_t start, end;
    printf("Start garbage collection.\n");
    start = clock();

    buildSegmentTree(memMngr.vtermsOffset);

    markVTerms(expr);

    swapBuffers();

    end = clock();

    printf("End garbage collection. Time elapsed: %f\n", ((float)(end - start)) / CLOCKS_PER_SEC);
}

static void swapBuffers()
{
    uint64_t dataOffset = 0;
    int i = 0;

//	for (i = memMngr.activeOffset; i < memMngr.vtermsOffset; ++i)
//	{
//		if (sumInSegmentTree(i, i + 1) > 0)
//		{
//			//Копируем vterm
//			memcpy((void*)(memMngr.activeTermsHeap + i),
//				(void*)vterms,
//				sizeof(struct v_term)
//			);
//			++vterms;

//			//Копируем данные vterm'а
//			dataOffset += copyVTerm(memMngr.activeTermsHeap + i);
//		}
//	}

//	memMngr.vtermsOffset = memMngr.activeOffset;

//	memMngr.activeTermsHeap = memMngr.inactiveTermsHeap;
//	memMngr.activeDataHeap = memMngr.inactiveDataHeap;
//	memMngr.vtermsCount = vterms - memMngr.inactiveTermsHeap;
//	memMngr.vtermsOffset = vterms - memMngr.inactiveTermsHeap;
//	memMngr.dataOffset = dataOffset;
}

static void markVTerms(struct lterm_t* expr)
{
    struct lterm_t* currTerm = expr;

    while (currTerm)
    {
        switch (currTerm->tag)
        {
            case L_TERM_FRAGMENT_TAG:
                markInSegmentTree(currTerm->fragment->offset, currTerm->fragment->offset + currTerm->fragment->length - 1);
                break;

            case L_TERM_CHAIN_TAG:
                markVTerms(currTerm);
                break;
        }

        currTerm = currTerm->next;
    }
}
