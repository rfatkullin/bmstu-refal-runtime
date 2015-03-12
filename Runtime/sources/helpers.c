#include <stdio.h>
#include <stdlib.h>

#include "helpers.h"

void failWithMemoryOverflow()
{
    printf("Failed with memory overflow!");
    exit(1);
}

void strictCheckVTermsOverflow(uint64_t needVTermsCount)
{
    if (memMngr.vtermsOffset + needVTermsCount > memMngr.vtermsMaxOffset)
        failWithMemoryOverflow();
}

int checkAndCleanVTerms(uint64_t needVTermsCount)
{
    int collected = 0;

    if (memMngr.vtermsOffset + needVTermsCount > memMngr.vtermsMaxOffset)
    {        
        collectGarbage();
        collected = 1;
    }

    if (memMngr.vtermsOffset + needVTermsCount > memMngr.vtermsMaxOffset)
        failWithMemoryOverflow();

    return collected;
}

int checkAndCleanData(uint64_t needDataSize)
{
    int collected = 0;

    if (memMngr.dataOffset + needDataSize > memMngr.dataMaxOffset)
    {        
        collectGarbage();
        collected = 1;
    }

    if (memMngr.dataOffset + needDataSize > memMngr.dataMaxOffset)
        failWithMemoryOverflow();

    return collected;
}


int checkVTermsOverflow(uint64_t needVTermsCount)
{
    return memMngr.vtermsOffset + needVTermsCount > memMngr.vtermsMaxOffset;
}

int checkDataOverflow(uint64_t needDataSize)
{
    return memMngr.dataOffset + needDataSize > memMngr.dataMaxOffset;
}
