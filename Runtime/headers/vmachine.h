#ifndef __V_MACHINE_H__
#define __V_MACHINE_H__

#include "lterm.h"
#include "func_call.h"

#define FIRST_CALL      0
#define ROLL_BACK       1
#define NEXT_ENTRYPOINT	2

#define BAD_EVAL_EXPR               "Can't execute expr in evaluate brackets!"
#define FUNC_CALL_FAILED            "Func call failed!\n"
#define MEMORY_OVERFLOW             "Memory overflow!"
#define ASSEMBLY_NOT_CHAIN          "Trying assembly not chain!\n"
#define SIMPLE_CHAIN_AT_ASSEMBLY    "Chain tag when assembly. Chains contains only chains keepers, not chains!\n"
#define BAD_TAG_AT_ASSEMBLY         "Bad tag when assembly!\n"
#define FUNC_CALL_AT_ASSEMBLY       "Func call term when assembly!\n"

#define CURR_FUNC_CALL (_currCallTerm->funcCall)

/// Главный цикл программы.
void mainLoop(const char*, RefalFunc);

/// Выполняет сборку lterm'ов в vterm'ы.
/// В процессе выполнения могут быть вызваны сборщики мусора vterm'ов и lterm'ов.
struct lterm_t* gcGetAssembliedChain(struct lterm_t* oldChain);

//struct func_call_t* _currFuncCall;
struct lterm_t* _currCallTerm;

#endif
