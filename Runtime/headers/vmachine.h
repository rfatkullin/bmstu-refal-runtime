#ifndef __V_MACHINE_H__
#define __V_MACHINE_H__

#include "lterm.h"
#include "func_call.h"

void mainLoop(struct func_result_t (*firstFuncPtr)(int entryPoint, struct env_t* env, struct lterm_chain_t* fieldOfView));
struct lterm_t* getAssembliedChain(struct lterm_chain_t* oldChain);

#endif
