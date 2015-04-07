#include <stdio.h>
#include <time.h>
#include <string.h>
#include <inttypes.h>

#include <helpers.h>
#include <vmachine.h>
#include <memory_manager.h>
#include <builtins/builtins.h>
#include <defines/gc_macros.h>
#include <defines/errors_str.h>
#include <allocators/data_alloc.h>
#include <defines/data_struct_sizes.h>

static void copyVTerms();
static void processVTermsInChain(struct lterm_t* expr);
static void processVTermsInFragment(struct fragment_t* frag);
static void processVTermsInFuncCall(struct func_call_t* funcCall);
static void processClosureVTerms(struct vclosure_t* closure);
static void processEnvVTerms(struct env_t* env);
static void copyClosureVTerm(uint64_t to, struct vclosure_t* closure);
static void copyIntVTerm(uint64_t to, struct vint_t* intNum);
static void printToCopyVTermsOffsets(uint64_t activeOffset);
static void copyBracketsVTerm(uint64_t to, struct fragment_t* frag);
static void setActualFragmentOffset(struct fragment_t* frag);
static void swap(uint64_t* a, uint64_t* b);
static void setActualDataInVTerms();

#define MARK_STAGE                  0
#define POINTER_CORRECTING_STAGE    1

static int stage;

void collectVTermGarbage(struct lterm_t* fieldOfView)
{    
    memset(_memMngr.gcInUseVTerms, 0, _memMngr.vtermsMaxOffset * sizeof(uint8_t));

    stage = MARK_STAGE;
    processVTermsInChain(fieldOfView);

    copyVTerms();

    stage = POINTER_CORRECTING_STAGE;
    setActualDataInVTerms();
    processVTermsInChain(fieldOfView);
}

static void processVTermsInChain(struct lterm_t* chain)
{
    struct lterm_t* currTerm = chain->next;

    while (currTerm != chain)
    {
        switch (currTerm->tag)
        {
            case L_TERM_FRAGMENT_TAG:
                processVTermsInFragment(currTerm->fragment);
                break;

            case L_TERM_CHAIN_KEEPER_TAG:
                processVTermsInChain(currTerm->chain);
                break;
            case L_TERM_FUNC_CALL:
                processVTermsInFuncCall(currTerm->funcCall);
                break;

            case L_TERM_CHAIN_TAG:
                PRINT_AND_EXIT(GC_VTERM_PROCESS_BAD_CHAIN_TAG);

            default:
                PRINT_AND_EXIT(GC_VTERM_PROCESS_BAD_TAG);
        }

        currTerm = currTerm->next;
    }
}

static void processVTermsInFragment(struct fragment_t* frag)
{
    uint64_t i = 0;
    uint64_t begin = frag->offset;
    uint64_t end = frag->offset + frag->length;

    if (stage == MARK_STAGE)
    {
        for (i = begin; i < end; ++i)
        {
            // Pass literals vterms. No need copy them.
            if (i < _memMngr.vtermsBeginOffset)
                continue;

            _memMngr.gcInUseVTerms[i - _memMngr.vtActiveOffset] = 1;

            switch (_memMngr.vterms[i].tag)
            {
                case V_CLOSURE_TAG:
                    processClosureVTerms(_memMngr.vterms[i].closure);
                    break;

                case V_BRACKETS_TAG:
                    processVTermsInFragment(_memMngr.vterms[i].brackets);
                    break;
            }
        }
    }
    else
    {
        setActualFragmentOffset(frag);
    }
}

static void setActualFragmentOffset(struct fragment_t* frag)
{
    // Fixed
    if (_memMngr.vtActiveOffset <= frag->offset && frag->offset < _memMngr.vtActiveOffset + _memMngr.vtermsMaxOffset)
        return;

    // Pass literals vterms --> No need fix reference to them.
    if (frag->offset < _memMngr.vtermsBeginOffset)
        return;

    // Nothing to copy. It can be fragment of expression variable.
    if (frag->length == 0)
        return;

    // Set new offset, stored in field brackets.
    frag->offset = (uint64_t)_memMngr.vterms[frag->offset].brackets; // Casting to hide warn! Get new offset from 'brackets' field.
}

static void processVTermsInFuncCall(struct func_call_t* funcCall)
{    
    if (funcCall->fieldOfView)
        processVTermsInChain(funcCall->fieldOfView);

    if (funcCall->subCall)
        processVTermsInChain(funcCall->subCall);    

    // Func called --> fovs inited
    if (funcCall->funcPtr)            
        processEnvVTerms(funcCall->env);    
}

static void processEnvVTerms(struct env_t* env)
{
    uint32_t i = 0;

    for (i = 0; i < env->fovsCount; ++i)
    {
        if (env->assembled[i])
            processVTermsInFragment(VTERM_BRACKETS(env->assembled[i]));
    }

    for (i = 0; i < env->localsCount; ++i)
        processVTermsInFragment(env->locals + i);

    for (i = 0; i < env->paramsCount; ++i)    
        processVTermsInFragment(env->params + i);


    if (env->workFieldOfView)
        processVTermsInChain(env->workFieldOfView);
}

static void processClosureVTerms(struct vclosure_t* closure)
{
    uint64_t i = 0;

    for (i = 0; i < closure->paramsCount; ++i)
        processVTermsInFragment(closure->params + i);
}

static void swap(uint64_t* a, uint64_t* b)
{
    uint64_t tmp = *a;
    *a = *b;
    *b = tmp;
}

static void printToCopyVTermsOffsets(uint64_t activeOffset)
{
    uint64_t toCopy = 0;
    uint64_t i = 0;

    for (i = 0; i < _memMngr.vtermsMaxOffset; ++i)
    {
        if (_memMngr.gcInUseVTerms[i])
        {
            printf("%" PRIu64 " ", activeOffset + i);
            toCopy++;
        }
    }

    printf("Total vterms to copy: %" PRIu64 "\n", toCopy);
}

static void copyVTerms()
{
    swap(&_memMngr.vtInactiveOffset, &_memMngr.vtActiveOffset);
    swap(&_memMngr.dtInactiveOffset, &_memMngr.dtActiveOffset);

    _memMngr.vtermsOffset = _memMngr.vtActiveOffset;
    _memMngr.dataOffset = _memMngr.dtActiveOffset;

    uint64_t i = 0;
    for (i = 0; i < _memMngr.vtermsMaxOffset; ++i)
    {
        if (_memMngr.gcInUseVTerms[i])
        {
            GC_VTERM_HEAP_CHECK_EXIT(1);

            uint64_t to = _memMngr.vtermsOffset;
            uint64_t from = _memMngr.vtInactiveOffset + i;

            switch (_memMngr.vterms[from].tag)
            {
                case V_INT_NUM_TAG:                    
                    copyIntVTerm(to, _memMngr.vterms[from].intNum);
                    break;
                case V_CLOSURE_TAG:
                    copyClosureVTerm(to, _memMngr.vterms[from].closure);
                    break;

                case V_BRACKETS_TAG:                    
                    copyBracketsVTerm(to, _memMngr.vterms[from].brackets);
                    break;

                case V_CHAR_TAG:
                case V_DOUBLE_NUM_TAG:
                    _memMngr.vterms[to] = _memMngr.vterms[from];
                    break;

                case V_IDENT_TAG:
                    // Ident structs only in literals data area --> no need to copy to new place. Just copy.
                    _memMngr.vterms[to] = _memMngr.vterms[from];
                    break;
            }

            _memMngr.vterms[to].tag = _memMngr.vterms[from].tag;
            _memMngr.vtermsOffset++;
            _memMngr.vterms[from].brackets = (struct fragment_t*)to; // Casting to hide warn! Save new offset in 'brackets' field.
        }
    }
}

static void setActualDataInVTerms()
{
    uint64_t i;
    for (i = _memMngr.vtActiveOffset; i < _memMngr.vtermsOffset; ++i)
    {
        // Pass literals vterms. No need copy them.
        if (i < _memMngr.vtermsBeginOffset)
            continue;

        switch (_memMngr.vterms[i].tag)
        {
            case V_CLOSURE_TAG:
            {
                processClosureVTerms(_memMngr.vterms[i].closure);
                break;
            }
            case V_BRACKETS_TAG:
            {
                setActualFragmentOffset(_memMngr.vterms[i].brackets);
                break;
            }
        }
    }
}

// TO FIX: Должно копироваться только один раз.
// Параметры копируются криво.
static void copyClosureVTerm(uint64_t to, struct vclosure_t* closure)
{    
    GC_DATA_HEAP_CHECK_EXIT(VCLOSURE_SIZE(closure->paramsCount));

    _memMngr.vterms[to].closure =
            allocateClosureStruct(closure->funcPtr, closure->paramsCount, closure->ident, closure->rollback);

    memcpy(_memMngr.vterms[to].closure->params, closure->params, closure->paramsCount * sizeof(struct fragment_t));
}

// TO FIX: Должно копироваться только один раз.
static void copyIntVTerm(uint64_t to, struct vint_t* intNum)
{
    GC_DATA_HEAP_CHECK_EXIT(VINT_STRUCT_SIZE(intNum->length));

    struct vint_t* newIntNum = allocateIntStruct(intNum->length);
    newIntNum->sign = intNum->sign;
    memcpy(newIntNum->bytes, intNum->bytes, intNum->length);
    _memMngr.vterms[to].intNum = newIntNum;
}

static void copyBracketsVTerm(uint64_t to, struct fragment_t* frag)
{
    GC_DATA_HEAP_CHECK_EXIT(FRAGMENT_STRUCT_SIZE(1));

    _memMngr.vterms[to].brackets = allocateFragment(1);
    memcpy(_memMngr.vterms[to].brackets, frag, FRAGMENT_STRUCT_SIZE(1));
}
