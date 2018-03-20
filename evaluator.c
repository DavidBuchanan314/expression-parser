#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>

enum operator {
	OP_NUL = 0, /* not a valid operator */
	OP_LIT, /* pseudo-op used to denote a literal value expression */
	OP_ADD,
	OP_SUB,
	OP_MUL,
	OP_DIV,
	OP_NUM /* the number of operators */
};

struct expr {
	enum operator op;
	int value; /* only assigned if op == OP_LIT */
	struct expr *left;
	struct expr *right;
};

static enum operator symbol_operators[0x100] = {
	['+'] = OP_ADD,
	['-'] = OP_SUB,
	['*'] = OP_MUL,
	['/'] = OP_DIV
};

static char operator_symbols[OP_NUM] = {
	[OP_ADD] = '+',
	[OP_SUB] = '-',
	[OP_MUL] = '*',
	[OP_DIV] = '/'
};

const char *parse_ws(const char *buf)
{
	while (isspace(*buf)) buf++;
	return buf;
}

enum operator parse_operator(const char *buf)
{
	return symbol_operators[(int) *buf];
}

/* if parse failed, *end == NULL */
int parse_int(const char *buf, const char **end)
{
	int value = 0;

	if (!isdigit(*buf)) {
		*end = NULL;
		return 0;
	}

	while (isdigit(*buf)) {
		value *= 10;
		value += *buf - '0';
		buf++;
	}

	*end = buf;
	return value;
}

/* returns NULL on failure */
struct expr *parse_expression(const char *buf, const char **end)
{
	const char *tmp;
	struct expr *expression;

	expression = calloc(1, sizeof(*expression));

	tmp = parse_ws(buf);

	expression->value = parse_int(tmp, &buf);
	if (buf != NULL) {
		expression->op = OP_LIT;
		*end = parse_ws(buf);
		return expression;
	}

	if (*tmp++ != '(') goto error;

	expression->left = parse_expression(tmp, &buf);
	if (expression->left == NULL) goto error;
	
	expression->op = parse_operator(buf++);
	if (expression->op == OP_NUL) goto error;
	
	expression->right = parse_expression(buf, &buf);
	if (expression->right == NULL) goto error;

	if (*buf++ != ')') goto error;

	*end = parse_ws(buf);
	return expression;

error:
	free(expression);
	return NULL;
}

struct expr *parse(const char *buf)
{
	struct expr *expression = parse_expression(buf, &buf);
	if (*buf) return NULL;
	return expression;
}

void dump(struct expr *expression)
{
	if (expression->op == OP_LIT) {
		printf("%d", expression->value);
		return;
	}

	printf("(");
	dump(expression->left);
	printf(" %c ", operator_symbols[expression->op]);
	dump(expression->right);
	printf(")");
}

int eval(struct expr *expression)
{
	int left, right;

	if (expression->op == OP_LIT) return expression->value;

	left = eval(expression->left);
	right = eval(expression->right);

	switch (expression->op) {
		case OP_ADD:
			return left + right;
		case OP_SUB:
			return left - right;
		case OP_MUL:
			return left * right;
		case OP_DIV:
			return left / right;
		default:
			assert(0); /* we should never get here... */
	}
}

int main(int argc, char *argv[])
{
	struct expr *expression;

	if (argc != 2) {
		printf("USAGE: %s expression\n", argv[0]);
		return -1;
	}

	expression = parse(argv[1]);

	if (expression == NULL) {
		printf("Invalid expression!\n");
		return -1;
	}

	dump(expression);
	printf(" = %d\n", eval(expression));

	return 0;
}
