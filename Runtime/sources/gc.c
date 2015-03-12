#include <stdio.h>
#include <time.h>
#include <string.h>

#include "memory_manager.h"
#include "data_allocators.h"

void collectGarbage()
{
    clock_t start, end;
    printf("Start garbage collection.\n");
    start = clock();

    collectVTermGarbage(memMngr.fieldOfView);

    copyChainLTerm(memMngr.fieldOfView);

    end = clock();

    printf("End garbage collection. Time elapsed: %f\n", ((float)(end - start)) / CLOCKS_PER_SEC);
}







