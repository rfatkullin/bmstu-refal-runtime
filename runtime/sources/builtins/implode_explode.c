#include <vmachine.h>
#include <builtins/builtins.h>
#include <allocators/data_alloc.h>
#include <defines/data_struct_sizes.h>

static int implodeAllowableChar(uint32_t ch);
static int implodeExtAllowableChar(uint32_t ch);
static struct func_result_t innerExplode(const char* funcName);
static struct func_result_t innerImplode(int notExt, int op(uint32_t ch), const char* funcName);

struct func_result_t Explode(int entryStatus)
{
    return innerExplode("Explode");
}

struct func_result_t Explode_Ext(int entryStatus)
{
    return innerExplode("Explode_Ext");
}

struct func_result_t Implode(int entryStatus)
{
    return innerImplode(1, implodeAllowableChar, "Implode");
}

struct func_result_t Implode_Ext(int entryStatus)
{
    return innerImplode(0, implodeExtAllowableChar, "Implode_Ext");
}

static struct func_result_t innerImplode(int notExt, int checker(uint32_t ch),  const char* funcName)
{
    gcInitBuiltinEnv();

    uint64_t i = 0;
    for (i = 0; i < BUILTIN_FRAG->length; ++i)
    {
        struct vterm_t* term = _memMngr.vterms + BUILTIN_FRAG->offset + i;

        if (term->tag != V_CHAR_TAG || !checker(term->ch))
            break;
    }

    uint64_t offset;
    if (i == 0)
    {
        if (notExt)
        {
            checkAndCleanHeaps(1, BUILTINS_RESULT_SIZE + VINT_STRUCT_SIZE(1));
            offset = allocateUInt8VTerm(0);
        }
        else
        {
            checkAndCleanHeaps(1, BUILTINS_RESULT_SIZE + VSTRING_SIZE(0));
            struct vstring_t* ident = allocateVString(0);
            offset = allocateIdentVTerm(ident);
        }
    }
    else
    {
        uint64_t addSize = 0;

        if (notExt && BUILTIN_FRAG->length - i > 0)
            addSize = CHAIN_LTERM_SIZE;

        checkAndCleanHeaps(1, BUILTINS_RESULT_SIZE + VSTRING_SIZE(i) + addSize);

        struct vstring_t* ident = allocateVString(i);

        uint64_t j = 0;
        for (j = 0; j < i; ++j)
            ident->head[j] = _memMngr.vterms[BUILTIN_FRAG->offset + j].ch;

        offset = allocateIdentVTerm(ident);
    }

    BUILTIN_FRAG->offset += i;
    BUILTIN_FRAG->length -= i;

    struct lterm_t* res = allocateBuiltinsResult(offset, 1);

    if (BUILTIN_FRAG->length > 0 && notExt)
    {
        struct lterm_t* restExpr = allocateSimpleChain();

        restExpr->tag = L_TERM_FRAGMENT_TAG;
        restExpr->fragment = BUILTIN_FRAG;

        ADD_TO_CHAIN(res, restExpr);
    }

    return (struct func_result_t){.status = OK_RESULT, .fieldChain = res, .callChain = 0};
}

static int implodeAllowableChar(uint32_t ch)
{
    if (ch == '-' || ch == '_')
        return 1;

    // Обработка цифр.
    if (ch >= '0' && ch <= '9')
        return 1;

    // Обработка латинских символов
    if (ch >= 'a' && ch <= 'z' ||
        ch >= 'A' && ch <= 'Z')
        return 1;

    // Обработка латинских символов
    if (ch >= 'a' && ch <= 'z' ||
        ch >= 'A' && ch <= 'Z')
        return 1;

    // Обработка символов, принадлежащих русскому алфавиту.
    if (ch >= 0x410 && ch <= 0x44F)
        return 1;

    // Обработка символов Ё и ё.
    if (ch == 0x401 || ch == 0x451)
        return 1;

    return 0;
}

static int implodeExtAllowableChar(uint32_t ch)
{
    return 1;
}

static struct func_result_t innerExplode(const char* funcName)
{
    gcInitBuiltinEnv();

    if (BUILTIN_FRAG->length == 0)
        FMT_PRINT_AND_EXIT(BAD_ARG, funcName);

    if (_memMngr.vterms[BUILTIN_FRAG->offset].tag != V_IDENT_TAG)
        FMT_PRINT_AND_EXIT(BAD_ARG, funcName);

    checkAndCleanHeaps(GET_VSTRING_LENGTH(_memMngr.vterms[BUILTIN_FRAG->offset].str),
            BUILTINS_RESULT_SIZE);

    struct vstring_t* ident = _memMngr.vterms[BUILTIN_FRAG->offset].str;
    uint64_t resOffset = _memMngr.vtermsOffset;
    uint64_t i = 0;

    for (i = 0; i < GET_VSTRING_LENGTH(ident); ++i)
        allocateSymbolVTerm(ident->head[i]);

    struct lterm_t* res = allocateBuiltinsResult(resOffset, GET_VSTRING_LENGTH(ident));

    return (struct func_result_t){.status = OK_RESULT, .fieldChain = res, .callChain = 0};
}
