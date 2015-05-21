#include <stdio.h>
#include <time.h>
#include <string.h>
#include <inttypes.h>

#include <vmachine.h>
#include <memory_manager.h>
#include <defines/gc_macros.h>
#include <defines/errors_str.h>

static void setActualDataInVTerms();
static void processEnvVTerms(struct env_t* env);
static void processVTermsInChain(struct lterm_t* expr);
static void setActualFragmentOffset(struct fragment_t* frag);
static void processVTermsInFragment(struct fragment_t* frag);
static void processClosureVTerms(struct vclosure_t* closure);
static void processVTermsInFuncCall(struct func_call_t* funcCall);

void collectVTermGarbage()
{    
    memset(_gc.inUseVTerms, 0, _memMngr.vtermsMaxOffset * sizeof(uint8_t));

    _gc.stage = GC_VTERMS_MARK_STAGE;
    processVTermsInChain(_memMngr.fieldOfView);

    copyVTerms();

    _gc.stage = GC_VTERMS_SET_ACTUAL_STAGE;
    setActualDataInVTerms();
    processVTermsInChain(_memMngr.fieldOfView);
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

            default:
                PRINT_AND_EXIT(GC_VTERM_PROCESS_BAD_TAG);
        }

        currTerm = currTerm->next;
    }
}

static void processVTermsInFuncCall(struct func_call_t* funcCall)
{
    if (funcCall->fieldOfView)
        processVTermsInChain(funcCall->fieldOfView);

    if (funcCall->subCall)
        processVTermsInChain(funcCall->subCall);

    // Проставлен указатель на функцию --> функция вызывалась --> окружение функции инициализировано.
    if (funcCall->funcPtr)
        processEnvVTerms(funcCall->env);
}

static void processEnvVTerms(struct env_t* env)
{
    uint32_t i = 0;

    for (i = 0; i < env->paramsCount; ++i)
        processVTermsInFragment(env->params + i);

    for (i = 0; i < env->localsCount; ++i)
        processVTermsInFragment(env->locals + i);

    if (env->workFieldOfView)
        processVTermsInChain(env->workFieldOfView);

    for (i = 0; i < env->fovsCount; ++i)
    {
        if (env->assembled[i])
        {
            uint64_t offset = env->assembled[i];

            if (_gc.stage == GC_VTERMS_MARK_STAGE)
            {
                // Пропускаем литеральные vterm'ы.
                if (!(offset < _memMngr.vtermsBeginOffset))
                    _gc.inUseVTerms[offset - _memMngr.vtActiveOffset] = 1;

                processVTermsInFragment(VTERM_BRACKETS(env->assembled[i]));
            }
            else
            {
                // Пропускаем фрагменты, которые указывают на литеральные vterm'ы.
                if (offset < _memMngr.vtermsBeginOffset)
                    continue;

                // Если смещение указывает на новую кучу, значит фрагмент уже обработан - пропускаем.
                if (_memMngr.vtActiveOffset <= offset && offset < _memMngr.vtActiveOffset + _memMngr.vtermsMaxOffset)
                    return;

                // Подправляем смещение.
                env->assembled[i] = (uint64_t)_memMngr.vterms[offset].brackets; // Приходтся кастить, чтобы избежать предупреждений.

                processVTermsInFragment(VTERM_BRACKETS(env->assembled[i]));
            }
        }
    }
}

static void processVTermsInFragment(struct fragment_t* frag)
{
    uint64_t i = 0;
    uint64_t begin = frag->offset;
    uint64_t end = frag->offset + frag->length;

    // Пропускаем, так как длина нулевая. Такое возможно для переменных типа e.
    if (frag->length == 0)
        return;

    CHECK_ON_ZERO_VTERM_OFFSET_EXIT(begin);

    // Маркируем vterm'ы, которые нужно скопировать в новую кучу.
    if (_gc.stage == GC_VTERMS_MARK_STAGE)
    {
        for (i = begin; i < end; ++i)
        {
            // Пропускаем литеральные vterm'ы.
            if (i < _memMngr.vtermsBeginOffset)
                continue;

            _gc.inUseVTerms[i - _memMngr.vtActiveOffset] = 1;

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
    else  // Ставим актуальные смещения в фрагментных lterm'ах.
    {
        setActualFragmentOffset(frag);
    }
}

static void processClosureVTerms(struct vclosure_t* closure)
{
    uint64_t i = 0;

    for (i = 0; i < closure->paramsCount; ++i)
        processVTermsInFragment(closure->params + i);
}

/// Подправляет данные в vterm'ах - проставляет
/// актуальные смещения. Фишка в том, что данные в vterm'ах
/// редактируются всего один раз, что невозможно обеспечить
/// при использовании функции processVTermsInChain.
static void setActualDataInVTerms()
{
    uint64_t i;
    for (i = _memMngr.vtActiveOffset; i < _memMngr.vtermsOffset; ++i)
    {
        // Пропускаем литеральные vterm'ы. Они никогда не перемещаются.
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

static void setActualFragmentOffset(struct fragment_t* frag)
{
    // Пропускаем, так как длина нулевая. Такое возможно для переменных типа e.
    if (frag->length == 0)
        return;

    // Пропускаем фрагменты, которые указывают на литеральные vterm'ы.
    if (frag->offset < _memMngr.vtermsBeginOffset)
        return;

    // Если смещение указывает на новую кучу, значит фрагмент уже обработан - пропускаем.
    if (_memMngr.vtActiveOffset <= frag->offset && frag->offset < _memMngr.vtActiveOffset + _memMngr.vtermsMaxOffset)
        return;

    // Подправляем смещение.
    frag->offset = (uint64_t)_memMngr.vterms[frag->offset].brackets; // Приходтся кастить, чтобы избежать предупреждений.
}
