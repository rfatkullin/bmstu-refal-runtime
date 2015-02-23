#include <string.h>
#include <math.h>

#include "segment_tree.h"

static void mark(uint64_t curr, uint64_t currBegin, uint64_t currEnd, uint64_t needBegin, uint64_t needEnd);
static uint64_t sum(uint64_t curr, uint64_t currBegin, uint64_t currEnd, uint64_t needBegin, uint64_t needEnd);

void buildSegmentTree(uint64_t n)
{
	memMngr.segmentTree->currN = n;
    uint64_t treeSize = 4 * memMngr.segmentTree->currN / memMngr.segmentLen;
    uint64_t i = 0;

	//Кажется, можно просто обнулять. Но пока работает)))
	for (i = 0; i < treeSize; ++i)
		memMngr.segmentTree->tree[i] = -1;

    memset(memMngr.segmentTree->elements, 0, sizeof(uint64_t) * memMngr.segmentTree->currN);
}

void clearSegmentTree()
{
    uint64_t treeSize = 4 * memMngr.segmentTree->currN / memMngr.segmentLen;
    uint64_t i = 0;

	for (i = 0; i < treeSize; ++i)
		memMngr.segmentTree->tree[i] = -1;

    memset(memMngr.segmentTree->elements, 0, sizeof(uint64_t) * memMngr.segmentTree->currN);
}

void markInSegmentTree(uint64_t begin, uint64_t end)
{
	mark(1, 0, memMngr.segmentTree->currN - 1, begin, end);
}

uint64_t sumInSegmentTree(uint64_t begin, uint64_t end)
{
	sum(1, 0, memMngr.segmentTree->currN - 1, begin, end);
}

static uint64_t max(uint64_t a, uint64_t b)
{
	if (a > b)
		return a;

	return b;
}

static uint64_t min(uint64_t a, uint64_t b)
{
	if (a > b)
		return b;

	return a;
}

static void markElements(uint64_t needBegin, uint64_t needEnd, int32_t mark)
{
	int i = 0;
    uint64_t segmentNum = needBegin / memMngr.segmentLen;

	for (i = segmentNum * memMngr.segmentLen; i < (segmentNum + 1) * memMngr.segmentLen; ++i)
	{
		if (i >= needBegin && i <= needEnd)
			memMngr.segmentTree->elements[i] = 1;
		else
			memMngr.segmentTree->elements[i] = mark;
	}
}

static uint64_t sumElements(uint64_t needBegin, uint64_t needEnd)
{
    uint64_t i = 0;
    uint64_t sum = 0;

	for (i = needBegin; i <= needEnd; ++i)
		sum += memMngr.segmentTree->elements[i];

	return sum;
}

static void push(uint64_t curr)
{
	if (memMngr.segmentTree->tree[curr] == -1)
				return;

	memMngr.segmentTree->tree[curr * 2] = memMngr.segmentTree->tree[curr];
	memMngr.segmentTree->tree[curr * 2 + 1] = memMngr.segmentTree->tree[curr];
	memMngr.segmentTree->tree[curr] = -1;
}

static uint64_t left(uint64_t val)
{
	return val * memMngr.segmentLen;
}

static uint64_t right(uint64_t val)
{
	return (val + 1) * memMngr.segmentLen - 1;
}

static void mark(uint64_t curr, uint64_t currBegin, uint64_t currEnd, uint64_t needBegin, uint64_t needEnd)
{
	if (needBegin > needEnd)
		return;

	//Наткнулись на узел, который совпадает с текущим отрезком
	if (needBegin == left(currBegin) && needEnd == right(currEnd))
	{
		memMngr.segmentTree->tree[curr] = 1;
		return;
	}

	//Дошли до листа
	if (currBegin == currEnd)
	{
        uint64_t label = 0;

		if (memMngr.segmentTree->tree[currBegin] > 0)
			label = 1;

		markElements(needBegin, needEnd, label);
		memMngr.segmentTree->tree[curr] = -1;
		return;
	}

	push(curr);

    uint64_t currMiddle = (currBegin + currEnd) / 2;

	mark(curr * 2, currBegin, currMiddle, needBegin, min(right(currMiddle), needEnd));
	mark(curr * 2 + 1, currMiddle + 1, currEnd, max(left(currMiddle + 1), needBegin), needEnd);
}

static uint64_t sum(uint64_t curr, uint64_t currBegin, uint64_t currEnd, uint64_t needBegin, uint64_t needEnd)
{
	if (needBegin > needEnd)
		return 0;

	if (memMngr.segmentTree->tree[curr] != -1)
		return memMngr.segmentTree->tree[curr] * (needEnd - needBegin + 1);

	//Дошли до листа
	if (currBegin == currEnd)
		return sumElements(needBegin, needEnd);

    uint64_t currMiddle = (currBegin + currEnd) / 2;

	return sum(curr * 2, currBegin, currMiddle, needBegin, min(right(currMiddle), needEnd)) +
		sum(curr * 2 + 1, currMiddle + 1, currEnd, max(left(currMiddle + 1), needBegin), needEnd);
}
