#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <gmp.h>

#include <vmachine.h>
#include <builtins/builtins.h>
#include <allocators/data_alloc.h>
#include <allocators/vterm_alloc.h>

#define WRONG_OPERANDS_NUMBER "It's binary operation!"
#define OPERANDS_TYPES_MISMATCH "Operands must be same type!"
#define OPERAND_BAD_TYPE "Operand must be int or double!"
#define BAD_BINARY_OPERATION "Unknown binary operation"
#define MOD_TO_DOUBLE_ERROR "Can't applay mod operation to double!"

#define EPS 1e-6

typedef void (*ArithOp) (mpz_ptr, mpz_srcptr, mpz_srcptr);

static struct lterm_t* constructDoubleNumLTerm(double val);
static void readIntOperands(mpz_t x, mpz_t y, struct fragment_t* frag);
static void readOperand(mpz_t num, struct v_term* term);
static struct func_result_t applyOp(ArithOp op, int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView);
static void numParseFailed();
static struct lterm_t* applyOpToInt(ArithOp op, struct fragment_t* frag);
static struct lterm_t* applyOpToDouble(ArithOp op, struct fragment_t* currExpr);

struct func_result_t Add(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView, int firstCall)
{
	return  applyOp(mpz_add, entryPoint, env, fieldOfView);
}

struct func_result_t Sub(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView, int firstCall)
{
	return  applyOp(mpz_sub, entryPoint, env, fieldOfView);
}

struct func_result_t Mul(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView, int firstCall)
{
	return  applyOp(mpz_mul, entryPoint, env, fieldOfView);
}

struct func_result_t Div(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView, int firstCall)
{
	return  applyOp(mpz_tdiv_q, entryPoint, env, fieldOfView);
}

struct func_result_t Mod(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView, int firstCall)
{
	return  applyOp(mpz_mod, entryPoint, env, fieldOfView);
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
        mpz_mul_si(num, num, -1);

    int res = mpz_get_si(num);

    mpz_clear(num);

    return res;
}

static struct func_result_t applyOp(ArithOp op, int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView)
{
    struct fragment_t* frag = gcGetAssembliedChain(fieldOfView)->fragment;
    struct lterm_t* resChain = 0;

    if (frag->length != 2)
        numParseFailed(WRONG_OPERANDS_NUMBER);

    if (memMngr.vterms[frag->offset].tag != memMngr.vterms[frag->offset + 1].tag )
        numParseFailed(OPERANDS_TYPES_MISMATCH);

    if (memMngr.vterms[frag->offset].tag == V_INT_NUM_TAG)
        resChain = applyOpToInt(op, frag);
    else if (memMngr.vterms[frag->offset].tag == V_DOUBLE_NUM_TAG)
        resChain = applyOpToDouble(op, frag);
    else
        numParseFailed(OPERAND_BAD_TYPE);

	return (struct func_result_t){.status = OK_RESULT, .fieldChain = resChain, .callChain = 0};
}

static struct lterm_t* applyOpToInt(ArithOp op, struct fragment_t* frag)
{
    mpz_t x;
    mpz_t y;
    mpz_t res;

    mpz_init(x);
    mpz_init(y);
    mpz_init(res);

    readIntOperands(x, y, frag);

    op(res, x, y);

    struct lterm_t* resChain = constructIntNumBuiltinResult(res);

    mpz_clear(x);
    mpz_clear(y);
    mpz_clear(res);

    return resChain;
}

static struct lterm_t* applyOpToDouble(ArithOp op, struct fragment_t* frag)
{
    double a = memMngr.vterms[frag->offset].doubleNum;
    double b = memMngr.vterms[frag->offset+1].doubleNum;

    if (op == mpz_add)
        return constructDoubleNumLTerm(a + b);
    else if (op == mpz_sub)
        return constructDoubleNumLTerm(a - b);
    else if (op == mpz_mul)
        return constructDoubleNumLTerm(a * b);
    else if (op == mpz_tdiv_q)
        return constructDoubleNumLTerm(a / b);
    else if (op == mpz_mod)
        numParseFailed(MOD_TO_DOUBLE_ERROR);

    numParseFailed(BAD_BINARY_OPERATION);
}

struct lterm_t* constructIntNumBuiltinResult(mpz_t num)
{      
    uint32_t numb = 8 * sizeof(uint8_t);
    uint64_t length = (mpz_sizeinbase (num, 2) + numb - 1) / numb;

    checkAndCleanVTerms(1);
    checkAndCleanData(VINT_STRUCT_SIZE(length) + BUILTINS_RESULT_SIZE);

    struct v_int* intNum = allocateIntStruct(length);

    mpz_export(intNum->bytes, &length, 1, sizeof(uint8_t), 1, 0, num);
    intNum->sign = mpz_sgn(num) < 0;

    uint64_t offset = allocateIntNumVTerm(intNum);

    return allocateBuiltinsResult(offset, 1);
}

static struct lterm_t* constructDoubleNumLTerm(double val)
{    
    checkAndCleanVTerms(1);
    checkAndCleanData(BUILTINS_RESULT_SIZE);

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

static void numParseFailed(char* msg)
{
    printf("Fail: %s\n", msg);
    exit(0);
}

