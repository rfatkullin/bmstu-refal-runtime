
#include <stdio.h>
#include <inttypes.h>

#include <gmp.h>

#include <vint.h>
#include <lterm.h>
#include <helpers.h>
#include <memory_manager.h>
#include <defines/errors_str.h>

static void printUStr(FILE* file, struct vstring_t* str);
static void printChain(FILE* file, struct lterm_t* chain);
static void printSymbol(FILE* file, struct vterm_t* term);
static void printIntNumber(FILE* file, struct vint_t* intNum);
static void printFragment(FILE* file, struct fragment_t* frag);
static void printFragmentTogether(FILE* file, struct fragment_t* frag);

void printFieldOfView(FILE* file, struct lterm_t* fov)
{
    printChain(file, fov);
    fprintf(file, "\n");
}

void printFragmentLn(FILE* file, struct fragment_t* frag)
{
    printFragment(file, frag);
    fprintf(file, "\n");
}

static void printFragment(FILE* file, struct fragment_t* frag)
{
    int i = 0;
    struct vterm_t* currTerm = _memMngr.vterms + frag->offset;

    for (i = 0; i < frag->length; ++i)
        printSymbol(file, currTerm + i);
}

static void printChain(FILE* file, struct lterm_t* chain)
{
    if (chain->tag != L_TERM_CHAIN_TAG)
        PRINT_AND_EXIT(DEBUG_PRINT_BAD_CHAIN);

    struct lterm_t* currTerm = chain->next;

    while (currTerm != chain)
    {
        switch (currTerm->tag)
        {
            case L_TERM_FRAGMENT_TAG :
            {
                printFragmentTogether(file, currTerm->fragment);
                break;
            }
            case L_TERM_CHAIN_KEEPER_TAG:
            {
                fprintf(file, "(");
                printChain(file, currTerm->chain);
                fprintf(file, ")");
                break;
            }

            case L_TERM_FUNC_CALL:
            {
                fprintf(file, "<");

                if (currTerm->funcCall->funcPtr)
                    printUStr(file, currTerm->funcCall->ident);

                if (currTerm->funcCall->fieldOfView)
                    printChain(file, currTerm->funcCall->fieldOfView);

                if (currTerm->funcCall->subCall)
                {
                    fprintf(file, "[[Func subCall:");
                    printChain(file, currTerm->funcCall->subCall);
                    fprintf(file, "]");
                }

                fprintf(file, ">");
                break;
            }

            default:
                PRINT_AND_EXIT(BAD_TAG_AT_ASSEMBLY);
        }

        currTerm = currTerm->next;
    }
}

static void printFragmentTogether(FILE* file, struct fragment_t* frag)
{
    int i = 0;
    struct vterm_t* currTerm = _memMngr.vterms + frag->offset;

    for (i = 0; i < frag->length; ++i)
        printSymbol(file, currTerm + i);
}

static void printSymbol(FILE* file, struct vterm_t* term)
{
    switch (term->tag)
    {
    case V_CHAR_TAG:
        printUTF32(file, term->ch);
        break;
    case V_IDENT_TAG:
        printUStr(file, term->str);
        fprintf(file, " ");
        break;
    case V_INT_NUM_TAG:
        printIntNumber(file, term->intNum);
        break;
    case V_DOUBLE_NUM_TAG:
        fprintf(file, "%lf ", term->doubleNum);
        break;
    case V_CLOSURE_TAG:
        printUStr(file, term->closure->ident);
        break;
    case V_BRACKETS_TAG:
        fprintf(file, "(");
        printFragment(file, term->brackets);
        fprintf(file, ")");
        break;
    }
}

static void printUStr(FILE* file, struct vstring_t* str)
{
    if (!str)
        return;

    uint64_t i = 0;

    for (i = 0; i < str->length; ++i)
        printUTF32(file, str->head[i]);
}

static void printIntNumber(FILE* file, struct vint_t* intNum)
{
    mpz_t num;
    mpz_init(num);

    mpz_import(num, GET_INT_LENGTH(intNum), 1, sizeof(uint8_t), 1, 0, intNum->bytes);

    if (GET_INT_SIGN(intNum))
        mpz_neg(num, num);

    gmp_fprintf(file, "%Zd ", num);

    mpz_clear(num);
}

