#include <stdio.h>
#include <time.h>
#include <string.h>
#include <inttypes.h>

#include <memory_manager.h>
#include <allocators/data_alloc.h>
#include <builtins/builtins.h>
#include <helpers.h>

static void copyVTerms();
static void processVTermsInChain(struct lterm_t* expr);
static void processVTermsInFragment(struct fragment_t* frag);
static void processVTermsInFuncCall(struct func_call_t* funcCall);
static void processClosureVTerms(struct v_closure* closure);
static void processEnvVTerms(struct env_t* env);
static void copyClosureVTerm(uint64_t to, struct v_closure* closure);
static void copyIntVTerm(uint64_t to, struct v_int* intNum);
static void printToCopyVTermsOffsets(uint64_t activeOffset);
static void swap(uint64_t* a, uint64_t* b);

#define MARK_STAGE                  0
#define POINTER_CORRECTING_STAGE    1

static int stage;

void collectVTermGarbage(struct lterm_t* fieldOfView)
{
    memset(memMngr.gcInUseVTerms, 0, memMngr.vtermsMaxOffset * sizeof(uint8_t));

    stage = MARK_STAGE;

    processVTermsInChain(fieldOfView);

    //TO FIX: Правильно обрабатывать _currFuncCall.
    //TO FIX: Будет обрабатываться в рамках _currFuncCall
    if (assembledFragInBuiltins)
        processVTermsInFragment(assembledFragInBuiltins->fragment);

    copyVTerms();

    stage = POINTER_CORRECTING_STAGE;    
    processVTermsInChain(fieldOfView);    

    //TO FIX: Правильно обрабатывать _currFuncCall.
    //TO FIX: Будет обрабатываться в рамках _currFuncCall
    if (assembledFragInBuiltins)
        processVTermsInFragment(assembledFragInBuiltins->fragment);
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
                processVTermsInChain(currTerm);
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
    uint64_t end = frag->offset + frag->length;

    if (stage == MARK_STAGE)
    {
        for (i = frag->offset; i < end; ++i)
        {
            // Pass literals vterms. No need copy them.
            if (i < memMngr.vtermsBeginOffset)
                continue;

            memMngr.gcInUseVTerms[i - memMngr.vtActiveOffset] = 1;

            if (memMngr.vterms[i].tag == V_CLOSURE_TAG)
                processClosureVTerms(memMngr.vterms[i].closure);

            //printf("%" PRIu64 " ", i - memMngr.vtActiveOffset);
        }
    }
    else
    {
        // Pass literals vterms --> No need fix reference to them.
        if (frag->offset < memMngr.vtermsBeginOffset)
            return;

        // Nothing to copy. It can be fragment of expression variable.
        if (frag->length == 0)
            return;

        /*printf("(%" PRIu64 ", %" PRIu64 ") ", frag->offset - memMngr.vtInactiveOffset, frag->length);

        if (!memMngr.gcInUseVTerms[frag->offset - memMngr.vtInactiveOffset])
        {
            printf("Warn %d!\n", frag->offset - memMngr.vtInactiveOffset);
            fflush(stdout);
        }
        */

        // Set new offset, stored in field inBracketLength.
        frag->offset = memMngr.vterms[frag->offset].inBracketLength;
    }
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
        if (env->fovs[i])
            processVTermsInChain(env->fovs[i]);
        if (env->assembledFOVs[i])
            processVTermsInFragment(env->assembledFOVs[i]->fragment);
    }

    for (i = 0; i < env->paramsCount; ++i)
        processVTermsInFragment(env->params[i].fragment);
}

static void processClosureVTerms(struct v_closure* closure)
{
    uint64_t i = 0;

    for (i  = 0; i < closure->paramsCount; ++i)
        processVTermsInFragment((closure->params + i)->fragment);
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

    for (i = 0; i < memMngr.vtermsMaxOffset; ++i)
    {
        if (memMngr.gcInUseVTerms[i])
        {
            printf("%" PRIu64 " ", activeOffset + i);
            toCopy++;
        }
    }

    printf("Total vterms to copy: %" PRIu64 "\n", toCopy);
}

static void copyVTerms()
{
    swap(&memMngr.vtInactiveOffset, &memMngr.vtActiveOffset);
    swap(&memMngr.dtInactiveOffset, &memMngr.dtActiveOffset);

    memMngr.vtermsOffset = memMngr.vtActiveOffset;
    memMngr.dataOffset = memMngr.dtActiveOffset;

    uint64_t i = 0;
    for (i = 0; i < memMngr.vtermsMaxOffset; ++i)
    {
        if (memMngr.gcInUseVTerms[i])
        {
            GC_VTERM_HEAP_CHECK_EXIT(1);

            uint64_t to = memMngr.vtermsOffset;
            uint64_t from = memMngr.vtInactiveOffset + i;            

            switch (memMngr.vterms[from].tag)
            {
                case V_INT_NUM_TAG:                    
                    copyIntVTerm(to, memMngr.vterms[from].intNum);
                    break;
                case V_CLOSURE_TAG:
                    copyClosureVTerm(to, memMngr.vterms[from].closure);
                    break;

                case V_BRACKET_CLOSE_TAG:
                case V_BRACKET_OPEN_TAG:
                case V_CHAR_TAG:
                case V_DOUBLE_NUM_TAG:
                    memMngr.vterms[to] = memMngr.vterms[from];
                    break;

                case V_IDENT_TAG:
                    // Ident structs only in literals data area --> no need to copy to new place. Just copy.
                    memMngr.vterms[to] = memMngr.vterms[from];
                    break;
            }

            memMngr.vterms[from].inBracketLength = memMngr.vtermsOffset;
            memMngr.vtermsOffset++;
        }
    }
}

// TO FIX: Должно копироваться только один раз.
static void copyClosureVTerm(uint64_t to, struct v_closure* closure)
{    
    GC_DATA_HEAP_CHECK_EXIT(VCLOSURE_SIZE(closure->paramsCount));

    memMngr.vterms[memMngr.vtermsOffset].closure =
            allocateClosureStruct(closure->funcPtr, closure->paramsCount, closure->ident, closure->rollback);
}

// TO FIX: Должно копироваться только один раз.
static void copyIntVTerm(uint64_t to, struct v_int* intNum)
{
    GC_DATA_HEAP_CHECK_EXIT(VINT_STRUCT_SIZE(intNum->length));

    struct v_int* newIntNum = allocateIntStruct(intNum->length);
    newIntNum->sign = intNum->sign;
    memcpy(newIntNum->bytes, intNum->bytes, intNum->length);
    memMngr.vterms[memMngr.vtermsOffset].intNum = newIntNum;
}




