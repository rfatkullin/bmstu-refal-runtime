#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <gmp.h>

#include "vmachine.h"
#include "builtins.h"

typedef void (*ArithOp) (mpz_ptr, mpz_srcptr, mpz_srcptr);

static void writeOperand(mpz_t num);
static struct lterm_t* constructNumLTerm(mpz_t num);
static void readOperands(mpz_t x, mpz_t y, struct fragment_t* frag);
static void readOperand(mpz_t num, struct v_term* term);
static struct func_result_t applyOp(ArithOp op, int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView);
static void numParseFailed();

struct func_result_t Add(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView)
{
	return  applyOp(mpz_add, entryPoint, env, fieldOfView);
}

struct func_result_t Sub(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView)
{
	return  applyOp(mpz_sub, entryPoint, env, fieldOfView);
}

struct func_result_t Mul(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView)
{
	return  applyOp(mpz_mul, entryPoint, env, fieldOfView);
}

struct func_result_t Div(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView)
{
	return  applyOp(mpz_tdiv_q, entryPoint, env, fieldOfView);
}

struct func_result_t Mod(int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView)
{
	return  applyOp(mpz_mod, entryPoint, env, fieldOfView);
}

struct func_result_t applyOp(ArithOp op, int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView)
{
	struct lterm_t* currExpr = getAssembliedChain(fieldOfView);

	mpz_t x;
	mpz_t y;
	mpz_t res;    

    mpz_init(x);
    mpz_init(y);
	mpz_init(res);

	readOperands(x, y, currExpr->fragment);

	op(res, x, y);

	struct lterm_t* resChain = constructNumLTerm(res);

	mpz_clear(x);
	mpz_clear(y);
	mpz_clear(res);

	return (struct func_result_t){.status = OK_RESULT, .fieldChain = resChain, .callChain = 0};
}


static struct lterm_t* constructNumLTerm(mpz_t num)
{
    uint64_t offset = memMngr.vtermsOffset;
    writeOperand(num);

    return constructLterm(offset, 1);
}

static void readOperand(mpz_t num, struct v_term* term)
{
    mpz_import(num, term->intNum->length, 1, sizeof(uint8_t), 1, 0, term->intNum->bytes);

    if (term->intNum->sign)
		mpz_mul_si(num, num, -1);	
}

/// Записывает целочисленное значение в vterm'ы.
/// Возвращает количество созданных vterm'ов.
static void writeOperand(mpz_t num)
{
    uint32_t numb = 8 * sizeof(uint8_t);
    uint64_t length = (mpz_sizeinbase (num, 2) + numb - 1) / numb;

    struct v_int* intNum = allocateIntNumber(length);

    mpz_export(intNum->bytes, &length, 1, sizeof(uint8_t), 1, 0, num);
    intNum->sign = mpz_sgn(num) < 0;

    uint64_t offset = allocateIntNum(1);
    memMngr.vterms[offset].intNum = intNum;
}

/// Вычисляет операнды x и y для бинарной операции.
/// frag - фрагмент, с помощью которого нужно вычислить операнды.
static void readOperands(mpz_t x, mpz_t y, struct fragment_t* frag)
{    
    if (frag->length != 2 || memMngr.vterms[frag->offset].tag != memMngr.vterms[frag->offset + 1].tag )
        numParseFailed();

    struct v_term* term = memMngr.vterms + frag->offset;

    readOperand(x, term);
    term++;
    readOperand(y, term);
}

static void numParseFailed()
{
    printf("Failed: Can't parse int num!\n");
    exit(0);
}

