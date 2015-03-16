#ifndef __GC_H__
#define __GC_H__

#include <memory_manager.h>

typedef int  allocate_result;

#define OK          0
#define NEED_CLEAN  1

/// Собирает мусор.
void collectGarbage();

struct lterm_t* copyFragmentLTerm(struct lterm_t* term);

void strictCheckHeaps(uint64_t needTermCount, uint64_t needDataSize);
int isHeapsOverflowed(uint64_t needTermCount, uint64_t needDataSize);
int checkAndCleanHeaps(uint64_t needTermCount, uint64_t needDataSize);

#endif
