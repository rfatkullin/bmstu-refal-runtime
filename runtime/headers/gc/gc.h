#ifndef __GC_H__
#define __GC_H__

#include <helpers.h>
#include <memory_manager.h>
#include <defines/errors_str.h>

typedef int  allocate_result;

#define GC_OK          0
#define GC_NEED_CLEAN  1

/// Собирает мусор.
void collectGarbage();

struct lterm_t* copySimpleChain(struct lterm_t* chain);
void collectVTermGarbage(struct lterm_t* fieldOfView);

struct lterm_t* copyFragmentLTerm(struct lterm_t* term);

void strictCheckHeaps(uint64_t needTermCount, uint64_t needDataSize);
int isHeapsOverflowed(uint64_t needTermCount, uint64_t needDataSize);
int checkAndCleanHeaps(uint64_t needTermCount, uint64_t needDataSize);

#endif
