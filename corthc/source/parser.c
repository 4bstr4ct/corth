#include "../include/parser.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>





#pragma region EXPRESSION API

static_assert(LITERAL_EXPRESSION_TYPES_COUNT == 13, "LITERAL_EXPRESSION_TYPES_COUNT is higher than accounted in _stringifiedLiteralExpressionTypes!");
static const char* const _stringifiedLiteralExpressionTypes[LITERAL_EXPRESSION_TYPES_COUNT] =
{
	[LITERAL_EXPRESSION_IDENTIFIER]	= "LITERAL_EXPRESSION_IDENTIFIER",
	[LITERAL_EXPRESSION_CHAR]		= "LITERAL_EXPRESSION_CHAR",
	[LITERAL_EXPRESSION_INT8]		= "LITERAL_EXPRESSION_INT8",
	[LITERAL_EXPRESSION_UINT8]		= "LITERAL_EXPRESSION_UINT8",
	[LITERAL_EXPRESSION_INT16]		= "LITERAL_EXPRESSION_INT16",
	[LITERAL_EXPRESSION_UINT16]		= "LITERAL_EXPRESSION_UINT16",
	[LITERAL_EXPRESSION_INT32]		= "LITERAL_EXPRESSION_INT32",
	[LITERAL_EXPRESSION_UINT32]		= "LITERAL_EXPRESSION_UINT32",
	[LITERAL_EXPRESSION_INT64]		= "LITERAL_EXPRESSION_INT64",
	[LITERAL_EXPRESSION_UINT64]		= "LITERAL_EXPRESSION_UINT64",
	[LITERAL_EXPRESSION_FLOAT32]	= "LITERAL_EXPRESSION_FLOAT32",
	[LITERAL_EXPRESSION_FLOAT64]	= "LITERAL_EXPRESSION_FLOAT64",
	[LITERAL_EXPRESSION_STRING]		= "LITERAL_EXPRESSION_STRING"
};
static_assert(sizeof(_stringifiedLiteralExpressionTypes) / sizeof(const char* const), "Size of _stringifiedLiteralExpressionTypes is not equal to LITERAL_EXPRESSION_TYPES_COUNT!");

const char* _stringifyLiteralExpressionType(
	const enum _LiteralExpressionType type)
{
	assert(type >= 0 && type < LITERAL_EXPRESSION_TYPES_COUNT);
	return _stringifiedLiteralExpressionTypes[type];
}

static struct _Expression _parseExpression(
	struct _Tokenizer* const tokenizer);

static struct _LiteralExpression _parseLiteralExpression(
	struct _Tokenizer* const tokenizer)
{
	struct _LiteralExpression expression = {0};
	struct _Token token = {0};
	_nextToken(tokenizer, &token);

	switch (token.type)
	{
		case TOKEN_IDENTIFIER:
		{
			const unsigned int length = token.value.identifier.length;
			expression.identifier.buffer = (char*)malloc((length + 1) * sizeof(char));
			memcpy(expression.identifier.buffer, token.value.identifier.buffer, length);
			expression.identifier.buffer[length] = '\0';
			expression.identifier.length = length;
		} break;

		case TOKEN_CHAR_LITERAL:
		{
			expression.literal.c = token.value.literal.c;
		} break;

		case TOKEN_INT8_LITERAL:
		{
			expression.literal.i8 = token.value.literal.i8;
		} break;

		case TOKEN_UINT8_LITERAL:
		{
			expression.literal.u8 = token.value.literal.u8;
		} break;

		case TOKEN_INT16_LITERAL:
		{
			expression.literal.i16 = token.value.literal.i16;
		} break;

		case TOKEN_UINT16_LITERAL:
		{
			expression.literal.u16 = token.value.literal.u16;
		} break;

		case TOKEN_INT32_LITERAL:
		{
			expression.literal.i32 = token.value.literal.i32;
		} break;

		case TOKEN_UINT32_LITERAL:
		{
			expression.literal.u32 = token.value.literal.u32;
		} break;

		case TOKEN_INT64_LITERAL:
		{
			expression.literal.i64 = token.value.literal.i64;
		} break;

		case TOKEN_UINT64_LITERAL:
		{
			expression.literal.u64 = token.value.literal.u64;
		} break;

		case TOKEN_FLOAT32_LITERAL:
		{
			expression.literal.f32 = token.value.literal.f32;
		} break;

		case TOKEN_FLOAT64_LITERAL:
		{
			expression.literal.f64 = token.value.literal.f64;
		} break;

		case TOKEN_STRING_LITERAL:
		{
			const unsigned int length = token.value.literal.string.length;
			expression.identifier.buffer = (char*)malloc((length + 1) * sizeof(char));
			memcpy(expression.identifier.buffer, token.value.literal.string.buffer, length);
			expression.identifier.buffer[length] = '\0';
			expression.identifier.length = length;
		} break;

		case TOKEN_LEFT_PARENTHESIS:
		{
			_parseExpression(tokenizer);
			_destroyToken(&token);
			_expectToken(tokenizer, &token, TOKEN_RIGHT_PARENTHESIS, 1);
		} break;

		default:
		{
			_destroyToken(&token);
			fprintf(stderr, "ERROR: failed to parse literal expression - unkown token was parsed!\nParsed %s!\n", _stringifyToken(&token));
			exit(1);
		} break;
	}

	_destroyToken(&token);
	return expression;
}

static_assert(EXPRESSION_TYPES_COUNT == 5, "EXPRESSION_TYPES_COUNT is higher than accounted in _stringifiedExpressionTypes!");
static const char* const _stringifiedExpressionTypes[EXPRESSION_TYPES_COUNT] =
{
	[EXPRESSION_LITERAL]			= "EXPRESSION_LITERAL",
	[EXPRESSION_INDEXING]			= "EXPRESSION_INDEXING",
	[EXPRESSION_PROCEDURE_CALL]		= "EXPRESSION_PROCEDURE_CALL",
	[EXPRESSION_BINARY]				= "EXPRESSION_BINARY",
	[EXPRESSION_UNARY]				= "EXPRESSION_UNARY"
};
static_assert(sizeof(_stringifiedExpressionTypes) / sizeof(const char* const), "Size of _stringifiedExpressionTypes is not equal to EXPRESSION_TYPES_COUNT!");

const char* _stringifyExpressionType(
	const enum _ExpressionType type)
{
	assert(type >= 0 && type < EXPRESSION_TYPES_COUNT);
	return _stringifiedExpressionTypes[type];
}

static struct _Expression _parseExpression(
	struct _Tokenizer* const tokenizer)
{
	struct _Expression expression = {0};
	struct _Token token = {0};

	_peekToken(tokenizer, &token, 0);

	switch (token.type)
	{
		case TOKEN_IDENTIFIER:
		case TOKEN_CHAR_LITERAL:
		case TOKEN_INT8_LITERAL:
		case TOKEN_UINT8_LITERAL:
		case TOKEN_INT16_LITERAL:
		case TOKEN_UINT16_LITERAL:
		case TOKEN_INT32_LITERAL:
		case TOKEN_UINT32_LITERAL:
		case TOKEN_INT64_LITERAL:
		case TOKEN_UINT64_LITERAL:
		case TOKEN_FLOAT32_LITERAL:
		case TOKEN_FLOAT64_LITERAL:
		case TOKEN_STRING_LITERAL:
		case TOKEN_LEFT_PARENTHESIS:
		{
			expression.type = EXPRESSION_LITERAL;
			expression.kind.literal = _parseLiteralExpression(tokenizer);
		} break;

		default:
		{
			_parseExpression(tokenizer);
		} break;
	}

	_destroyToken(&token);
	return expression;
}

#pragma endregion





#pragma region STATEMENT API

static_assert(STATEMENT_TYPES_COUNT == 7, "STATEMENT_TYPES_COUNT is higher than accounted in _stringifiedStatementTypes!");
static const char* const _stringifiedStatementTypes[STATEMENT_TYPES_COUNT] =
{
	[STATEMENT_COMPOUND]			= "STATEMENT_COMPOUND",
	[STATEMENT_CONDITIONAL]			= "STATEMENT_CONDITIONAL",
	[STATEMENT_LOOP]				= "STATEMENT_LOOP",
	[STATEMENT_RETURN]				= "STATEMENT_RETURN",
	[STATEMENT_VARIABLE_DEFINITION]	= "STATEMENT_VARIABLE_DEFINITION",
	[STATEMENT_EXPRESSION]			= "STATEMENT_EXPRESSION",
	[STATEMENT_NO_OPERATION]		= "STATEMENT_NO_OPERATION"
};
static_assert(sizeof(_stringifiedStatementTypes) / sizeof(const char* const), "Size of _stringifiedStatementTypes is not equal to STATEMENT_TYPES_COUNT!");

const char* _stringifyStatementType(
	const enum _StatementType type)
{
	assert(type >= 0 && type < STATEMENT_TYPES_COUNT);
	return _stringifiedStatementTypes[type];
}

static struct _Statement* _parseStatement(struct _Tokenizer* const tokenizer)
{
	struct _Statement* statement = (struct _Statement*)malloc(sizeof(struct _Statement));
	struct _Token token = {0};
	_nextToken(tokenizer, &token);

	switch (token.type)
	{
		case TOKEN_LEFT_BRACE:
		{
			if (token.type != TOKEN_RIGHT_BRACE)
			{
				_parseStatement(tokenizer);
			}
			else
			{
				_expectToken(tokenizer, &token, TOKEN_RIGHT_BRACE, 1);
			}
		} break;

		case TOKEN_IF_KEYWORD:
		case TOKEN_WHILE_KEYWORD:
		{
			_expectToken(tokenizer, &token, TOKEN_LEFT_PARENTHESIS, 1);
			_parseExpression(tokenizer);
			_expectToken(tokenizer, &token, TOKEN_RIGHT_PARENTHESIS, 1);
		} break;

		case TOKEN_RETURN_KEYWORD:
		{
			_parseExpression(tokenizer);
			_expectToken(tokenizer, &token, TOKEN_SEMICOLON, 1);
		} break;

		case TOKEN_VAR_KEYWORD:
		{
			_expectToken(tokenizer, &token, TOKEN_VAR_KEYWORD, 1);
			_expectToken(tokenizer, &token, TOKEN_IDENTIFIER, 1);
			// Expect = token
			// Parse expression ...
			_expectToken(tokenizer, &token, TOKEN_SEMICOLON, 1);
		} break;

		default:
		{
			// Parse expression ...
			_expectToken(tokenizer, &token, TOKEN_SEMICOLON, 1);
		} break;
	}	

	_destroyToken(&token);
	return statement;
}

#pragma endregion





#pragma region PROCEDURE API

static struct _Procedure _parseProcedure(struct _Tokenizer* const tokenizer)
{
	struct _Procedure procedure = {0};

	struct _Token token = {0};

	// Parsing name
	enum _TokenType parsedType = _expectToken(tokenizer, &token, TOKEN_PROC_KEYWORD, 1);

	if (parsedType != TOKEN_PROC_KEYWORD)
	{
		_destroyToken(&token);
		fprintf(stderr, "ERROR: expected TOKEN_PROC_KEYWORD token, but parsed %s!\n", _stringifyTokenType(parsedType));
		exit(1);
	}

	_destroyToken(&token);
	parsedType = _expectToken(tokenizer, &token, TOKEN_IDENTIFIER, 1);

	if (parsedType != TOKEN_IDENTIFIER)
	{
		_destroyToken(&token);
		fprintf(stderr, "ERROR: expected TOKEN_IDENTIFIER token, but parsed %s!\n", _stringifyTokenType(parsedType));
		exit(1);
	}

	_destroyToken(&token);

	// Parsing body
	struct _Statement* statementsBegin = NULL;
	struct _Statement* statementsEnd = NULL;

	while (token.type != TOKEN_RIGHT_BRACE)
	{
		// _destroyToken(&token);
		// _nextToken(tokenizer, &token);
		/*
		struct _Statement* statementNode = _parseStatement(tokenizer);

		if (statementsEnd != NULL)
		{
			statementsEnd->next = statementNode;
			statementsEnd = statementNode;
		}
		else
		{
			assert(statementsBegin == NULL);
			statementsBegin = statementsEnd = statementNode;
		}
		*/
	}

	_destroyToken(&token);
	return procedure;
}

#pragma endregion





void _parse(const char* filePath)
{
#if 0
	struct _Tokenizer tokenizer = _createTokenizer(filePath);

	_parseProcedure(&tokenizer);

	_destroyTokenizer(&tokenizer);
#endif

#if 1
	struct _Tokenizer tokenizer = _createTokenizer(filePath);
	struct _Token token = {0};

	do
	{
		_nextToken(&tokenizer, &token);
		fprintf(stdout, "%s\n", _stringifyToken(&token));
		_destroyToken(&token);
	} while (token.type != TOKEN_END_OF_FILE);

	_destroyTokenizer(&tokenizer);
#endif
}
