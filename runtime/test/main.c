#include <stdio.h>
#include <stdlib.h>

#include <builtins/builtins.h>

int main(int argc, char** argv)
{

#ifdef DEBUG
    printf("Debug.\n");
#else
    printf("Release!\n");
#endif

    return 0;
}
