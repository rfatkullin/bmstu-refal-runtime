#ifndef __DATA_ALLOCATORS_H___
#define __DATA_ALLOCATORS_H___

#include <lterm.h>
#include <vterm.h>
#include <gc/gc.h>

/// Следующие функции создают объект и инициализируют его.
/// При этом делают проверку на переполнение памяти.
/// В аргументе *res возвращается результат выделения.
/// В случае успеха *res == GC_OK, возвращается созданный объект.
/// В случае неудачи *res == NEED_DATA_CLEAN, возвращается нулевой указатель.
struct lterm_t*     chAllocateFragmentLTerm(uint32_t count, allocate_result* res);
struct lterm_t*     chAllocateChainKeeperLTerm(uint32_t count, allocate_result* res);
struct lterm_t*     chAllocateSimpleChainLTerm(allocate_result* res);
struct lterm_t*     chAllocateFuncCallLTerm(allocate_result* res);
struct lterm_t*     chAllocateBuiltinsResult(uint64_t offset, uint64_t length, allocate_result* res);
struct lterm_t*     chCopyFieldOfView(struct lterm_t* fieldOfView, allocate_result* restrict);
struct vclosure_t*  chAllocateClosureStruct(RefalFunc funcPtr, uint32_t paramsCount, struct vstring_t* ident, int rollback, allocate_result* res);
struct vstring_t*   chAllocateVStringFromASCIIName(const char* name, allocate_result* res);

/// Следующие функции создают объект и инициализируют его.
/// При этом НЕ делают проверку на переполнение памяти.
struct vint_t*      allocateIntStruct(uint64_t length);
struct lterm_t*     allocateBuiltinsResult(uint64_t offset, uint64_t length);
struct vclosure_t*  allocateClosureStruct(RefalFunc funcPtr, uint32_t paramsCount, struct vstring_t* ident, int rollback);
struct lterm_t*     allocateFragmentLTerm(uint32_t count);
struct lterm_t*     allocateFuncCallLTerm();
struct lterm_t*     allocateChainKeeperLTerm(uint32_t count);
struct lterm_t*     allocateSimpleChain();
struct fragment_t*  allocateFragment(uint32_t count);

/// Инициализирует окружение функции. Также выделяет память.
struct env_t*       initEnvData(struct env_t* env, uint32_t localsCount, uint32_t patternsCount, uint32_t bracketsCount);

/// Выделяют память с помощью malloc'а
struct vstring_t* allocateVStringLiteral(uint32_t* runes, uint64_t length);
struct vint_t* allocateIntNumberLiteral(uint8_t* bytes, uint8_t sign, uint64_t length);

#endif
