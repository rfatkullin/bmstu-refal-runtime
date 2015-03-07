#ifndef __V_MACHINE_H__
#define __V_MACHINE_H__

#include "lterm.h"
#include "func_call.h"

#define FIRST_CALL      0
#define ROLL_BACK       1
#define NEXT_ENTRYPOINT	2

#define BAD_EVAL_EXPR "Can't execute expr in evaluate brackets!"
#define FUNC_CALL_FAILED "Func call failed!"
#define MEMORY_OVERFLOW "Memory overflow!"

/// Главный цикл программы.
void mainLoop(const char*, RefalFunc);

/// Выполняет сборку lterm'ов в vterm'ы.
/// В процессе выполнения могут быть вызваны сборщики мусора vterm'ов и lterm'ов.
struct lterm_t* gcGetAssembliedChain(struct lterm_t* oldChain);

#endif
