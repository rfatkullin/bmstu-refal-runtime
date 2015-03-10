#ifndef __V_MACHINE_H__
#define __V_MACHINE_H__

#include "lterm.h"
#include "func_call.h"

#define FIRST_CALL      0
#define ROLL_BACK       1
#define NEXT_ENTRYPOINT	2

#define BAD_EVAL_EXPR       "Can't execute expr in evaluate brackets!"
#define FUNC_CALL_FAILED    "Func call failed!"
#define MEMORY_OVERFLOW     "Memory overflow!"

#define ALLOCATE_CHECK(var, expr, statusVar)    \
do{                                             \
    var = expr;                                 \
    if (statusVar == NEED_CLEAN)                \
    {                                           \
        statusVar = 0;                          \
        continue;                               \
    }                                           \
}while(0)

/// Главный цикл программы.
void mainLoop(const char*, RefalFunc);

/// Выполняет сборку lterm'ов в vterm'ы.
/// В процессе выполнения могут быть вызваны сборщики мусора vterm'ов и lterm'ов.
struct lterm_t* gcGetAssembliedChain(struct lterm_t* oldChain);

#endif
