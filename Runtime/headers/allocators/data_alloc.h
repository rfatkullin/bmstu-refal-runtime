#ifndef __DATA_ALLOCATORS_H___
#define __DATA_ALLOCATORS_H___

#include <lterm.h>
#include <vterm.h>
#include <gc/gc.h>

#define VINT_STRUCT_SIZE(length)                (sizeof(struct vint_t) + length)
#define FUNC_CALL_LTERM_SIZE                    (sizeof(struct func_call_t) + sizeof(struct env_t) + sizeof(struct lterm_t))
#define FRAGMENT_LTERM_SIZE(count)              (count * (sizeof(struct lterm_t) + sizeof(struct fragment_t)))
#define CHAIN_KEEPER_LTERM_SIZE(count)          (count * sizeof(struct lterm_t) + count * CHAIN_LTERM_SIZE)
#define CHAIN_LTERM_SIZE                        (sizeof(struct lterm_t))
#define BUILTINS_RESULT_SIZE                    (FRAGMENT_LTERM_SIZE(1) + CHAIN_LTERM_SIZE)
#define VCLOSURE_SIZE(paramsCount)              (sizeof(struct vclosure_t) + FRAGMENT_LTERM_SIZE(paramsCount))
#define VSTRING_SIZE(length)                    (sizeof(struct vstring_t) + length * sizeof(uint32_t))
#define ENV_SIZE(localsCount, patternsCount)    (FRAGMENT_LTERM_SIZE(localsCount) + patternsCount * sizeof(struct lterm_t*) + \
                                                patternsCount * sizeof(struct lterm_t*) + patternsCount * sizeof(int))


#define FOV_CONTAINS_FUNC_CALL      "FieldOfView passed in funcation contains func call term!\n"
#define FOV_CONTAINS_SIMPLE_CHAIN   "FieldOfView can't contains simple chain without chain keeper!\n"

/// Выделяют память в хипе данных
/// Статус выполнения возвращается в аргументе res.
/// Если памяти хватило и она выделена,то в *res будет Ok,
/// в противном случае NEED_DATA_CLEAN
struct lterm_t*   chAllocateFragmentLTerm(uint32_t count, allocate_result* res);
struct lterm_t*   chAllocateChainKeeperLTerm(uint32_t count, allocate_result* res);
struct lterm_t*   chAllocateSimpleChainLTerm(allocate_result* res);
struct lterm_t*   chAllocateFuncCallLTerm(allocate_result* res);
struct lterm_t*   chAllocateBuiltinsResult(uint64_t offset, uint64_t length, allocate_result* res);
struct vclosure_t* chAllocateClosureStruct(RefalFunc funcPtr, uint32_t paramsCount, struct vstring_t* ident, int rollback, allocate_result* res);
struct lterm_t*   chCopyFieldOfView(struct lterm_t* fieldOfView, allocate_result* restrict);

/// Выделяют память в хипе данных
/// В процессе выполнения может быть вызван сборщик мусора для данных.
struct lterm_t*   gcAllocateFragmentLTerm(uint32_t count);
struct vint_t*     gcAllocateIntStruct(uint64_t length);
struct vclosure_t* gcAllocateClosureStruct(RefalFunc funcPtr, uint32_t paramsCount, struct vstring_t* ident, int rollback);

/// Выделяют память в хипе данных без проверок
struct vint_t*     allocateIntStruct(uint64_t length);
struct lterm_t*   allocateBuiltinsResult(uint64_t offset, uint64_t length);
struct vclosure_t* allocateClosureStruct(RefalFunc funcPtr, uint32_t paramsCount, struct vstring_t* ident, int rollback);
struct lterm_t*   allocateFragmentLTerm(uint32_t count);
struct lterm_t*   allocateFuncCallLTerm();
struct env_t*     allocateEnvData(struct env_t* env, uint32_t localsCount, uint32_t patternsCount);

// TO FIX:
struct lterm_t*   allocateChainKeeperLTerm(uint32_t count);
struct lterm_t*   allocateSimpleChain();

/// Выделяют память с помощью malloc'а
struct vstring_t* allocateVStringLiteral(uint32_t* runes, uint64_t length);
struct vint_t* allocateIntNumberLiteral(uint8_t* bytes, uint8_t sign, uint64_t length);

#endif
