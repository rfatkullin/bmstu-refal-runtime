#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <helpers.h>
#include <vmachine.h>
#include <debug_print.h>
#include <defines/gc_macros.h>
#include <builtins/builtins.h>
#include <defines/errors_str.h>
#include <builtins/unicode_io.h>
#include <allocators/data_alloc.h>
#include <allocators/vterm_alloc.h>
#include <builtins/case_mapping.h>
#include <defines/data_struct_sizes.h>

static void _gcPut();
static char* getFileName();
static struct func_result_t _gcGet(FILE* file);
static uint64_t createCharVTerms(uint8_t* chars, uint64_t length);
static void getCharsFromVTerms(uint8_t* chars, uint64_t offset, uint64_t length);

struct func_result_t Card(int entryStatus)
{
    return _gcGet(stdin);
}

struct func_result_t Get(int entryStatus)
{
    gcInitBuiltin();

    if (BUILTIN_FRAG->length != 1)
        PRINT_AND_EXIT(GET_WRONG_ARG_NUM);    

    uint8_t descr = getDescr(BUILTIN_FRAG);

    if (!descr)
        return _gcGet(stdin);

    if (!_files[descr].file)
        gcOpenDefaultFile(descr, READ_MODE);

    return _gcGet(_files[descr].file);
}

struct func_result_t Prout(int entryStatus)
{
    gcInitBuiltin();

    printFragmentLn(stdout, BUILTIN_FRAG);

	return (struct func_result_t){.status = OK_RESULT, .fieldChain = 0, .callChain = 0};
}

struct func_result_t Print(int entryStatus)
{
    gcInitBuiltin();

    printFragmentLn(stdout, BUILTIN_FRAG);

    return (struct func_result_t){.status = OK_RESULT, .fieldChain = CURR_FUNC_CALL->fieldOfView, .callChain = 0};
}

struct func_result_t Put(int entryStatus)
{
    _gcPut();

    // Remove descr vterm
    BUILTIN_FRAG->offset++;
    BUILTIN_FRAG->length--;

    return (struct func_result_t){.status = OK_RESULT, .fieldChain = CURR_FUNC_CALL->fieldOfView, .callChain = 0};
}

struct func_result_t Putout(int entryStatus)
{
    _gcPut();

    return (struct func_result_t){.status = OK_RESULT, .fieldChain = 0, .callChain = 0};
}

static struct func_result_t _gcGet(FILE* file)
{
    uint64_t firstOffset = _memMngr.vtermsOffset;
    uint64_t currOffset =  _memMngr.vtermsOffset;
    struct lterm_t* mainChain = 0;

    checkAndCleanHeaps(0, BUILTINS_RESULT_SIZE);

    uint32_t ch;
    while(1)
    {
        // Simple fix \r\n line endings
        while ((ch = readUTF8Char(file)) == '\r');

        if (ch == '\n' || ch == 0)
            break;

        if (GC_VTERM_OV(1))
        {
            uint64_t length = currOffset - firstOffset;
            uint8_t* buff = malloc(length);

            getCharsFromVTerms(buff, firstOffset, length);

            collectGarbage();
            GC_VTERM_HEAP_CHECK_EXIT(length + 1);

            // Copy earlier read chars.
            firstOffset = createCharVTerms(buff, length);
            currOffset = _memMngr.vtermsOffset;

            free(buff);
        }

        // Checked --> may call func without gc prefix.
        allocateSymbolVTerm(ch);
        currOffset++;
    }

    if (ch == '\n')
    {
        // Checked --> may call func without gc prefix.
        mainChain = allocateBuiltinsResult(firstOffset, _memMngr.vtermsOffset - firstOffset);
    }
    else // ch == 0
    {        
        checkAndCleanHeaps(1, VINT_STRUCT_SIZE(1) + BUILTINS_RESULT_SIZE);
        allocateUInt8VTerm(0);
        mainChain = allocateBuiltinsResult(firstOffset, _memMngr.vtermsOffset - firstOffset);
    }    

    return (struct func_result_t){.status = OK_RESULT, .fieldChain = mainChain, .callChain = 0};
}

static void _gcPut()
{
    gcInitBuiltin();

    if (BUILTIN_FRAG->length < 1)
        PRINT_AND_EXIT(TOO_FEW_ARGUMENTS);

    uint8_t descr = getDescr(BUILTIN_FRAG);

    if (!descr)
    {
        printFragmentLn(stdout, BUILTIN_FRAG);
        return;
    }

    if (!_files[descr].file)
        gcOpenDefaultFile(descr, WRITE_MODE);

    printFragmentLn(_files[descr].file, BUILTIN_FRAG);
}

static void getCharsFromVTerms(uint8_t* chars, uint64_t offset, uint64_t length)
{
    uint64_t i = 0;
    for (i = 0; i < length; ++i)
    {
        chars[i] = _memMngr.vterms[offset + i].ch;
    }
}

static uint64_t createCharVTerms(uint8_t* chars, uint64_t length)
{
    uint64_t firstOffset = _memMngr.vtermsOffset;

    uint64_t i = 0;
    for (i = 0; i < length; ++i)
    {        
        allocateSymbolVTerm(chars[i]);
    }

    return firstOffset;
}

/// Проверка на равенство двух строк. 1 - успех, 0 - неудача.
int ustrEq(struct vstring_t* a, struct vstring_t* b)
{
    if (!a || !b)
        return 0;

    if (a->length != b->length)
        return 0;

    uint64_t i = 0;
    for (i = 0; i < a->length; ++i)
    {
        if (a->head[i] != b->head[i])
            return 0;
    }

    return 1;
}
