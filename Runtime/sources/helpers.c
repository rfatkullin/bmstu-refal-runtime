#include <stdio.h>
#include <stdlib.h>

#include "helpers.h"

void failWithMemoryOverflow()
{
    printf("Failed with memory overflow!");
    exit(1);
}

void checkAndCleanVTerms(uint64_t needVTermsCount)
{
    if (memMngr.vtermsOffset + needVTermsCount > memMngr.vtermsMaxOffset)
    {
        //TO FIX: Передать корень FOV.
        collectGarbage(0);
    }

    if (memMngr.vtermsOffset + needVTermsCount > memMngr.vtermsMaxOffset)
        failWithMemoryOverflow();
}

void checkAndCleanData(uint64_t needDataSize)
{
    if (memMngr.dataOffset + needDataSize > memMngr.dataMaxOffset)
    {
        //TO FIX: Передать корень FOV.
        collectGarbage(0);
    }

    if (memMngr.dataOffset + needDataSize > memMngr.dataMaxOffset)
        failWithMemoryOverflow();
}
