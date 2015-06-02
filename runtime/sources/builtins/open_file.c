#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <inttypes.h>

#include <vint.h>
#include <helpers.h>
#include <vmachine.h>
#include <memory_manager.h>
#include <builtins/builtins.h>
#include <defines/errors_str.h>
#include <builtins/unicode_io.h>

static uint8_t getOpenMode(struct fragment_t* frag);
static getFileDescr(struct vint_t* bigInt, uint8_t* descr);
static void openFileWithName(char* fileName, uint8_t mode, uint8_t descr);
static void gcOpenFile(struct fragment_t* frag, uint8_t mode, uint8_t descr);

struct func_result_t Open(int entryStatus)
{
    gcInitBuiltinEnv();

    if (BUILTIN_FRAG->length < 2)
        PRINT_AND_EXIT(TOO_FEW_ARGUMENTS);

    if (_memMngr.vterms[BUILTIN_FRAG->offset].tag != V_CHAR_TAG)
        FMT_PRINT_AND_EXIT(BAD_FILE_OPEN_MODE, "Open");

    uint8_t mode = getOpenMode(BUILTIN_FRAG);
    uint8_t descr = getDescr(BUILTIN_FRAG);

    gcOpenFile(BUILTIN_FRAG, mode, descr);

    return (struct func_result_t){.status = OK_RESULT, .fieldChain = 0, .callChain = 0};
}

void gcOpenDefaultFile(uint8_t descr, uint8_t mode)
{
    checkAndCleanHeaps(0, PATTERN_FILE_NAME_LENGHT);
    char* fileName = (char*)(_memMngr.data + _memMngr.dataOffset);

    snprintf(fileName, PATTERN_FILE_NAME_LENGHT, FILE_NAME_PATTERN, descr);

    openFileWithName(fileName, mode, descr);
}

uint8_t getDescr(struct fragment_t* frag)
{
    if (_memMngr.vterms[frag->offset].tag != V_INT_NUM_TAG)
        FMT_PRINT_AND_EXIT(BAD_DESCR, "Open/Get/Put/Putout", MAX_FILE_DESCR);

    uint8_t descr = 0;
    if (!getFileDescr(_memMngr.vterms[frag->offset].intNum, &descr))
        FMT_PRINT_AND_EXIT(BAD_DESCR, "Open/Get/Put/Putout", MAX_FILE_DESCR, descr);

    frag->offset++;
    frag->length--;

    return descr;
}

static void gcOpenFile(struct fragment_t* frag, uint8_t mode, uint8_t descr)
{
    char* fileName = vtermsToChars(frag->offset, frag->length, 0);

    if (!fileName)
    {
        gcOpenDefaultFile(descr, mode);
        return;
    }

    openFileWithName(fileName, mode, descr);

    free(fileName);
}

static void openFileWithName(char* fileName, uint8_t mode, uint8_t descr)
{
    if (_files[descr].file)
        fclose(_files[descr].file);

    if (descr == 0)
        FMT_PRINT_AND_EXIT(TRY_TO_TAKE_TERMINAL_DESCR, "Open", MAX_FILE_DESCR);

    _files[descr].file = fopen(fileName, modeStr[mode]);

    if (!_files[descr].file)
        FMT_PRINT_AND_EXIT(FILE_OPEN_ERROR, "Open", fileName, modeStr[mode], strerror(errno));

    _files[descr].mode = mode;
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
            FMT_PRINT_AND_EXIT(BAD_FILE_OPEN_MODE, "Open");
    }

    frag->offset++;
    frag->length--;

    return mode;
}

static getFileDescr(struct vint_t* bigInt, uint8_t* descr)
{
    if (GET_INT_SIGN(bigInt) || GET_INT_LENGTH(bigInt) > 1)
        return 0;

    *descr = *bigInt->bytes;

    return *descr < MAX_FILE_DESCR;
}
