#include <stdio.h>
#include <time.h>
#include <string.h>

#include "memory_manager.h"
#include "data_allocators.h"

void collectGarbage(struct lterm_t* fieldOfView)
{
    clock_t start, end;
    printf("Start garbage collection.\n");
    start = clock();

    collectVTermGarbage(fieldOfView);

    copyChainLTerm(fieldOfView);

    end = clock();

    printf("End garbage collection. Time elapsed: %f\n", ((float)(end - start)) / CLOCKS_PER_SEC);
}







