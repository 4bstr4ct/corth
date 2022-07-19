#include "../include/parser.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>





#if 0

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
			assert(expression.identifier.buffer != NULL);
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
			expression.literal.string.buffer = (char*)malloc((length + 1) * sizeof(char));
			assert(expression.literal.string.buffer != NULL);
			memcpy(expression.literal.string.buffer, token.value.literal.string.buffer, length);
			expression.literal.string.buffer[length] = '\0';
			expression.literal.string.length = length;
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

		// TODO: ANY TYPE POSSIBLE HERE!
		/*
		case TOKEN_VAR_KEYWORD:
		{
			_expectToken(tokenizer, &token, TOKEN_VAR_KEYWORD, 1);
			_expectToken(tokenizer, &token, TOKEN_IDENTIFIER, 1);
			// Expect = token
			// Parse expression ...
			_expectToken(tokenizer, &token, TOKEN_SEMICOLON, 1);
		} break;
		*/

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

#endif





struct _ProcedureArgument
{
	struct
	{
		char* buffer;
		unsigned int length;
	} type;
	struct
	{
		char* buffer;
		unsigned int length;
	} name;
	struct _ProcedureArgument* next;
};

struct _Procedure
{
	int isConst;
	struct
	{
		char* buffer;
		unsigned int length;
	} name;
	struct
	{
		char* buffer;
		unsigned int length;
	} type;
	struct _ProcedureArgument* firstArgument;
};

static struct _Procedure _parseProcedure(
	struct _Tokenizer* const tokenizer)
{
	struct _Procedure procedure = {0};
	struct _Token token = {0};

	// Parsing proc / constproc
	_nextToken(tokenizer, &token);
	switch (token.type)
	{
		case TOKEN_PROC_KEYWORD:
		{
			procedure.isConst = 0;
		} break;

		case TOKEN_CONSTPROC_KEYWORD:
		{
			procedure.isConst = 1;
		} break;

		default:
		{
			fprintf(stderr, "ERROR: expected TOKEN_PROC_KEYWORD or TOKEN_CONSTPROC_KEYWORD token, but parsed:\n\t%s\n", _stringifyToken(&token));
			_destroyToken(&token);
			exit(1);
		} break;
	}
	_destroyToken(&token);

	// Parsing return type
	if (!_expectToken(tokenizer, &token, TOKEN_LEFT_PARENTHESIS))
	{
		fprintf(stderr, "ERROR: expected TOKEN_LEFT_PARENTHESIS token, but parsed %s\n", _stringifyToken(&token));
		_destroyToken(&token);
		exit(1);
	}

	_destroyToken(&token);
	_nextToken(tokenizer, &token);

	#define SET_DYNAMIC_STRING(stringStructure, source, sourceLength) \
		{ \
			stringStructure.buffer = (char*)malloc((sourceLength + 1) * sizeof(char)); \
			assert(stringStructure.buffer != NULL); \
			memcpy(stringStructure.buffer, source, sourceLength); \
			stringStructure.buffer[sourceLength] = '\0'; \
			stringStructure.length = sourceLength; \
		}

	switch (token.type)
	{
		case TOKEN_VOID_TYPE:
		{
			SET_DYNAMIC_STRING(procedure.type, "void", 4);
		} break;

		case TOKEN_CHAR_TYPE:
		{
			SET_DYNAMIC_STRING(procedure.type, "char", 4);
		} break;

		case TOKEN_INT8_TYPE:
		{
			SET_DYNAMIC_STRING(procedure.type, "int8", 4);
		} break;

		case TOKEN_UINT8_TYPE:
		{
			SET_DYNAMIC_STRING(procedure.type, "uint8", 5);
		} break;

		case TOKEN_INT16_TYPE:
		{
			SET_DYNAMIC_STRING(procedure.type, "int16", 5);
		} break;

		case TOKEN_UINT16_TYPE:
		{
			SET_DYNAMIC_STRING(procedure.type, "uint16", 6);
		} break;

		case TOKEN_INT32_TYPE:
		{
			SET_DYNAMIC_STRING(procedure.type, "int32", 5);
		} break;

		case TOKEN_UINT32_TYPE:
		{
			SET_DYNAMIC_STRING(procedure.type, "uint32", 6);
		} break;

		case TOKEN_INT64_TYPE:
		{
			SET_DYNAMIC_STRING(procedure.type, "int64", 5);
		} break;

		case TOKEN_UINT64_TYPE:
		{
			SET_DYNAMIC_STRING(procedure.type, "uint64", 6);
		} break;

		case TOKEN_FLOAT32_TYPE:
		{
			SET_DYNAMIC_STRING(procedure.type, "float32", 7);
		} break;

		case TOKEN_FLOAT64_TYPE:
		{
			SET_DYNAMIC_STRING(procedure.type, "float64", 7);
		} break;

		// Edge case for user-defined types. Must have some sort of checks!
		case TOKEN_IDENTIFIER:
		{
			SET_DYNAMIC_STRING(procedure.type, token.value.identifier.buffer, token.value.identifier.length);
		} break;

		default:
		{
			fprintf(stderr, "ERROR: expected TOKEN_LEFT_PARENTHESIS token, but parsed %s\n", _stringifyToken(&token));
			_destroyToken(&token);
			exit(1);
		} break;
	}

	_destroyToken(&token);
	if (!_expectToken(tokenizer, &token, TOKEN_RIGHT_PARENTHESIS))
	{
		fprintf(stderr, "ERROR: expected TOKEN_RIGHT_PARENTHESIS token, but parsed %s\n", _stringifyToken(&token));
		_destroyToken(&token);
		exit(1);
	}

	_destroyToken(&token);
	if (!_expectToken(tokenizer, &token, TOKEN_IDENTIFIER))
	{
		fprintf(stderr, "ERROR: expected TOKEN_IDENTIFIER token, but parsed %s\n", _stringifyToken(&token));
		_destroyToken(&token);
		exit(1);
	}

	SET_DYNAMIC_STRING(
		procedure.name,
		token.value.identifier.buffer,
		token.value.identifier.length
	);

	_destroyToken(&token);
	if (!_expectToken(tokenizer, &token, TOKEN_LEFT_PARENTHESIS))
	{
		fprintf(stderr, "ERROR: expected TOKEN_LEFT_PARENTHESIS token, but parsed %s\n", _stringifyToken(&token));
		_destroyToken(&token);
		exit(1);
	}

	_destroyToken(&token);
	struct _ProcedureArgument* end = NULL;

	while (token.type != TOKEN_RIGHT_PARENTHESIS)
	{
		struct _Token typeToken = {0};
		_nextToken(tokenizer, &typeToken);
		if (typeToken.type < TOKEN_VOID_TYPE || typeToken.type > TOKEN_FLOAT64_TYPE)
		{
			fprintf(stderr, "ERROR: expected TOKEN_<TYPE>_TYPE token, but parsed %s\n", _stringifyToken(&typeToken));
			_destroyToken(&typeToken);
			exit(1);
		}

		struct _Token nameToken = {0};
		if (!_expectToken(tokenizer, &nameToken, TOKEN_IDENTIFIER))
		{
			fprintf(stderr, "ERROR: expected TOKEN_IDENTIFIER token, but parsed %s\n", _stringifyToken(&nameToken));
			_destroyToken(&nameToken);
			exit(1);
		}

		struct _ProcedureArgument* newArgument = (struct _ProcedureArgument*)malloc(sizeof(struct _ProcedureArgument));
		SET_DYNAMIC_STRING(newArgument->type, typeToken.value.keyword.buffer, typeToken.value.keyword.length);
		SET_DYNAMIC_STRING(newArgument->name, nameToken.value.identifier.buffer, nameToken.value.identifier.length);
		newArgument->next = NULL;

		if (end != NULL)
		{
			end->next = newArgument;
			end = end->next;
		}
		else
		{
			assert(procedure.firstArgument == NULL);
			procedure.firstArgument = end = newArgument;
		}

		_destroyToken(&typeToken);
		_destroyToken(&nameToken);

		struct _Token commaToken = {0};
		_destroyToken(&token);
		_nextToken(tokenizer, &token);

		if (token.type != TOKEN_COMMA && token.type != TOKEN_RIGHT_PARENTHESIS)
		{
			fprintf(stderr, "ERROR: expected TOKEN_COMMA or TOKEN_RIGHT_PARENTHESIS token, but parsed %s\n", _stringifyToken(&token));
			_destroyToken(&token);
			exit(1);
		}
		_destroyToken(&token);
	}

	_destroyToken(&token);
	if (!_expectToken(tokenizer, &token, TOKEN_COLON))
	{
		fprintf(stderr, "ERROR: expected TOKEN_COLON token, but parsed %s\n", _stringifyToken(&token));
		_destroyToken(&token);
		exit(1);
	}

	_destroyToken(&token);
	if (!_expectToken(tokenizer, &token, TOKEN_END_KEYWORD))
	{
		fprintf(stderr, "ERROR: expected TOKEN_END_KEYWORD token, but parsed %s\n", _stringifyToken(&token));
		_destroyToken(&token);
		exit(1);
	}

	_destroyToken(&token);
	return procedure;
}

void _printProcedure(
	const struct _Procedure* const procedure)
{
	assert(procedure != NULL);

	fprintf(stdout, "%s", procedure->isConst ? "constproc" : "proc");
	fprintf(stdout, "(%s)", procedure->type.buffer);
	fprintf(stdout, " %s(", procedure->name.buffer);

	struct _ProcedureArgument* iterator = procedure->firstArgument;
	while (iterator != NULL)
	{
		fprintf(stdout, "%s %s", iterator->type.buffer, iterator->name.buffer);
		if (iterator->next != NULL) fprintf(stdout, ", ");
		iterator = iterator->next;
	}

	fprintf(stdout, "):\n");
	fprintf(stdout, "\t// Do something here ...\n");
	fprintf(stdout, "end\n");
}

void _parseExample_HelloWorld(
	struct _Tokenizer* const tokenizer)
{
	struct _Token token = {0};
	_peekToken(tokenizer, &token, 0);

	if (token.type == TOKEN_PROC_KEYWORD || token.type == TOKEN_CONSTPROC_KEYWORD)
	{
		struct _Procedure procedure = _parseProcedure(tokenizer);
		_printProcedure(&procedure);
	}
}

void _parse(const char* filePath)
{
#if 1
	struct _Tokenizer tokenizer = _createTokenizer(filePath);
	_parseExample_HelloWorld(&tokenizer);
	_destroyTokenizer(&tokenizer);
#endif

#if 0
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
