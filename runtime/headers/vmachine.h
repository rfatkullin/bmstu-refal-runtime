#ifndef __V_MACHINE_H__
#define __V_MACHINE_H__

#include <gc/gc.h>
#include <lterm.h>
#include <func_call.h>

#define FIRST_CALL      0
#define ROLL_BACK       1
#define NEXT_ENTRYPOINT	2

#define CURR_FUNC_CALL              (_currCallTerm->funcCall)
#define VTERM_BRACKETS(vtermInd)    (_memMngr.vterms[vtermInd].brackets)
#define RIGHT_BOUND(vtermInd)       (_memMngr.vterms[vtermInd].brackets->offset + _memMngr.vterms[vtermInd].brackets->length)

#define ASSEMBLY_FIELD(index, chain)                                \
do{                                                                 \
    allocate_result res;                                            \
    uint64_t tmpOffset;                                             \
    DOUBLE_TRY(tmpOffset, gcGetAssembliedChain(chain, &res), res);  \
    CURR_FUNC_CALL->env->assembled[index] = tmpOffset;              \
}while(0)

/// Main loop
/// entryFuncName - entry func name ("Go" or "GO")
/// entryFuncPointer - C pointer on entry func
void mainLoop(const char* entryFuncName, RefalFunc entryFuncPointer);

/// Выполняет сборку lterm'ов в vterm'ы.
/// В процессе выполнения могут быть вызваны сборщики мусора vterm'ов и lterm'ов.
uint64_t gcGetAssembliedChain(struct lterm_t* chain, allocate_result *res);

struct lterm_t* _currCallTerm;

#endif
