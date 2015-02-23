#ifndef __SEGMENT_TREE_H__
#define __SEGMENT_TREE_H__

#include "memory_manager.h"

struct segment_tree
{
    uint64_t currN;
	int32_t* tree;
    uint64_t* elements;
};

void buildSegmentTree(uint64_t n);
void clearSegmentTree();
void markInSegmentTree(uint64_t begin, uint64_t end);
uint64_t sumInSegmentTree(uint64_t begin, uint64_t end);

#endif
