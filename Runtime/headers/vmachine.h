#ifndef __V_MACHINE_H__
#define __V_MACHINE_H__

#include "gc/gc.h"
#include "lterm.h"
#include "func_call.h"

#define FIRST_CALL      0
#define ROLL_BACK       1
#define NEXT_ENTRYPOINT	2

#define DEFAULT_HEAP_SIZE UINT64_C(1024 * 1024 * 1024)

#define BAD_EVAL_EXPR               "Can't execute expr in evaluate brackets!\n"
#define FUNC_CALL_FAILED            "Func call failed!\n"
#define ASSEMBLY_NOT_CHAIN          "Trying assembly not chain!\n"
#define SIMPLE_CHAIN_AT_ASSEMBLY    "Chain tag when assembly. Chains contains only chains keepers, not chains!\n"
#define BAD_TAG_AT_ASSEMBLY         "Bad tag when assembly!\n"
#define FUNC_CALL_AT_ASSEMBLY       "Func call term when assembly!\n"

#define CURR_FUNC_CALL (_currCallTerm->funcCall)

#define VTERM_BRACKETS(vtermInd) (_memMngr.vterms[vtermInd].brackets)

#define RIGHT_BOUND(vtermInd) (_memMngr.vterms[vtermInd].brackets->offset + _memMngr.vterms[vtermInd].brackets->length)

#define ASSEMBLY_FIELD(index, chain)                                \
do{                                                                 \
    allocate_result res;                                            \
    uint64_t tmpOffset;                                             \
    DOUBLE_TRY(tmpOffset, gcGetAssembliedChain(chain, &res), res);  \
    CURR_FUNC_CALL->env->assembled[index] = tmpOffset;              \
}while(0)

/// Главный цикл программы.
void mainLoop(const char*, RefalFunc);

/// Выполняет сборку lterm'ов в vterm'ы.
/// В процессе выполнения могут быть вызваны сборщики мусора vterm'ов и lterm'ов.
uint64_t gcGetAssembliedChain(struct lterm_t* chain, allocate_result *res);

struct lterm_t* _currCallTerm;

uint64_t getHeapSize(int argc, char** argv);

int eqSymbol(uint64_t a, uint64_t b);
int eqFragment(uint64_t a, uint64_t b, uint64_t length);

#endif
