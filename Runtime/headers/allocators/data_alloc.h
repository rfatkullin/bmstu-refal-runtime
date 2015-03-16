#ifndef __DATA_ALLOCATORS_H___
#define __DATA_ALLOCATORS_H___

#include <lterm.h>
#include <vterm.h>
#include <gc/gc.h>

#define VINT_STRUCT_SIZE(length)        (sizeof(struct v_int) + length)
#define FRAGMENT_LTERM_SIZE(count)      (count * (sizeof(struct lterm_t) + sizeof(struct fragment_t)))
#define CHAIN_LTERM_SIZE(count)         (2 * count * sizeof(struct lterm_t))
#define SIMPLE_CHAIN_SIZE               (sizeof(struct lterm_t))
#define BUILTINS_RESULT_SIZE            (FRAGMENT_LTERM_SIZE(1) + SIMPLE_CHAIN_SIZE)
#define CLOSURE_SIZE(paramsCount)       (sizeof(struct v_closure) + paramsCount * sizeof(struct lterm_t))
#define VSTRING_SIZE(length)            (sizeof(struct v_string) + length * sizeof(uint32_t))

/// Выделяют память в хипе данных
/// Статус выполнения возвращается в аргументе res.
/// Если памяти хватило и она выделена,то в *res будет Ok,
/// в противном случае NEED_DATA_CLEAN
struct lterm_t*   chAllocateFragmentLTerm(uint32_t count, allocate_result* res);
struct lterm_t*   chAllocateChainLTerm(uint32_t count, allocate_result* res);
struct lterm_t*   chAllocateFuncCallLTerm(allocate_result* res);
struct lterm_t*   chAllocateBuiltinsResult(uint64_t offset, uint64_t length, allocate_result* res);
struct v_closure* chAllocateClosureStruct(RefalFunc funcPtr, uint32_t paramsCount, struct v_string* ident, int rollback, allocate_result* res);

/// Выделяют память в хипе данных
/// В процессе выполнения может быть вызван сборщик мусора для данных.
struct lterm_t*   gcAllocateFragmentLTerm(uint32_t count);
struct v_int*     gcAllocateIntStruct(uint64_t length);
struct env_t*     gcAllocateEnvData(struct env_t* env, uint32_t localsCount, uint32_t patternsCount);
struct v_closure* gcAllocateClosureStruct(RefalFunc funcPtr, uint32_t paramsCount, struct v_string* ident, int rollback);

/// Выделяют память в хипе данных без проверок
struct v_int*     allocateIntStruct(uint64_t length);
struct lterm_t*   allocateBuiltinsResult(uint64_t offset, uint64_t length);
struct v_closure* allocateClosureStruct(RefalFunc funcPtr, uint32_t paramsCount, struct v_string* ident, int rollback);
struct lterm_t*   allocateFragmentLTerm(uint32_t count);
struct lterm_t*   allocateFuncCallLTerm();
struct env_t*     allocateEnvData(struct env_t* env, uint32_t localsCount, uint32_t patternsCount);

// TO FIX:
struct lterm_t*   allocateChainLTerm(uint32_t count);
struct lterm_t*   allocateSimpleChain();

/// Выделяют память с помощью malloc'а
struct v_string* allocateVStringLiteral(uint32_t* runes, uint64_t length);
struct v_int* allocateIntNumberLiteral(uint8_t* bytes, uint8_t sign, uint64_t length);

#endif
