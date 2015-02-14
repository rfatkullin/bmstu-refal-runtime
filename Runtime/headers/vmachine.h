#ifndef __V_MACHINE_H__
#define __V_MACHINE_H__

#include "lterm.h"
#include "func_call.h"

typedef struct func_result_t (*RefalFunc)(int*, struct env_t*, struct lterm_t*);

void mainLoop(RefalFunc);
struct lterm_t* getAssembliedChain(struct lterm_t* oldChain);

#endif
