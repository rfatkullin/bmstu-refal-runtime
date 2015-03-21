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

static void printSymbol(FILE* file, struct v_term* term);
static void printUnicodeChar(uint32_t ch);
static uint64_t copySymbols(uint64_t first, uint64_t length);
static char* getFileName();
static void gcAssemblyFileName(struct fragment_t* frag);
static void gcOpenFile(struct fragment_t* frag, uint8_t mode, uint8_t descr);
static uint64_t calcNeedBytesCount(struct fragment_t* frag);
static uint8_t getOpenMode(struct fragment_t* frag);
static uint8_t getDescr(struct fragment_t* frag);
static void gcOpenDefaultFile(uint8_t descr, uint8_t mode);
static void openFileWithName(char* fileName, uint8_t mode, uint8_t descr);
static void _gcPut();
static struct func_result_t _gcGet(FILE* file);
static getFileDescr(struct v_int* bigInt, uint8_t* descr);
static void printChain(FILE* file, struct lterm_t* chain);
static void printFragmentTogether(FILE* file, struct fragment_t* frag);

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

    printFragment(stdout, BUILTIN_FRAG);

	return (struct func_result_t){.status = OK_RESULT, .fieldChain = 0, .callChain = 0};
}

struct func_result_t Print(int entryStatus)
{
    gcInitBuiltin();

    printFragment(stdout, BUILTIN_FRAG);

    return (struct func_result_t){.status = OK_RESULT, .fieldChain = _currFuncCall->fieldOfView, .callChain = 0};
}

struct func_result_t Open(int entryStatus)
{
    gcInitBuiltin();

    if (BUILTIN_FRAG->length < 2)
        PRINT_AND_EXIT(TOO_FEW_ARGUMENTS);

    if (memMngr.vterms[BUILTIN_FRAG->offset].tag != V_CHAR_TAG)
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
    _currFuncCall->fieldOfView->next->fragment->offset++;
    _currFuncCall->fieldOfView->next->fragment->length--;

    return (struct func_result_t){.status = OK_RESULT, .fieldChain = _currFuncCall->fieldOfView, .callChain = 0};
}

struct func_result_t Putout(int entryStatus)
{
    _gcPut();

    return (struct func_result_t){.status = OK_RESULT, .fieldChain = 0, .callChain = 0};
}

static struct func_result_t _gcGet(FILE* file)
{
    uint64_t firstOffset = memMngr.vtermsOffset;
    uint64_t currOffset =  memMngr.vtermsOffset;
    struct lterm_t* mainChain = 0;

    checkAndCleanHeaps(0, BUILTINS_RESULT_SIZE);

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
            currOffset = memMngr.vtermsOffset;
        }

        // Checked --> may call func without gc prefix.
        allocateSymbolVTerm(ch);
        currOffset++;
    }

    if (ch == '\n')
    {
        // Checked --> may call func without gc prefix.
        mainChain = allocateBuiltinsResult(firstOffset, memMngr.vtermsOffset - firstOffset);
    }
    else // ch == 0
    {        
        checkAndCleanHeaps(1, VINT_STRUCT_SIZE(1) + BUILTINS_RESULT_SIZE);
        allocateUInt8VTerm(0);
        mainChain = allocateBuiltinsResult(firstOffset, memMngr.vtermsOffset - firstOffset);
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
        printFragment(stdout, BUILTIN_FRAG);
        return;
    }

    if (!files[descr].file)
        gcOpenDefaultFile(descr, WRITE_MODE);

    printFragment(files[descr].file, BUILTIN_FRAG);
}

static uint8_t getOpenMode(struct fragment_t* frag)
{
    uint8_t mode;

    switch (memMngr.vterms[frag->offset].ch)
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
    if (memMngr.vterms[frag->offset].tag != V_INT_NUM_TAG)
        FMT_PRINT_AND_EXIT(BAD_DESCR, MAX_FILE_DESCR);

    uint8_t descr = 0;
    if (!getFileDescr(memMngr.vterms[frag->offset].intNum, &descr))
        FMT_PRINT_AND_EXIT(BAD_DESCR, MAX_FILE_DESCR, descr);

    frag->offset++;
    frag->length--;

    return descr;
}

static getFileDescr(struct v_int* bigInt, uint8_t* descr)
{
    if (bigInt->sign || bigInt->length > 1)
        return 0;

    *descr = *bigInt->bytes;

    return *descr < MAX_FILE_DESCR;
}

static void gcOpenFile(struct fragment_t* frag, uint8_t mode, uint8_t descr)
{
    gcAssemblyFileName(frag);
    char* fileName = (char*)(memMngr.data + memMngr.dataOffset);

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
    char* fileName = (char*)(memMngr.data + memMngr.dataOffset);

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
    uint64_t needBytesCount = calcNeedBytesCount(frag);
    checkAndCleanHeaps(0, needBytesCount + 1); // +1 for 0 character.
    char* pointer = (char*)(memMngr.data + memMngr.dataOffset);

    uint64_t i = 0;
    for (i = 0; i < frag->length; ++i)
    {
        struct v_term* term = memMngr.vterms + frag->offset + i;

        switch (term->tag)
        {
            case V_BRACKET_CLOSE_TAG:
                *pointer++ = '(';
                break;

            case V_BRACKET_OPEN_TAG:
                *pointer++ = ')';
                break;

            case V_CHAR_TAG:
                pointer = writeAsUTF8ToBuff(term->ch, pointer);
                break;

            case V_IDENT_TAG:
                pointer = vStringToCharArr(term->str, pointer);
                break;

            case V_CLOSURE_TAG:
                pointer = vStringToCharArr(term->closure->ident, pointer);
                break;

            case V_INT_NUM_TAG:
                pointer = vIntToCharArr(term->intNum, pointer);
                break;

            case V_DOUBLE_NUM_TAG:
                pointer += sprintf(pointer, "%f", term->doubleNum);
                break;

            default:
                PRINT_AND_EXIT(BAD_VTERM);
        }
    }

    *pointer++ = 0;    
}

static uint64_t calcNeedBytesCount(struct fragment_t* frag)
{
    uint64_t needBytesCount = 0;
    uint64_t i = 0;
    for (i = 0; i < frag->length; ++i)
    {
        struct v_term* term = memMngr.vterms + frag->offset + i;

        switch (term->tag)
        {
            case V_BRACKET_CLOSE_TAG:
            case V_BRACKET_OPEN_TAG:
                ++needBytesCount;
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
    uint64_t firstOffset = memMngr.vtermsOffset;

    uint64_t i = 0;
    for (i = 0; i < length; ++i)
        allocateSymbolVTerm(memMngr.vterms[first + i].ch);

    return firstOffset;
}

void printFieldOfView(FILE* file, struct lterm_t* fov)
{
    fprintf(file, "FOV START-----------------\n");
    printChain(file, fov);
    fprintf(file, "\n\n");
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
                else
                    printChain(file, currTerm->funcCall->env->fovs[0]);

                if (currTerm->funcCall->subCall)
                {
                    fprintf(file, "[Func subCall]");
                    printChain(file, currTerm->funcCall->subCall);
                }

                fprintf(file, ">");
                break;
            }
        }

        currTerm = currTerm->next;
    }
}

static void printFragmentTogether(FILE* file, struct fragment_t* frag)
{
    int i = 0;
    struct v_term* currTerm = memMngr.vterms + frag->offset;

    for (i = 0; i < frag->length; ++i)
        printSymbol(file, currTerm + i);
}

void printFragment(FILE* file, struct fragment_t* frag)
{
	int i = 0;
	struct v_term* currTerm = memMngr.vterms + frag->offset;

	for (i = 0; i < frag->length; ++i)	
        printSymbol(file, currTerm + i);

    fprintf(file, "\n");
}

static void printSymbol(FILE* file, struct v_term* term)
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
    case V_BRACKET_OPEN_TAG:
        fprintf(file, "(");
		break;
    case V_BRACKET_CLOSE_TAG:
        fprintf(file, ")");
        break;
	}
}

/// Проверка на равенство двух строк. 1 - успех, 0 - неудача.
int ustrEq(struct v_string* a, struct v_string* b)
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

void printUStr(FILE* file, struct v_string* str)
{
    if (!str)
        return;

    uint64_t i = 0;

    for (i = 0; i < str->length; ++i)
        printUTF32(file, str->head[i]);
}

void printIntNumber(FILE* file, struct v_int* intNum)
{
    mpz_t num;
    mpz_init(num);

    mpz_import(num, intNum->length, 1, sizeof(uint8_t), 1, 0, intNum->bytes);

    if (intNum->sign)
        mpz_neg(num, num);

    gmp_fprintf(file, "%Zd ", num);

    mpz_clear(num);
}
