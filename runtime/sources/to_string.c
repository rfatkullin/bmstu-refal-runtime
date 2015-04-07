#include <gmp.h>

#include <helpers.h>
#include <to_string.h>
#include <builtins/unicode_io.h>

#define BAD_BUFF  "Bad buffer passed to char array converter!"

uint64_t calcBytesForIntCharArr(struct vint_t* intNum, mpz_t* outputNum)
{
    mpz_t num;
    mpz_init(num);
    mpz_import(num, intNum->length, 1, sizeof(uint8_t), 1, 0, intNum->bytes);

    if (intNum->sign)
        mpz_neg(num, num);

    char ch;
    uint64_t size = gmp_snprintf(&ch, 1, "%Zd", num);

    if (outputNum)
        mpz_swap(*outputNum, num);
    else
        mpz_clear(num);

    return size;
}

uint64_t calcBytesForVStringCharArr(struct vstring_t* str)
{
    if (!str)
        return 0;

    return  sizeof(uint32_t) * str->length;
}

char* vIntToCharArr(struct vint_t* intNum, char* buff)
{
    mpz_t num;
    uint64_t size = calcBytesForIntCharArr(intNum, &num);

    if (!buff)
        PRINT_AND_EXIT(BAD_BUFF);

    gmp_sprintf (buff, "%Zd", num);

    mpz_clear(num);

    return buff + size;
}

char* vStringToCharArr(struct vstring_t* str, char* buff)
{
    uint64_t i = 0;
    for (i = 0; i < str->length; ++i)
        buff = writeAsUTF8ToBuff(str->head[i], buff);

    return buff;
}

