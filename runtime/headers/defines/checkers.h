#ifndef _BMSTU_REFC_CHECKERS_H_
#define _BMSTU_REFC_CHECKERS_H_

#define VTERM_BRACKETS(vtermInd)    (_memMngr.vterms[vtermInd].brackets)
#define BUILTIN_FRAG                (VTERM_BRACKETS(CURR_FUNC_CALL->env->assembled[0]))


#define CHECK_CHAR_LIT(i, j)    (_memMngr.vterms[i].tag == V_CHAR_TAG && _memMngr.vterms[i].ch == _memMngr.vterms[j].ch)

#define CHECK_COMP_LIT(i, j)    ((_memMngr.vterms[i].tag == V_IDENT_TAG && ustrEq(_memMngr.vterms[i].str, _memMngr.vterms[j].str)) || \
                                 (_memMngr.vterms[i].tag == V_CLOSURE_TAG && ustrEq(_memMngr.vterms[i].closure->ident, _memMngr.vterms[j].str)))

#define CHECK_DOUBLE_LIT(i, j)  (_memMngr.vterms[i].tag == V_DOUBLE_NUM_TAG && !doubleCmp(_memMngr.vterms[i].doubleNum, _memMngr.vterms[j].doubleNum))

#define CHECK_INT_LIT(i, j)     (_memMngr.vterms[i].tag == V_INT_NUM_TAG && !intCmp(_memMngr.vterms[i].intNum, _memMngr.vterms[j].intNum))

#define CHECK_BR(i)             (_memMngr.vterms[i].tag == V_BRACKETS_TAG)

#define CHECK_SYMB_VAR(i, j)    (_memMngr.vterms[i].tag != V_BRACKETS_TAG && eqSymbol(i, j))

#define CHECK_TERM_VAR(i, j)    ((_memMngr.vterms[i].tag == V_BRACKETS_TAG && eqFragment(VTERM_BRACKETS(i)->offset, VTERM_BRACKETS(j)->offset, VTERM_BRACKETS(i)->length)) || \
                                    eqSymbol(i, j))

#endif
