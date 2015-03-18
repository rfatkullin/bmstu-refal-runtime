#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <gmp.h>

#include <vmachine.h>
#include <builtins/builtins.h>
#include <allocators/data_alloc.h>
#include <allocators/vterm_alloc.h>
#include <helpers.h>

#define WRONG_OPERANDS_NUMBER       "It's binary operation!\n"
#define OPERANDS_TYPES_MISMATCH     "Operands must be same type!\n"
#define OPERAND_BAD_TYPE            "Operand must be int or double!\n"
#define BAD_BINARY_OPERATION        "Unknown binary operation\n"
#define MOD_TO_DOUBLE_ERROR         "Can't applay mod operation to double!\n"

#define EPS 1e-6

typedef void (*ArithOp) (mpz_ptr, mpz_srcptr, mpz_srcptr);

static void readIntOperands(mpz_t x, mpz_t y, struct fragment_t* frag);
static void readOperand(mpz_t num, struct v_term* term);
static struct lterm_t* gcApplyOpToInt(ArithOp op, struct fragment_t* frag);
static struct lterm_t* gcApplyOpToDouble(ArithOp op, struct fragment_t* currExpr);
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
    struct fragment_t* frag = gcGetAssembliedChain(_currFuncCall->fieldOfView)->fragment;

    if (frag->length != 2)
        PRINT_AND_EXIT(WRONG_OPERANDS_NUMBER);

    if (memMngr.vterms[frag->offset].tag != memMngr.vterms[frag->offset + 1].tag )
        PRINT_AND_EXIT(OPERANDS_TYPES_MISMATCH);

    int cmpRes = 0;
    char resChar = '0';

    if (memMngr.vterms[frag->offset].tag == V_INT_NUM_TAG)
        cmpRes = intCmp(memMngr.vterms[frag->offset].intNum, memMngr.vterms[frag->offset + 1].intNum);
    else if (memMngr.vterms[frag->offset].tag == V_DOUBLE_NUM_TAG)
        cmpRes = doubleCmp(memMngr.vterms[frag->offset].doubleNum, memMngr.vterms[frag->offset + 1].doubleNum);
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
int intCmp(struct v_int* a, struct v_int* b)
{
    if (a->length == 1 && b->length == 1 &&
        a->bytes[0] == 0 && b->bytes[0] == 0)
        return 0;

    int invert = 1;

    if (a->sign > b->sign)
        return -1;

    if (a->sign < b->sign)
        return 1;

    if (a->sign)
        invert = -1;

    if (a->length > b->length)
        return 1 * invert;

    if (a->length < b->length)
        return -1 * invert;

    uint64_t i = 0;
    for (i = 0; i < a->length; ++i)
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

int ConvertToInt(struct v_int* numData)
{
    mpz_t num;

    mpz_init(num);

    mpz_import(num, numData->length, 1, sizeof(uint8_t), 1, 0, numData->bytes);

    if (numData->sign)
        mpz_neg(num, num);

    int res = mpz_get_si(num);

    mpz_clear(num);

    return res;
}

static struct func_result_t gcApplyOp(ArithOp op)
{
    struct fragment_t* frag = gcGetAssembliedChain(_currFuncCall->fieldOfView)->fragment;
    struct lterm_t* resChain = 0;

    if (frag->length != 2)
        PRINT_AND_EXIT(WRONG_OPERANDS_NUMBER);

    if (memMngr.vterms[frag->offset].tag != memMngr.vterms[frag->offset + 1].tag )
        PRINT_AND_EXIT(OPERANDS_TYPES_MISMATCH);

    if (memMngr.vterms[frag->offset].tag == V_INT_NUM_TAG)
        resChain = gcApplyOpToInt(op, frag);
    else if (memMngr.vterms[frag->offset].tag == V_DOUBLE_NUM_TAG)
        resChain = gcApplyOpToDouble(op, frag);
    else
        PRINT_AND_EXIT(OPERAND_BAD_TYPE);

	return (struct func_result_t){.status = OK_RESULT, .fieldChain = resChain, .callChain = 0};
}

static struct lterm_t* gcApplyOpToInt(ArithOp op, struct fragment_t* frag)
{
    mpz_t x;
    mpz_t y;
    mpz_t res;

    mpz_init(x);
    mpz_init(y);
    mpz_init(res);

    readIntOperands(x, y, frag);

    op(res, x, y);

    struct lterm_t* resChain = gcConstructIntNumBuiltinResult(res);

    mpz_clear(x);
    mpz_clear(y);
    mpz_clear(res);

    return resChain;
}

static struct lterm_t* gcApplyOpToDouble(ArithOp op, struct fragment_t* frag)
{
    double a = memMngr.vterms[frag->offset].doubleNum;
    double b = memMngr.vterms[frag->offset+1].doubleNum;

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

    struct v_int* intNum = allocateIntStruct(length);

    mpz_export(intNum->bytes, &length, 1, sizeof(uint8_t), 1, 0, num);
    intNum->sign = mpz_sgn(num) < 0;

    uint64_t offset = allocateIntNumVTerm(intNum);

    return allocateBuiltinsResult(offset, 1);
}

static struct lterm_t* gcConstructDoubleNumLTerm(double val)
{    
    checkAndCleanHeaps(1, BUILTINS_RESULT_SIZE);

    return allocateBuiltinsResult(allocateDoubleNumVTerm(val), 1);
}

static void readOperand(mpz_t num, struct v_term* term)
{
    mpz_import(num, term->intNum->length, 1, sizeof(uint8_t), 1, 0, term->intNum->bytes);

    if (term->intNum->sign)
		mpz_mul_si(num, num, -1);	
}

/// Вычисляет операнды x и y для бинарной операции.
/// frag - фрагмент, с помощью которого нужно вычислить операнды.
static void readIntOperands(mpz_t x, mpz_t y, struct fragment_t* frag)
{
    struct v_term* term = memMngr.vterms + frag->offset;

    readOperand(x, term);
    term++;
    readOperand(y, term);
}
