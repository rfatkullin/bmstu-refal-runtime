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
        memMngr.vtermsOverflow = 1;
        //TO FIX: Передать корень FOV.
        collectGarbage(0);
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
        memMngr.dataOverflow = 1;
        //TO FIX: Передать корень FOV.
        collectGarbage(0);
        collected = 1;
    }

    if (memMngr.dataOffset + needDataSize > memMngr.dataMaxOffset)
        failWithMemoryOverflow();

    return collected;
}


int checkVTermsOverflow(uint64_t needVTermsCount)
{
    if (memMngr.vtermsOffset + needVTermsCount > memMngr.vtermsMaxOffset)
    {
        memMngr.vtermsOverflow = 1;
        return 1;
    }

    return 0;
}

int checkDataOverflow(uint64_t needDataSize)
{
    if (memMngr.dataOffset + needDataSize > memMngr.dataMaxOffset)
    {
        memMngr.dataOverflow = 1;
        return 1;
    }

    return 0;
}
