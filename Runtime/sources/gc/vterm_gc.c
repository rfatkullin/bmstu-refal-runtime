#include <stdio.h>
#include <time.h>
#include <string.h>

#include <memory_manager.h>
#include <allocators/data_alloc.h>
#include <builtins/builtins.h>

static void copyVTerms();
static void processChainVTerms(struct lterm_t* expr);
static void processFragmentVTerms(struct fragment_t* frag);
static void processFuncCallVTerms(struct func_call_t* funcCall);
static void processClosureVTerms(struct v_closure* closure);
static void processEnvVTerms(struct env_t* env);
static void copyClosureVTerm(uint64_t to, struct v_closure* closure);
static void copyIntVTerm(uint64_t to, struct v_int* intNum);

#define MARK_STAGE                  0
#define POINTER_CORRECTING_STAGE    1

static int stage;

void collectVTermGarbage(struct lterm_t* fieldOfView)
{
    memset(memMngr.gcInUseVTerms, 0, memMngr.vtermsMaxOffset * sizeof(uint8_t));

    stage = MARK_STAGE;
    processChainVTerms(fieldOfView);
    processFragmentVTerms(assembledFragInBuiltins->fragment);

    copyVTerms();

    stage = POINTER_CORRECTING_STAGE;
    processChainVTerms(fieldOfView);
    processFragmentVTerms(assembledFragInBuiltins->fragment);
}

static void processChainVTerms(struct lterm_t* chain)
{
    struct lterm_t* currTerm = chain->next;

    while (currTerm != chain)
    {
        switch (currTerm->tag)
        {
            case L_TERM_FRAGMENT_TAG:
                processFragmentVTerms(currTerm->fragment);
                break;

            case L_TERM_CHAIN_TAG:
                processChainVTerms(currTerm);
                break;
            case L_TERM_FUNC_CALL:
                processFuncCallVTerms(currTerm->funcCall);
                break;
        }

        currTerm = currTerm->next;
    }
}

static void processFragmentVTerms(struct fragment_t* frag)
{
    uint64_t i = 0;
    uint64_t end = frag->offset + frag->length;

    if (stage == MARK_STAGE)
    {
        for (i = frag->offset; i < end; ++i)
        {
            memMngr.gcInUseVTerms[i - memMngr.vtActiveOffset] = 1;

            if (memMngr.vterms[i].tag == V_CLOSURE_TAG)
                processClosureVTerms(memMngr.vterms[i].closure);
        }
    }
    else
    {
        // Ставим новый offset.
        frag->offset = memMngr.vterms[frag->offset].inBracketLength;
    }
}


static void processFuncCallVTerms(struct func_call_t* funcCall)
{
    if (funcCall->fieldOfView)
        processChainVTerms(funcCall->fieldOfView);

    if (funcCall->subCall)
        processChainVTerms(funcCall->subCall);

    processEnvVTerms(funcCall->env);
}

static void processEnvVTerms(struct env_t* env)
{
    uint32_t i = 0;
    for (i = 0; i < env->fovsCount; ++i)
    {
        processChainVTerms(env->fovs[i]);
        processFragmentVTerms(env->assembledFOVs[i]->fragment);
    }

    for (i = 0; i < env->paramsCount; ++i)
        processFragmentVTerms((env->params + i)->fragment);
}

static void processClosureVTerms(struct v_closure* closure)
{
    uint64_t i = 0;

    for (i  = 0; i < closure->paramsCount; ++i)
        processFragmentVTerms((closure->params + i)->fragment);
}


static void copyVTerms()
{
    uint64_t i = 0;

    memMngr.vtermsOffset = memMngr.vtInactiveOffset;
    memMngr.dataOffset = memMngr.dtInactiveOffset;

    for (i = 0; i < memMngr.vtermsMaxOffset; ++i)
    {
        if (memMngr.gcInUseVTerms[i])
        {
            uint64_t to = memMngr.vtermsOffset;
            uint64_t from = memMngr.vtActiveOffset + i;

            memMngr.vterms[to] = memMngr.vterms[from];
            memMngr.vterms[to].inBracketLength = memMngr.vtermsOffset;

            switch (memMngr.vterms[to].tag)
            {
                case V_INT_NUM_TAG:
                    copyIntVTerm(to, memMngr.vterms[from].intNum);
                    break;
                case V_CLOSURE_TAG:
                    copyClosureVTerm(to, memMngr.vterms[from].closure);
                    break;
            }

            memMngr.vtermsOffset++;
        }
    }
}

static void copyClosureVTerm(uint64_t to, struct v_closure* closure)
{
    memMngr.vterms[memMngr.vtermsOffset].closure =
            allocateClosureStruct(closure->funcPtr, closure->paramsCount, closure->ident, closure->rollback);
}

static void copyIntVTerm(uint64_t to, struct v_int* intNum)
{
    struct v_int* newIntNum = allocateIntStruct(intNum->length);
    memcpy(newIntNum, intNum, sizeof(struct v_int) + intNum->length);
    memMngr.vterms[memMngr.vtermsOffset].intNum = newIntNum;
}
