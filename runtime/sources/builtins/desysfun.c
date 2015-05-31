#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>

#include <helpers.h>
#include <vmachine.h>
#include <debug_print.h>
#include <defines/gc_macros.h>
#include <builtins/builtins.h>
#include <builtins/unicode_io.h>
#include <defines/errors_str.h>
#include <builtins/unicode_io.h>
#include <allocators/data_alloc.h>
#include <allocators/vterm_alloc.h>
#include <builtins/case_mapping.h>
#include <defines/data_struct_sizes.h>

struct func_result_t DeSysfun(int entryStatus)
{
    gcInitBuiltinEnv();

    uint64_t i = 0;
    for (i = 0; i < BUILTIN_FRAG->length; ++i)
    {
        if (_memMngr.vterms[BUILTIN_FRAG->offset + i].tag != V_CHAR_TAG)
            break;
    }

    char* fileName = vtermsToChars(BUILTIN_FRAG->offset, i, 0);
    FILE* outFile = fopen(fileName, "w");
    free(fileName);

    if (!outFile)
        FMT_PRINT_AND_EXIT(FILE_OPEN_ERROR, "DeSysfun", fileName, "w", strerror(errno));

    if (i == BUILTIN_FRAG->length)
        FMT_PRINT_AND_EXIT(BAD_ARG, "DeSysfun");

    if (_memMngr.vterms[BUILTIN_FRAG->offset + i].tag != V_BRACKETS_TAG)
        FMT_PRINT_AND_EXIT(BAD_ARG, "DeSysfun");

    uint64_t bOffset = _memMngr.vterms[BUILTIN_FRAG->offset + i].brackets->offset;
    uint64_t bLength = _memMngr.vterms[BUILTIN_FRAG->offset + i].brackets->length;

    if (_memMngr.vterms[bOffset].tag != V_INT_NUM_TAG)
        FMT_PRINT_AND_EXIT(BAD_ARG, "DeSysfun");

    uint32_t lineSize = getUint32(_memMngr.vterms[bOffset].intNum);
    char* exprStr = vtermsToChars(bOffset + 1, bLength - 1, 1);

    char endLine[1] = {'\n'};
    uint64_t strLen = strlen(exprStr);
    for (i = 0; i < strLen; i += lineSize)
    {
        uint64_t len = min(lineSize, strLen - i);
        fwrite (exprStr + i, sizeof(char), len, outFile);

        if (i + lineSize < strLen)
            fwrite(endLine, sizeof(char), 1, outFile);
    }

    free(exprStr);
    fclose(outFile);

    return (struct func_result_t){.status = OK_RESULT, .fieldChain = 0, .callChain = 0};
}


