#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <gmp.h>

#include <vint.h>
#include <helpers.h>
#include <vmachine.h>
#include <builtins/builtins.h>
#include <allocators/data_alloc.h>
#include <allocators/vterm_alloc.h>
#include <defines/data_struct_sizes.h>

#define EPS 1e-6

typedef void (*ArithOp) (mpz_ptr, mpz_srcptr, mpz_srcptr);

static void readIntOperands(mpz_t x, mpz_t y);
static void readOperand(mpz_t num, struct vterm_t* term);
static struct lterm_t* gcApplyOpToInt(ArithOp op);
static struct lterm_t* gcApplyOpToDouble(ArithOp op);
static struct lterm_t* gcConstructDoubleNumLTerm(double val);
static struct func_result_t gcApplyOp(ArithOp op);

struct func_result_t Add(int entryStatus)
{
    return  gcApplyOp(mpz_add);
}

struct func_result_t Sub(int entryStatus)
{
    return  gcApplyOp(mpz_sub);
}

struct func_result_t Mul(int entryStatus)
{
    return  gcApplyOp(mpz_mul);
}

struct func_result_t Div(int entryStatus)
{
    return  gcApplyOp(mpz_tdiv_q);
}

struct func_result_t Mod(int entryStatus)
{
    return  gcApplyOp(mpz_mod);
}

struct func_result_t Compare(int entryStatus)
{
    gcInitBuiltin();

    if (BUILTIN_FRAG->length != 2)
        PRINT_AND_EXIT(WRONG_OPERANDS_NUMBER);

    if (_memMngr.vterms[BUILTIN_FRAG->offset].tag != _memMngr.vterms[BUILTIN_FRAG->offset + 1].tag )
        PRINT_AND_EXIT(OPERANDS_TYPES_MISMATCH);

    int cmpRes = 0;
    char resChar = '0';

    if (_memMngr.vterms[BUILTIN_FRAG->offset].tag == V_INT_NUM_TAG)
        cmpRes = intCmp(_memMngr.vterms[BUILTIN_FRAG->offset].intNum, _memMngr.vterms[BUILTIN_FRAG->offset + 1].intNum);
    else if (_memMngr.vterms[BUILTIN_FRAG->offset].tag == V_DOUBLE_NUM_TAG)
        cmpRes = doubleCmp(_memMngr.vterms[BUILTIN_FRAG->offset].doubleNum, _memMngr.vterms[BUILTIN_FRAG->offset + 1].doubleNum);
    else
        PRINT_AND_EXIT(OPERAND_BAD_TYPE);

    switch (cmpRes)
    {
        case 0 :
            resChar = '0';
            break;
        case 1 :
            resChar = '+';
            break;
        case -1 :
            resChar = '-';
            break;
    }

    checkAndCleanHeaps(1, BUILTINS_RESULT_SIZE);

    struct lterm_t* resChain = allocateBuiltinsResult(allocateSymbolVTerm(resChar), 1);

    return (struct func_result_t){.status = OK_RESULT, .fieldChain = resChain, .callChain = 0};
}

/// Проверка на равенство двух чисел. 1 - успех, 0 - неудача.
int intCmp(struct vint_t* a, struct vint_t* b)
{
    if (GET_INT_LENGTH(a) == 1 && GET_INT_LENGTH(b) == 1 &&
        a->bytes[0] == 0 && b->bytes[0] == 0)
        return 0;

    int invert = 1;

    if (GET_INT_SIGN(a) > GET_INT_SIGN(b))
        return -1;

    if (GET_INT_SIGN(a) < GET_INT_SIGN(b))
        return 1;

    if (GET_INT_SIGN(a))
        invert = -1;

    if (GET_INT_LENGTH(a) > GET_INT_LENGTH(b))
        return 1 * invert;

    if (GET_INT_LENGTH(a) < GET_INT_LENGTH(b))
        return -1 * invert;

    uint64_t i = 0;
    for (i = 0; i < GET_INT_LENGTH(a); ++i)
    {
        if (a->bytes[i] > b->bytes[i])
            return 1 * invert;

        if (a->bytes[i] < b->bytes[i])
            return -1 * invert;
    }

    return 0;
}

int doubleCmp(double a, double b)
{
    if (b + EPS < a)
        return 1;

    if (a + EPS < b)
        return -1;

    return 0;
}

int ConvertToInt(struct vint_t* numData)
{
    mpz_t num;

    mpz_init(num);

    mpz_import(num, GET_INT_LENGTH(numData), 1, sizeof(uint8_t), 1, 0, numData->bytes);

    if (GET_INT_SIGN(numData))
        mpz_neg(num, num);

    int res = mpz_get_si(num);

    mpz_clear(num);

    return res;
}

static struct func_result_t gcApplyOp(ArithOp op)
{
    gcInitBuiltin();
    struct lterm_t* resChain = 0;

    if (BUILTIN_FRAG->length != 2)
    {
        PRINT_AND_EXIT(WRONG_OPERANDS_NUMBER);
    }

    if (_memMngr.vterms[BUILTIN_FRAG->offset].tag != _memMngr.vterms[BUILTIN_FRAG->offset + 1].tag )
        PRINT_AND_EXIT(OPERANDS_TYPES_MISMATCH);

    if (_memMngr.vterms[BUILTIN_FRAG->offset].tag == V_INT_NUM_TAG)
        resChain = gcApplyOpToInt(op);
    else if (_memMngr.vterms[BUILTIN_FRAG->offset].tag == V_DOUBLE_NUM_TAG)
        resChain = gcApplyOpToDouble(op);
    else
        PRINT_AND_EXIT(OPERAND_BAD_TYPE);

	return (struct func_result_t){.status = OK_RESULT, .fieldChain = resChain, .callChain = 0};
}

static struct lterm_t* gcApplyOpToInt(ArithOp op)
{
    mpz_t x;
    mpz_t y;
    mpz_t res;

    mpz_init(x);
    mpz_init(y);
    mpz_init(res);

    readIntOperands(x, y);

    op(res, x, y);

    struct lterm_t* resChain = gcConstructIntNumBuiltinResult(res);

    mpz_clear(x);
    mpz_clear(y);
    mpz_clear(res);

    return resChain;
}

static struct lterm_t* gcApplyOpToDouble(ArithOp op)
{
    double a = _memMngr.vterms[BUILTIN_FRAG->offset].doubleNum;
    double b = _memMngr.vterms[BUILTIN_FRAG->offset+1].doubleNum;

    if (op == mpz_add)
        return gcConstructDoubleNumLTerm(a + b);
    else if (op == mpz_sub)
        return gcConstructDoubleNumLTerm(a - b);
    else if (op == mpz_mul)
        return gcConstructDoubleNumLTerm(a * b);
    else if (op == mpz_tdiv_q)
        return gcConstructDoubleNumLTerm(a / b);
    else if (op == mpz_mod)
        PRINT_AND_EXIT(MOD_TO_DOUBLE_ERROR);

    PRINT_AND_EXIT(BAD_BINARY_OPERATION);
}

struct lterm_t* gcConstructIntNumBuiltinResult(mpz_t num)
{      
    uint32_t numb = 8 * sizeof(uint8_t);
    uint64_t length = (mpz_sizeinbase (num, 2) + numb - 1) / numb;

    checkAndCleanHeaps(1, VINT_STRUCT_SIZE(length) + BUILTINS_RESULT_SIZE);

    struct vint_t* intNum = allocateIntStruct(length);

    mpz_export(intNum->bytes, &length, 1, sizeof(uint8_t), 1, 0, num);

    if (mpz_sgn(num) < 0)
        SET_INT_SIGN(intNum);

    uint64_t offset = allocateIntNumVTerm(intNum);

    return allocateBuiltinsResult(offset, 1);
}

static struct lterm_t* gcConstructDoubleNumLTerm(double val)
{    
    checkAndCleanHeaps(1, BUILTINS_RESULT_SIZE);

    return allocateBuiltinsResult(allocateDoubleNumVTerm(val), 1);
}

static void readOperand(mpz_t num, struct vterm_t* term)
{
    mpz_import(num, GET_INT_LENGTH(term->intNum), 1, sizeof(uint8_t), 1, 0, term->intNum->bytes);

    if (GET_INT_SIGN(term->intNum))
		mpz_mul_si(num, num, -1);	
}

/// Вычисляет операнды x и y для бинарной операции.
/// BUILTIN_FRAG - фрагмент, с помощью которого нужно вычислить операнды.
static void readIntOperands(mpz_t x, mpz_t y)
{
    struct vterm_t* term = _memMngr.vterms + BUILTIN_FRAG->offset;

    readOperand(x, term);
    term++;
    readOperand(y, term);    
}
