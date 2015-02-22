#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <gmp.h>

#include "vmachine.h"
#include "builtins.h"

typedef void (*ArithOp) (mpz_ptr, mpz_srcptr, mpz_srcptr);

static uint32_t writeOperand(mpz_t num);
static struct lterm_t* constructNumLTerm(mpz_t num);
static void readOperands(mpz_t x, mpz_t y, struct fragment_t* frag);
static uint32_t readOperand(mpz_t num, uint32_t currOffset, uint32_t maxOffset, int all);
static struct func_result_t applyOp(ArithOp op, int* entryPoint, struct env_t* env, struct lterm_t* fieldOfView);

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
	int sign = 1;

	mpz_init(res);

	readOperands(x, y, currExpr->fragment);

	if (op == mpz_mod)
	{
		sign = mpz_cmp_ui(y, 0);
		if (sign < 0)
			mpz_neg(y, y);
	}

	op(res, x, y);

	if (op == mpz_mod && sign < 0)
		mpz_neg(res, res);

	struct lterm_t* resChain = constructNumLTerm(res);

	mpz_clear(x);
	mpz_clear(y);
	mpz_clear(res);

	return (struct func_result_t){.status = OK_RESULT, .fieldChain = resChain, .callChain = 0};
}


static void numParseFailed()
{
	printf("Failed: Can't parse int num!\n");
	exit(0);
}

static struct lterm_t* constructNumLTerm(mpz_t num)
{
	uint32_t offset = memMngr.vtermsOffset;
	uint32_t length = writeOperand(num);

	return constructLterm(offset, length);
}

static uint32_t readOperand(mpz_t num, uint32_t currOffset, uint32_t maxOffset, int all)
{
	uint32_t sign = 0;

	mpz_init_set_ui(num, 0);

	if (memMngr.vterms[currOffset].tag == V_CHAR_TAG)
	{
		currOffset++;
		sign = 1;
	}

	if (currOffset >= maxOffset)
		numParseFailed();

	do
	{
		if (memMngr.vterms[currOffset].tag != V_INT_NUM_TAG)
			numParseFailed();

		mpz_mul(num, num, base);
		mpz_add_ui(num, num, memMngr.vterms[currOffset].intNum);
		currOffset++;
	}
	while (currOffset < maxOffset && all);

	if (sign)
		mpz_mul_si(num, num, -1);

	return currOffset;
}

static uint32_t getNumsCount(mpz_t num)
{
	uint32_t count = 0;
	mpz_t tmp;

	mpz_init_set(tmp, num);

	if (!mpz_cmp_ui(tmp, 0))
	{
		count = 1;
	}
	else
	{
		while (mpz_cmp_ui(tmp, 0) > 0)
		{
			mpz_div(tmp, tmp, base);
			count++;
		}
	}

	mpz_clear(tmp);

	return count;
}

static uint32_t writeOperand(mpz_t num)
{
	uint32_t i = 0;
	uint32_t sign = 0;
	mpz_t remainder;
	mpz_t quotient;

	mpz_init_set_ui(remainder, 0);
	mpz_init_set_ui(quotient, 0);

	if (mpz_cmp_ui(num, 0) < 0)
	{
		sign = 1;
		allocateSymbol('-');
		mpz_neg(num, num);
	}

	uint32_t count = getNumsCount(num);
	uint32_t offset = allocateIntNum(count);

	for (i = offset + count - 1; i >= offset; --i)
	{
		mpz_divmod(quotient, remainder, num, base);
		memMngr.vterms[i].intNum = mpz_get_ui(remainder);
		mpz_set(num, quotient);
	}

	mpz_clear(remainder);
	mpz_clear(quotient);

	return count + sign;
}

static void readOperands(mpz_t x, mpz_t y, struct fragment_t* frag)
{
	uint32_t currOffset = frag->offset;
	uint32_t maxOffset = frag->offset + frag->length;

	if (memMngr.vterms[currOffset].tag == V_BRACKET_TAG)
	{
		currOffset = readOperand(x, currOffset + 1, currOffset + memMngr.vterms[currOffset].inBracketLength - 1, 1);

		if (memMngr.vterms[currOffset].tag != V_BRACKET_TAG || memMngr.vterms[currOffset].inBracketLength != 0)
			numParseFailed();

		currOffset = readOperand(y, currOffset + 1, maxOffset, 1);
	}
	else
	{
		currOffset = readOperand(x, currOffset, maxOffset, 0);
		currOffset = readOperand(y, currOffset, maxOffset, 1);
	}

	if (currOffset < maxOffset)
		numParseFailed();
}
