#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <string.h>

#include "memory_manager.h"

static float byte2KByte(uint64_t bytes);
static void swapBuffers();
static void markVTerms(struct lterm_t* expr);

static void allocateMemoryForSegmentTree(uint64_t termsNumber, uint8_t** pointer);
static void allocateMemoryForVTerms(uint64_t size, uint8_t** pointer);
static void allocateMemoryForData(uint64_t size, uint8_t** pointer);
static void allocateMemoryForLTerms(uint64_t size, uint8_t** pointer);
static struct lterm_t* allocateLTerm(uint64_t offset, uint64_t len);
static void failWithMemoryOverflow();

static void failWithMemoryOverflow()
{
	printf("Failed with memory overflow!");
	exit(1);
}

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

// TO FIX: сделать проверку переполнения памяти.
// Сделать все правильно -- выделение памяти в области данных хипа
uint64_t allocateClosure(RefalFunc funcPtr, uint64_t envSize)
{
	struct v_term* term = memMngr.vterms + memMngr.vtermsOffset;
	term->tag = V_CLOSURE_TAG;

	//Выделение должно происходить в области данных хипа
	term->closure = (struct v_closure*)malloc(sizeof(struct v_closure));
	term->closure->env = (struct lterm_t*)malloc(envSize * sizeof(struct lterm_t));
	term->closure->funcPtr = funcPtr;

	return memMngr.vtermsOffset++;
}

uint64_t allocateSymbol(uint32_t ch)
{
	checkVTermsMemoryOverflow(1);

	struct v_term* term = memMngr.vterms + memMngr.vtermsOffset;
	term->tag = V_CHAR_TAG;
	term->ch = ch;

	return memMngr.vtermsOffset++;
}

uint64_t allocateIntNum(uint64_t count)
{
    uint64_t i = 0;
    uint64_t offset = 0;

	checkVTermsMemoryOverflow(count);

	for (i = 0; i < count; ++i)
		memMngr.vterms[memMngr.vtermsOffset + i].tag = V_INT_NUM_TAG;

	offset = memMngr.vtermsOffset;
	memMngr.vtermsOffset += count;

	return offset;
}

//TO FIX: сделать проверку переполнения памяти.
void allocateVTerms(struct fragment_t* frag)
{
    uint64_t i = 0;
	for (i = 0; i < frag->length; ++i)
	{
		memMngr.vterms[memMngr.vtermsOffset].tag = memMngr.vterms[frag->offset + i].tag;

		switch (memMngr.vterms[frag->offset + i].tag)
		{
			case V_CHAR_TAG:
				memMngr.vterms[memMngr.vtermsOffset].ch = memMngr.vterms[frag->offset + i].ch;
				break;

			case V_IDENT_TAG :
				memMngr.vterms[memMngr.vtermsOffset].str = memMngr.vterms[frag->offset + i].str;
				break;

			case V_INT_NUM_TAG:
				memMngr.vterms[memMngr.vtermsOffset].intNum = memMngr.vterms[frag->offset + i].intNum;
				break;

			case V_FLOAT_NUM_TAG:
				memMngr.vterms[memMngr.vtermsOffset].floatNum = memMngr.vterms[frag->offset + i].floatNum;
				break;

			case V_CLOSURE_TAG:
				//TO DO:
				break;

			case V_BRACKET_TAG:
				memMngr.vterms[memMngr.vtermsOffset].inBracketLength = memMngr.vterms[frag->offset + i].inBracketLength;
				break;
		}
		memMngr.vtermsOffset++;
	}
}

//TO FIX: сделать проверку переполнения памяти.
uint64_t allocateBracketVTerm(uint64_t length)
{
	memMngr.vterms[memMngr.vtermsOffset].tag = V_BRACKET_TAG;
	memMngr.vterms[memMngr.vtermsOffset].inBracketLength = length;

	return memMngr.vtermsOffset++;
}

void changeBracketLength(uint64_t offset, uint64_t newLength)
{
	memMngr.vterms[offset].inBracketLength = newLength;
}

void initAllocator(uint64_t size)
{
	memMngr.vterms = (struct v_term*)malloc(size);
	memMngr.totalSize = size;
}

void initHeaps(uint64_t segmentLen, uint64_t literalsNumber)
{
	//TO FIX: Если инициализируем данные, то их тоже нужно тут учитывать.
    uint64_t size = memMngr.totalSize - literalsNumber * sizeof(struct v_term);
    uint64_t dataHeapSize = DATA_HEAP_SIZE_FACTOR * size;
    uint64_t vtermsHeapSize = V_TERMS_HEAP_SIZE_FACTOR * size;
    uint64_t ltermsHeapSize = size - dataHeapSize - vtermsHeapSize;


//	printf("\nAllocation size:                      %.2f Kb\n", byte2KByte(size));
//	printf("\nAllocation ratios and sizes:         Ratio\t   Size\n");
//	printf("\t For data:                    %.2f\t %.2f Kb\n", DATA_HEAP_SIZE_FACTOR, byte2KByte(dataHeapSize));
//	printf("\t For vterms:                  %.2f\t %.2f Kb\n", V_TERMS_HEAP_SIZE_FACTOR, byte2KByte(vtermsHeapSize));
//	printf("\t For lterms:                  %.2f\t %.2f Kb\n", 1.0f - (DATA_HEAP_SIZE_FACTOR + V_TERMS_HEAP_SIZE_FACTOR), byte2KByte(ltermsHeapSize));

	memMngr.segmentLen = segmentLen;
	memMngr.literalsNumber = literalsNumber;

	uint8_t* pointer = (uint8_t*)memMngr.vterms;

	allocateMemoryForVTerms(vtermsHeapSize, &pointer);
	allocateMemoryForData(dataHeapSize, &pointer);
	allocateMemoryForLTerms(ltermsHeapSize, &pointer);

	memMngr.vtermsOffset = memMngr.activeOffset;
	memMngr.dataOffset = 0;
	memMngr.ltermsOffset = 0;
}

//void debugLiteralsPrint()
//{
//	printf("vterms debug print:\n\t");
//	int i;
//	for (i = 0; i < memMngr.literalsNumber; ++i)
//	{
//		printSymbol(memMngr.vterms + i);
//	}

//	printf("\n");
//}

static struct lterm_t* allocateLTerm(uint64_t offset, uint64_t len)
{
	struct lterm_t* term = (struct lterm_t*)malloc(sizeof(struct lterm_t));

	term->tag = L_TERM_FRAGMENT_TAG;
	term->fragment = (struct fragment_t*)malloc(sizeof(struct fragment_t));
	term->fragment->offset = offset;
	term->fragment->length = len;

	return term;
}

static float byte2KByte(uint64_t bytes)
{
	return bytes / 1024.0f;
}

// Значение n выводится из формулы:
// size = 2 * n * sizeof(struct v_term) + (4 * n / N + n) * sizeof(uint64_t)
// где N - длина отрезка в листьях дерева.
static uint64_t getTermsMaxNumber(uint64_t size)
{
    uint64_t memSizeWithoutHeader = size - sizeof(struct segment_tree);
    uint64_t N = memMngr.segmentLen;
    return (N * memSizeWithoutHeader) / (2 * N * sizeof(struct v_term) + (4 + N) * sizeof(uint64_t));
}

static void allocateMemoryForSegmentTree(uint64_t termsNumber, uint8_t** pointer)
{
    uint64_t chunck = memMngr.segmentLen;
    uint64_t memSizeForTree = 4 * termsNumber / chunck * sizeof(uint64_t);
    uint64_t memSizeForElements = termsNumber * sizeof(uint64_t);

	memMngr.segmentTree = (struct segment_tree*)(*pointer);
	memMngr.segmentTree->tree = (int32_t*)(*pointer + sizeof(struct segment_tree));
	memMngr.segmentTree->elements = (int32_t*)(*pointer + memSizeForTree + sizeof(struct segment_tree));

	*pointer += memSizeForTree + memSizeForElements + sizeof(struct segment_tree);
}

static void allocateMemoryForVTerms(uint64_t size, uint8_t** pointer)
{
    uint64_t maxTermsNumber = getTermsMaxNumber(size);
	memMngr.activeOffset = memMngr.literalsNumber;
	memMngr.inactiveOffset = memMngr.activeOffset + maxTermsNumber;
	memMngr.vtermsMaxOffset = maxTermsNumber - 1;
	*pointer += memMngr.literalsNumber * sizeof(struct v_term);
	*pointer += 2 * maxTermsNumber * sizeof(struct v_term);

	allocateMemoryForSegmentTree(maxTermsNumber, pointer);

	//printf("\tMax terms: %d\n", memMngr.maxTermsNumber);
}

static void allocateMemoryForData(uint64_t size, uint8_t** pointer)
{
    uint64_t singleDataHeapSize = size / 2;

	memMngr.dataHeap = *pointer;
	memMngr.activeDataHeap = *pointer;
	memMngr.inactiveDataHeap = *pointer + singleDataHeapSize;

	memMngr.dataMaxOffset = singleDataHeapSize;

	*pointer += size;
}

static void allocateMemoryForLTerms(uint64_t size, uint8_t** pointer)
{
	memMngr.lTermsHeap = *pointer;

	*pointer += size;
}

//Возвращает сколько байтов было использовано
static uint64_t copyVTerm(struct v_term* term)
{
	uint8_t* data = memMngr.inactiveDataHeap;
    uint64_t memSize = 0;

	switch (term->tag)
	{
		case V_CHAR_TAG:
			data[0] = term->str[0];
			memSize = 1;
			break;

		case V_IDENT_TAG:
			memSize = strlen(term->str) + 1;
			memcpy(term->str, data, memSize);
			break;

		case V_INT_NUM_TAG:
			((int*)data)[0] = term->intNum;
			memSize = sizeof(int);
			break;

		case V_CLOSURE_TAG:
			//Пока ничего не делаем
			break;
	}

	return memSize;
}

void checkVTermsMemoryOverflow(uint64_t needVTermsCount)
{
	if (memMngr.vtermsOffset + needVTermsCount > memMngr.vtermsMaxOffset)
	{
		//TO FIX: Передать корень FOV.
		collectGarbage(0);
	}

	if (memMngr.vtermsOffset + needVTermsCount > memMngr.vtermsMaxOffset)
		failWithMemoryOverflow();
}

void checkLTermsMemoryOverflow(uint64_t needLTermsCount)
{
	if (memMngr.ltermsOffset + needLTermsCount > memMngr.ltermsMaxOffset)
	{
		//TO FIX: Передать корень FOV.
		collectGarbage(0);
	}

	if (memMngr.ltermsOffset + needLTermsCount > memMngr.ltermsMaxOffset)
		failWithMemoryOverflow();
}

void checkDataMemoryOverflow(uint64_t needDataCount)
{
	if (memMngr.dataOffset + needDataCount > memMngr.dataMaxOffset)
	{
		//TO FIX: Передать корень FOV.
		collectGarbage(0);
	}

	if (memMngr.dataOffset + needDataCount > memMngr.dataMaxOffset)
		failWithMemoryOverflow();
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

struct lterm_t* constructLterm(uint64_t offset, uint64_t length)
{
	struct lterm_t* chain = 0;

	//TO FIX: Replace malloc
	struct lterm_t* inputFragment = (struct lterm_t*)malloc(sizeof(struct lterm_t));
	inputFragment->tag = L_TERM_FRAGMENT_TAG;
	inputFragment->fragment = (struct fragment_t*)malloc(sizeof(struct fragment_t));
	inputFragment->fragment->offset = offset;
	inputFragment->fragment->length = length;

	chain = (struct lterm_t*)malloc(sizeof(struct lterm_t));
	chain->next = inputFragment;
	chain->prev = inputFragment;
	inputFragment->next = chain;
	inputFragment->prev = chain;

	return chain;
}
