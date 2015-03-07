#ifndef __V_MACHINE_H__
#define __V_MACHINE_H__

#include "lterm.h"
#include "func_call.h"

#define FIRST_CALL      0
#define ROLL_BACK       1
#define NEXT_ENTRYPOINT	2

#define BAD_EVAL_EXPR "Can't execute expr in evaluate brackets!"
#define FUNC_CALL_FAILED "Func call failed!"


#define CHECK_SUCCESS(val) {if (!val) return 0;}

void mainLoop(const char*, RefalFunc);
struct lterm_t* getAssembliedChain(struct lterm_t* oldChain);

#endif
