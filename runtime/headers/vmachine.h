#ifndef _BMSTU_REFC_V_MACHINE_H_
#define _BMSTU_REFC_V_MACHINE_H_

#include <gc/gc.h>
#include <lterm.h>
#include <func_call.h>
#include <defines/gc_macros.h>

#define FIRST_CALL      1
#define ROLL_BACK       2
#define NEXT_ENTRYPOINT	3

#define CURR_FUNC_CALL              (_currCallTerm->funcCall)
#define VTERM_BRACKETS(vtermInd)    (_memMngr.vterms[vtermInd].brackets)
#define RIGHT_BOUND(vtermInd)       (_memMngr.vterms[vtermInd].brackets->offset + _memMngr.vterms[vtermInd].brackets->length)

#define ASSEMBLY_FIELD(index, chain)                                \
do{                                                                 \
    allocate_result res;                                            \
    uint64_t tmpOffset;                                             \
    DOUBLE_TRY(tmpOffset, chGetAssembliedChain(chain, &res), res);  \
    CURR_FUNC_CALL->env->assembled[index] = tmpOffset;              \
}while(0)

/// Главный цикл.
/// entryFuncName - имя входной функции ("Go" или "GO").
/// entryFuncPointer - указатель на входную функцию.
void mainLoop(const char* entryFuncName, RefalFunc entryFuncPointer);

/// Выполняет сборку lterm'ов в vterm'ы. Статус выполнения записывается в *res.
/// При успешном завершении *res == GC_OK, вернет смещение vterm'а, который
/// хранит собранный фрагмент vterm'ов.
/// При нехватке памяти *res == GC_NEED_CLEAN, вернет 0.
uint64_t chGetAssembliedChain(struct lterm_t* chain, allocate_result *res);

/// Чистит все промежуточные данные.
void clearCurrFuncEnvData();

/// Указатель на текущий функциональный lterm.
struct lterm_t* _currCallTerm;

/// Копирует объектное выражение представленное в виде головного l-терма.
struct lterm_t* chCopySimpleExpr(struct lterm_t* chain, allocate_result* res);

#endif
