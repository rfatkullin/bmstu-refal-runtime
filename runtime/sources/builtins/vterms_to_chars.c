#include <vint.h>
#include <helpers.h>
#include <vmachine.h>
#include <memory_manager.h>
#include <builtins/builtins.h>
#include <defines/errors_str.h>
#include <builtins/unicode_io.h>

static uint64_t calcBytesCount(uint64_t offset, uint64_t length);
static char* assemblyFragment(uint64_t offset, uint64_t length, char* buff);
static uint64_t calcBytesForVStringCharArr(struct vstring_t* str);
static char* vIntToCharArr(struct vint_t* intNum, char* buff);
static char* vStringToCharArr(struct vstring_t* str, char* buff);

char* vtermsToChars(uint64_t offset, uint64_t length)
{
    uint64_t needBytesCount = calcBytesCount(offset, length);

    char* buff = (char*)malloc(needBytesCount + 1);

    char* str = assemblyFragment(offset, length, buff);
    *str++ = 0;

    return buff;
}

uint64_t calcBytesForIntCharArr(struct vint_t* intNum, mpz_t* outputNum)
{
    mpz_t num;
    mpz_init(num);
    mpz_import(num, GET_INT_LENGTH(intNum), 1, sizeof(uint8_t), 1, 0, intNum->bytes);

    if (GET_INT_SIGN(intNum))
        mpz_neg(num, num);

    char ch;
    uint64_t size = gmp_snprintf(&ch, 1, "%Zd", num);

    if (outputNum)
        mpz_swap(*outputNum, num);
    else
        mpz_clear(num);

    return size;
}

static char* assemblyFragment(uint64_t offset, uint64_t length, char* buff)
{
    uint64_t i = 0;
    for (i = 0; i < length; ++i)
    {
        struct vterm_t* term = _memMngr.vterms + offset + i;

        switch (term->tag)
        {
            case V_BRACKETS_TAG:
                *buff++ = '(';
                buff = assemblyFragment(term->brackets->offset, term->brackets->length, buff);
                *buff++ = ')';
                break;
            case V_CHAR_TAG:
                buff = writeAsUTF8ToBuff(term->ch, buff);
                break;

            case V_IDENT_TAG:
                buff = vStringToCharArr(term->str, buff);
                break;

            case V_CLOSURE_TAG:
                buff = vStringToCharArr(term->closure->ident, buff);
                break;

            case V_INT_NUM_TAG:
                buff = vIntToCharArr(term->intNum, buff);
                break;

            case V_DOUBLE_NUM_TAG:
                buff += sprintf(buff, "%f", term->doubleNum);
                break;

            default:
                FMT_PRINT_AND_EXIT(BAD_VTERM, "Open");
        }
    }

    return buff;
}

static uint64_t calcBytesCount(uint64_t offset, uint64_t length)
{
    uint64_t needBytesCount = 0;
    uint64_t i = 0;

    for (i = 0; i < length; ++i)
    {
        struct vterm_t* term = _memMngr.vterms + offset + i;

        switch (term->tag)
        {
            case V_BRACKETS_TAG:
                needBytesCount += calcBytesCount(term->brackets->offset, term->brackets->length);
                needBytesCount += 2;
                break;

            case V_CHAR_TAG:
                needBytesCount += sizeof(uint32_t);
                break;

            case V_IDENT_TAG:
                needBytesCount += calcBytesForVStringCharArr(term->str);
                break;

            case V_CLOSURE_TAG:
                needBytesCount += calcBytesForVStringCharArr(term->closure->ident);
                break;

            case V_INT_NUM_TAG:
                needBytesCount += calcBytesForIntCharArr(term->intNum, 0);
                break;

            case V_DOUBLE_NUM_TAG:
                needBytesCount += snprintf(0, 0, "%f", term->doubleNum);
                break;
        }
    }

    return needBytesCount;
}

static uint64_t calcBytesForVStringCharArr(struct vstring_t* str)
{
    if (!str)
        return 0;

    return  sizeof(uint32_t) * str->length;
}

static char* vIntToCharArr(struct vint_t* intNum, char* buff)
{
    mpz_t num;
    uint64_t size = calcBytesForIntCharArr(intNum, &num);

    if (!buff)
        PRINT_AND_EXIT(BAD_BUFF);

    gmp_sprintf (buff, "%Zd", num);

    mpz_clear(num);

    return buff + size;
}

static char* vStringToCharArr(struct vstring_t* str, char* buff)
{
    uint64_t i = 0;
    for (i = 0; i < str->length; ++i)
        buff = writeAsUTF8ToBuff(str->head[i], buff);

    return buff;
}
