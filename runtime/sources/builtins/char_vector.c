#include <stdlib.h>
#include <string.h>

#include <builtins/char_vector.h>

void constructCharVector(struct char_vector_t* charVec)
{
    charVec->buff = (char*) malloc(1);
    charVec->size = 0;
    charVec->capacity = 1;
}

void delCharVec(struct char_vector_t* charVec)
{
    if (charVec->buff)
        free(charVec->buff);

    memset(charVec, 0, sizeof(struct char_vector_t));
}

void clearCharVector(struct char_vector_t* charVec)
{
    charVec->size = 0;
}

void addToCharVector(struct char_vector_t* charVec, char ch)
{
    if (charVec->size + 1 > charVec->capacity)
    {
        char* tmp = (char*) malloc(2 * charVec->capacity);
        memcpy(tmp, charVec->buff, charVec->capacity);
        charVec->capacity *= 2;
        free(charVec->buff);
        charVec->buff = tmp;
    }

    charVec->buff[charVec->size] = ch;
    charVec->size++;
}
