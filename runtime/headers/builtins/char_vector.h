#ifndef _BMSTU_REFC_CHAR_VECTOR_H_
#define _BMSTU_REFC_CHAR_VECTOR_H_

#include <stdint.h>

struct char_vector_t
{
    char* buff;
    uint32_t size;
    uint32_t capacity;
};

void constructCharVector(struct char_vector_t* charVec);
void delCharVec(struct char_vector_t* charVec);
void clearCharVector(struct char_vector_t* charVec);
void addToCharVector(struct char_vector_t* charVec, char ch);

#endif
