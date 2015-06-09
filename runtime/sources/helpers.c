#include <inttypes.h>
#include <sys/resource.h>

#include <gmp.h>

#include <vint.h>
#include <helpers.h>
#include <vmachine.h>
#include <memory_manager.h>
#include <defines/checkers.h>
#include <builtins/unicode_io.h>

uint64_t getHeapSizeFromCmdArgs(int argc, char** argv)
{
    uint64_t heapSize = 0;
    int i = 0;

    for (i = 0; i < argc; ++i)
    {
        if (argv[i][0] == '-')
        {
            if (sscanf(argv[i], "-heapSize=%" PRIu64 , &heapSize) == 1)
            {
#ifdef DEBUG
                printf("Heap size: %" PRIu64 "\n", heapSize);
#endif
                return heapSize;
            }
        }
    }

    return DEFAULT_HEAP_SIZE;
}

int eqFragment(uint64_t a, uint64_t b, uint64_t length)
{
    uint64_t i = 0;
    for (i = 0; i < length; i++)
    {
        if (_memMngr.vterms[a + i].tag != _memMngr.vterms[b + i].tag)
            return 0;

        if (_memMngr.vterms[a + i].tag == V_BRACKETS_TAG)
        {
            if ((VTERM_BRACKETS(a + i)->length != VTERM_BRACKETS(b + i)->length)
             || !eqFragment(VTERM_BRACKETS(a + i)->offset, VTERM_BRACKETS(b + i)->offset, VTERM_BRACKETS(b + i)->length))
            return 0;
        }
        else if (!eqSymbol(a + i, b + i))
            return 0;
    }

    return 1;
}

int eqSymbol(uint64_t a, uint64_t b)
{
    return
        (_memMngr.vterms[a].tag == _memMngr.vterms[b].tag)
        &&
        (
            (_memMngr.vterms[a].tag == V_CHAR_TAG
                && _memMngr.vterms[a].ch == _memMngr.vterms[b].ch)

            || (_memMngr.vterms[a].tag == V_IDENT_TAG
                && ustrEq(_memMngr.vterms[a].str, _memMngr.vterms[b].str))

            || (_memMngr.vterms[a].tag == V_INT_NUM_TAG
                && !intCmp(_memMngr.vterms[a].intNum, _memMngr.vterms[b].intNum))

            || (_memMngr.vterms[a].tag == V_DOUBLE_NUM_TAG
                && !doubleCmp(_memMngr.vterms[a].doubleNum, _memMngr.vterms[b].doubleNum))

            || (_memMngr.vterms[a].tag == V_CLOSURE_TAG
                && ustrEq(_memMngr.vterms[a].closure->ident, _memMngr.vterms[b].closure->ident))
        );
}

uint64_t min(uint64_t a, uint64_t b)
{
    if (a > b)
        return b;

    return a;
}

uint32_t getUint32(struct vint_t* numData)
{
    mpz_t num;

    mpz_init(num);
    mpz_import(num, GET_INT_LENGTH(numData), 1, sizeof(uint8_t), 1, 0, numData->bytes);
    uint32_t res = mpz_get_ui(num);
    mpz_clear(num);

    return res;
}

void resizeStack()
{
//    const rlim_t kStackSize = 512L * 1024L * 1024L;   // min stack size = 64 Mb
//    struct rlimit rl;
//    int result;

//    result = getrlimit(RLIMIT_STACK, &rl);
//    if (result == 0)
//    {
//        if (rl.rlim_cur < kStackSize)
//        {
//            rl.rlim_cur = kStackSize;
//            result = setrlimit(RLIMIT_STACK, &rl);
//            if (result != 0)
//            {
//                fprintf(stderr, "setrlimit returned result = %d\n", result);
//            }
//        }
//    }
}
