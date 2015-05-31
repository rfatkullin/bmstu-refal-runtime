#ifndef _BMSTU_REFC_DATA_STRUCT_SIZES_H_
#define _BMSTU_REFC_DATA_STRUCT_SIZES_H_

#include <vterm.h>
#include <lterm.h>
#include <vint.h>
#include <vstring.h>

#define VINT_STRUCT_SIZE(length)                (sizeof(struct vint_t) + length)

#define FUNC_CALL_LTERM_SIZE                    (sizeof(struct func_call_t) + sizeof(struct env_t) + sizeof(struct lterm_t))

#define FRAGMENT_LTERM_SIZE(count)              (count * (sizeof(struct lterm_t) + sizeof(struct fragment_t)))

#define FRAGMENT_STRUCT_SIZE(count)             (count * sizeof(struct fragment_t))

#define CHAIN_KEEPER_LTERM_SIZE(count)          (count * sizeof(struct lterm_t) + count * CHAIN_LTERM_SIZE)

#define CHAIN_LTERM_SIZE                        (sizeof(struct lterm_t))

#define BUILTINS_RESULT_SIZE                    (FRAGMENT_LTERM_SIZE(1) + CHAIN_LTERM_SIZE)

#define VCLOSURE_SIZE(paramsCount)              (sizeof(struct vclosure_t) + FRAGMENT_STRUCT_SIZE(paramsCount))

#define VSTRING_SIZE(length)                    (sizeof(struct vstring_t) + length * sizeof(uint32_t))

#define ENV_SIZE(localsCount, patternsCount, bracketsCount)                     \
        ( FRAGMENT_STRUCT_SIZE(localsCount)                     /*locals*/      \
        + patternsCount * sizeof(uint64_t)                      /*assembleds*/  \
        + patternsCount * sizeof(int)                           /*stretchs*/    \
        + bracketsCount * sizeof(uint64_t))                     /*brackets*/    \

#endif
