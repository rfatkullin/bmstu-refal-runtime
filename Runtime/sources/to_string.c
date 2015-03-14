#include <gmp.h>

#include <helpers.h>
#include <to_string.h>
#include <builtins/unicode_io.h>

#define BAD_BUFF  "Bad buffer passed to char array converter!"

uint64_t calcBytesForIntCharArr(struct v_int* intNumStruct)
{
    mpz_t num;
    mpz_init(num);

    mpz_import(num, intNumStruct->length, 1, sizeof(uint8_t), 1, 0, intNumStruct->bytes);

    uint64_t size = mpz_sizeinbase(num, 10);

    if (intNumStruct->sign)
        ++size;

    mpz_clear(num);

    return size;
}

uint64_t calcBytesForVStringCharArr(struct v_string* str)
{
    if (!str)
        return 0;

    return  sizeof(uint32_t) * str->length;
}

char* vIntToCharArr(struct v_int* intNumStruct, char* buff)
{
    mpz_t num;
    mpz_init(num);

    mpz_import(num, intNumStruct->length, 1, sizeof(uint8_t), 1, 0, intNumStruct->bytes);
    uint64_t size = mpz_sizeinbase(num, 10);

    if (intNumStruct->sign)
    {
        mpz_mul_si(num, num, -1);
        ++size;
    }

    if (!buff)
        PRINT_AND_EXIT(BAD_BUFF);

    gmp_sprintf (buff, "%Zd", num);

    mpz_clear(num);

    return buff + size;
}

char* vStringToCharArr(struct v_string* str, char* buff)
{
    uint64_t i = 0;
    for (i = 0; i < str->length; ++i)
        buff = writeAsUTF8ToBuff(str->head[i], buff);

    return buff;
}

