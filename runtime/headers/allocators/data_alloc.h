#ifndef __DATA_ALLOCATORS_H___
#define __DATA_ALLOCATORS_H___

#include <lterm.h>
#include <vterm.h>
#include <gc/gc.h>

#define FOV_CONTAINS_FUNC_CALL      "FieldOfView passed in funcation contains func call term!\n"
#define FOV_CONTAINS_SIMPLE_CHAIN   "FieldOfView can't contains simple chain without chain keeper!\n"

/// Выделяют память в хипе данных
/// Статус выполнения возвращается в аргументе res.
/// Если памяти хватило и она выделена,то в *res будет Ok,
/// в противном случае NEED_DATA_CLEAN
struct lterm_t*     chAllocateFragmentLTerm(uint32_t count, allocate_result* res);
struct lterm_t*     chAllocateChainKeeperLTerm(uint32_t count, allocate_result* res);
struct lterm_t*     chAllocateSimpleChainLTerm(allocate_result* res);
struct lterm_t*     chAllocateFuncCallLTerm(allocate_result* res);
struct lterm_t*     chAllocateBuiltinsResult(uint64_t offset, uint64_t length, allocate_result* res);
struct lterm_t*     chCopyFieldOfView(struct lterm_t* fieldOfView, allocate_result* restrict);
struct vclosure_t*  chAllocateClosureStruct(RefalFunc funcPtr, uint32_t paramsCount, struct vstring_t* ident, int rollback, allocate_result* res);
struct vstring_t*   chAllocateVStringFromASCIIName(const char* name, allocate_result* res);

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
struct env_t* initEnvData(struct env_t* env, uint32_t localsCount, uint32_t patternsCount, uint32_t bracketsCount);
void clearCurrFuncEnvData();

// TO FIX:
struct lterm_t*   allocateChainKeeperLTerm(uint32_t count);
struct lterm_t*   allocateSimpleChain();

/// Выделяют память с помощью malloc'а
struct vstring_t* allocateVStringLiteral(uint32_t* runes, uint64_t length);
struct vint_t* allocateIntNumberLiteral(uint8_t* bytes, uint8_t sign, uint64_t length);

struct fragment_t* allocateFragment(uint32_t count);

#endif
