#ifndef _BMSTU_REFC_ALLOC_MACROS_H_
#define _BMSTU_REFC_ALLOC_MACROS_H_

#include <string.h>

#include <lterm.h>
#include <vterm.h>
#include <func_call.h>

#define ALLC_SIMPL_CHAIN(res)                                       \
    res = (struct lterm_t*)(_memMngr.data + _memMngr.dataOffset);   \
    _memMngr.dataOffset += sizeof(struct lterm_t);                  \
    res->tag = L_TERM_CHAIN_TAG;                                    \
    res->prev = res;                                                \
    res->next = res;

#endif


#define ALLC_FUNC_LTERM(res)                                                    \
    res = (struct lterm_t*)(_memMngr.data + _memMngr.dataOffset);               \
    _memMngr.dataOffset +=  sizeof(struct lterm_t);                             \
    res->tag = L_TERM_FUNC_CALL;                                                \
    res->funcCall = (struct func_call_t*)(_memMngr.data + _memMngr.dataOffset); \
    _memMngr.dataOffset += sizeof(struct func_call_t);                          \
    memset(res->funcCall, 0, sizeof(struct func_call_t));                     \
    res->funcCall->env = (struct env_t*)(_memMngr.data + _memMngr.dataOffset);  \
    _memMngr.dataOffset += sizeof(struct env_t);                                \
    memset(res->funcCall->env, 0, sizeof(struct env_t));

#define ALLC_FRAG_STRUCT_N(res, count)                                  \
    res = (struct fragment_t*)(_memMngr.data + _memMngr.dataOffset);    \
    _memMngr.dataOffset += count * sizeof(struct fragment_t);


#define ALLC_FRAG_LTERM(res)                                                    \
    res = (struct lterm_t*)(_memMngr.data + _memMngr.dataOffset);               \
    _memMngr.dataOffset += sizeof(struct lterm_t);                              \
    res->tag = L_TERM_FRAGMENT_TAG;                                             \
    res->fragment = (struct fragment_t*)(_memMngr.data + _memMngr.dataOffset);  \
    _memMngr.dataOffset += sizeof(struct fragment_t);

#define ALLC_CLOSURE_STRUCT(res, realFuncPtr, realParamsCount, realIdent, realRollBack) \
    res = (struct vclosure_t*)(_memMngr.data + _memMngr.dataOffset);                    \
    _memMngr.dataOffset += sizeof(struct vclosure_t);                                   \
    ALLC_FRAG_STRUCT_N(res->params, realParamsCount);                                   \
    res->funcPtr = realFuncPtr;                                                         \
    res->ident = realIdent;                                                             \
    res->paramsCount = realParamsCount;                                                 \
    res->rollback = realRollBack;

#define ALLC_CLOSURE_VTERM                                      \
    _memMngr.vterms[_memMngr.vtermsOffset].tag = V_CLOSURE_TAG; \
    _memMngr.vtermsOffset++;


#define ALLC_CHAIN_KEEPER_LTERM_N(res, count)                       \
do                                                                  \
{                                                                   \
    res = (struct lterm_t*)(_memMngr.data + _memMngr.dataOffset);   \
    _memMngr.dataOffset += count * sizeof(struct lterm_t);          \
    struct lterm_t* tmp = res;                                      \
    uint32_t i = 0;                                                 \
    for (i = 0; i < count; ++i)                                     \
    {                                                               \
        tmp->tag = L_TERM_CHAIN_KEEPER_TAG;                         \
        ALLC_SIMPL_CHAIN(tmp->chain)                                \
        tmp++;                                          \
    }                                                               \
}                                                                   \
while(0)
