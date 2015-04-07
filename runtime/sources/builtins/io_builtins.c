#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <helpers.h>
#include <vmachine.h>
#include <to_string.h>
#include <builtins/builtins.h>
#include <builtins/unicode_io.h>
#include <allocators/data_alloc.h>
#include <allocators/vterm_alloc.h>
#include <builtins/case_map_table.h>

static void printSymbol(FILE* file, struct vterm_t* term);
static void printUnicodeChar(uint32_t ch);
static uint64_t copySymbols(uint64_t first, uint64_t length);
static char* getFileName();
static void gcAssemblyFileName(struct fragment_t* frag);
static void gcOpenFile(struct fragment_t* frag, uint8_t mode, uint8_t descr);
static uint8_t getOpenMode(struct fragment_t* frag);
static uint8_t getDescr(struct fragment_t* frag);
static void gcOpenDefaultFile(uint8_t descr, uint8_t mode);
static void openFileWithName(char* fileName, uint8_t mode, uint8_t descr);
static void _gcPut();
static struct func_result_t _gcGet(FILE* file);
static getFileDescr(struct vint_t* bigInt, uint8_t* descr);
static void printChain(FILE* file, struct lterm_t* chain);
static void printFragmentTogether(FILE* file, struct fragment_t* frag);

static uint64_t getBytesCountToAssembly(struct fragment_t* frag);
static char* assemblyFragment(struct fragment_t* frag, char* buff);

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

    if (!files[descr].file)
        gcOpenDefaultFile(descr, READ_MODE);

    return _gcGet(files[descr].file);
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

struct func_result_t Open(int entryStatus)
{
    gcInitBuiltin();

    if (BUILTIN_FRAG->length < 2)
        PRINT_AND_EXIT(TOO_FEW_ARGUMENTS);

    if (_memMngr.vterms[BUILTIN_FRAG->offset].tag != V_CHAR_TAG)
        PRINT_AND_EXIT(BAD_FILE_OPEN_MODE);

    uint8_t mode = getOpenMode(BUILTIN_FRAG);
    uint8_t descr = getDescr(BUILTIN_FRAG);

    gcOpenFile(BUILTIN_FRAG, mode, descr);

    return (struct func_result_t){.status = OK_RESULT, .fieldChain = 0, .callChain = 0};
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

    if (CURR_FUNC_CALL->env->workFieldOfView)
        PRINT_AND_EXIT("BEDA!!!\n");

    uint32_t ch;
    while(1)
    {
        // Simple fix of windows \r\n
        while ((ch = readUTF8Char(file)) == '\r');

        if (ch == '\n' || ch == 0)
            break;

        if (checkAndCleanHeaps(1, 0))
        {
            uint64_t length = currOffset - firstOffset + 1;
            strictCheckHeaps(length + 1, 0);

            // Copy earlier read chars.
            firstOffset = copySymbols(firstOffset, currOffset - firstOffset);
            currOffset = _memMngr.vtermsOffset;
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

    if (!files[descr].file)
        gcOpenDefaultFile(descr, WRITE_MODE);

    printFragmentLn(files[descr].file, BUILTIN_FRAG);
}

static uint8_t getOpenMode(struct fragment_t* frag)
{
    uint8_t mode;

    switch (_memMngr.vterms[frag->offset].ch)
    {
        case 'w':
        case 'W':            
            mode = WRITE_MODE;
            break;

        case 'r':
        case 'R':            
            mode = READ_MODE;
           break;

        default:
            PRINT_AND_EXIT(BAD_FILE_OPEN_MODE);
    }

    frag->offset++;
    frag->length--;

    return mode;
}

static uint8_t getDescr(struct fragment_t* frag)
{
    if (_memMngr.vterms[frag->offset].tag != V_INT_NUM_TAG)
        FMT_PRINT_AND_EXIT(BAD_DESCR, MAX_FILE_DESCR);

    uint8_t descr = 0;
    if (!getFileDescr(_memMngr.vterms[frag->offset].intNum, &descr))
        FMT_PRINT_AND_EXIT(BAD_DESCR, MAX_FILE_DESCR, descr);

    frag->offset++;
    frag->length--;

    return descr;
}

static getFileDescr(struct vint_t* bigInt, uint8_t* descr)
{
    if (bigInt->sign || bigInt->length > 1)
        return 0;

    *descr = *bigInt->bytes;

    return *descr < MAX_FILE_DESCR;
}

static void gcOpenFile(struct fragment_t* frag, uint8_t mode, uint8_t descr)
{
    gcAssemblyFileName(frag);
    char* fileName = (char*)(_memMngr.data + _memMngr.dataOffset);

    if (!fileName)
    {
        gcOpenDefaultFile(descr, mode);
        return;
    }

    openFileWithName(fileName, mode, descr);    
}

static void gcOpenDefaultFile(uint8_t descr, uint8_t mode)
{    
    checkAndCleanHeaps(0, PATTERN_FILE_NAME_LENGHT);
    char* fileName = (char*)(_memMngr.data + _memMngr.dataOffset);

    snprintf(fileName, PATTERN_FILE_NAME_LENGHT, FILE_NAME_PATTERN, descr);

    openFileWithName(fileName, mode, descr);    
}

static void openFileWithName(char* fileName, uint8_t mode, uint8_t descr)
{
    if (files[descr].file)
        FMT_PRINT_AND_EXIT(DESCR_ALREADY_IN_USE, descr);

    if (descr == 0)
        FMT_PRINT_AND_EXIT(TRY_TO_TAKE_TERMINAL_DESCR, MAX_FILE_DESCR);

    files[descr].file = fopen(fileName, modeStr[mode]);

    if (!files[descr].file)
        FMT_PRINT_AND_EXIT(FILE_OPEN_ERROR, fileName, modeStr[mode], strerror(errno));

    files[descr].mode = mode;
}

static void gcAssemblyFileName(struct fragment_t* frag)
{
    uint64_t needBytesCount = getBytesCountToAssembly(frag);
    checkAndCleanHeaps(0, needBytesCount + 1); // +1 for 0 character.
    char* buff = (char*)(_memMngr.data + _memMngr.dataOffset);

    buff = assemblyFragment(frag, buff);

    *buff++ = 0;
}

static char* assemblyFragment(struct fragment_t* frag, char* buff)
{
    uint64_t i = 0;
    for (i = 0; i < frag->length; ++i)
    {
        struct vterm_t* term = _memMngr.vterms + frag->offset + i;

        switch (term->tag)
        {
            case V_BRACKETS_TAG:
                *buff++ = '(';
                buff = assemblyFragment(term->brackets, buff);
                *buff++ = ')';
                break;
            case V_CHAR_TAG:
                buff = writeAsUTF8ToBuff(term->ch, buff);
                break;

            case V_IDENT_TAG:
                buff = vStringToCharArr(term->str, buff);
                break;

            case V_CLOSURE_TAG:
                buff = vStringToCharArr(term->closure->ident, buff);
                break;

            case V_INT_NUM_TAG:
                buff = vIntToCharArr(term->intNum, buff);
                break;

            case V_DOUBLE_NUM_TAG:
                buff += sprintf(buff, "%f", term->doubleNum);
                break;

            default:
                PRINT_AND_EXIT(BAD_VTERM);
        }
    }

    return buff;
}

static uint64_t getBytesCountToAssembly(struct fragment_t* frag)
{
    uint64_t needBytesCount = 0;
    uint64_t i = 0;

    for (i = 0; i < frag->length; ++i)
    {
        struct vterm_t* term = _memMngr.vterms + frag->offset + i;

        switch (term->tag)
        {
            case V_BRACKETS_TAG:
                needBytesCount += getBytesCountToAssembly(term->brackets);
                needBytesCount += 2;
                break;

            case V_CHAR_TAG:
                needBytesCount += sizeof(uint32_t);
                break;

            case V_IDENT_TAG:
                needBytesCount += calcBytesForVStringCharArr(term->str);
                break;

            case V_CLOSURE_TAG:
                needBytesCount += calcBytesForVStringCharArr(term->closure->ident);
                break;

            case V_INT_NUM_TAG:
                needBytesCount += calcBytesForIntCharArr(term->intNum, 0);
                break;

            case V_DOUBLE_NUM_TAG:
                needBytesCount += snprintf(0, 0, "%f", term->doubleNum);
                break;
        }
    }

    return needBytesCount;
}

static uint64_t copySymbols(uint64_t first, uint64_t length)
{
    uint64_t firstOffset = _memMngr.vtermsOffset;

    uint64_t i = 0;
    for (i = 0; i < length; ++i)
        allocateSymbolVTerm(_memMngr.vterms[first + i].ch);

    return firstOffset;
}

void printFieldOfView(FILE* file, struct lterm_t* fov)
{
    fprintf(file, "FOV START-----------------\n");
    printChain(file, fov);
    fprintf(file, "\n");
}

static void printChain(FILE* file, struct lterm_t* chain)
{
    if (chain->tag != L_TERM_CHAIN_TAG)
        PRINT_AND_EXIT("[Print chain]: Must be chain");

    struct lterm_t* currTerm = chain->next;

    while (currTerm != chain)
    {
        switch (currTerm->tag)
        {
            case L_TERM_FRAGMENT_TAG :
            {
                printFragmentTogether(file, currTerm->fragment);
                break;
            }
            case L_TERM_CHAIN_KEEPER_TAG:
            {
                fprintf(file, "(");
                printChain(file, currTerm->chain);
                fprintf(file, ")");
                break;
            }

            case L_TERM_FUNC_CALL:
            {
                fprintf(file, "<");

                if (currTerm->funcCall->funcPtr)
                    fprintf(file, "[FuncCalled]");

                if (currTerm->funcCall->fieldOfView)
                    printChain(file, currTerm->funcCall->fieldOfView);                

                if (currTerm->funcCall->subCall)
                {
                    fprintf(file, "<");
                    fprintf(file, "[Func subCall]:");
                    printChain(file, currTerm->funcCall->subCall);
                    fprintf(file, ">");
                }

                fprintf(file, ">");
                break;
            }

            default:
                PRINT_AND_EXIT(BAD_TAG_AT_ASSEMBLY);

        }

        currTerm = currTerm->next;
    }
}

static void printFragmentTogether(FILE* file, struct fragment_t* frag)
{
    int i = 0;
    struct vterm_t* currTerm = _memMngr.vterms + frag->offset;

    for (i = 0; i < frag->length; ++i)
        printSymbol(file, currTerm + i);
}

void printFragmentLn(FILE* file, struct fragment_t* frag)
{
    printFragment(file, frag);

    fprintf(file, "\n");
}

void printFragment(FILE* file, struct fragment_t* frag)
{
    int i = 0;
    struct vterm_t* currTerm = _memMngr.vterms + frag->offset;

    for (i = 0; i < frag->length; ++i)
        printSymbol(file, currTerm + i);
}

static void printSymbol(FILE* file, struct vterm_t* term)
{
	switch (term->tag)
	{
	case V_CHAR_TAG:
        printUTF32(file, term->ch);
		break;
	case V_IDENT_TAG:
        printUStr(file, term->str);
        fprintf(file, " ");
		break;
	case V_INT_NUM_TAG:
        printIntNumber(file, term->intNum);
		break;
    case V_DOUBLE_NUM_TAG:
        fprintf(file, "%lf ", term->doubleNum);
		break;
    case V_CLOSURE_TAG:
        printUStr(file, term->closure->ident);
		break;
    case V_BRACKETS_TAG:
        fprintf(file, "(");
        printFragment(file, term->brackets);
        fprintf(file, ")");
		break;    
	}
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

void printUStr(FILE* file, struct vstring_t* str)
{
    if (!str)
        return;

    uint64_t i = 0;

    for (i = 0; i < str->length; ++i)
        printUTF32(file, str->head[i]);
}

void printIntNumber(FILE* file, struct vint_t* intNum)
{
    mpz_t num;
    mpz_init(num);

    mpz_import(num, intNum->length, 1, sizeof(uint8_t), 1, 0, intNum->bytes);

    if (intNum->sign)
        mpz_neg(num, num);

    gmp_fprintf(file, "%Zd ", num);

    mpz_clear(num);
}
