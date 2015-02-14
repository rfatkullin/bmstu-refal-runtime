#ifndef __V_MACHINE_H__
#define __V_MACHINE_H__

#include "lterm.h"
#include "func_call.h"

void mainLoop(const char*, RefalFunc);
struct lterm_t* getAssembliedChain(struct lterm_t* oldChain);

#endif
