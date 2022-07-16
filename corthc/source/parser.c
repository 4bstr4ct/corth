#include "../include/parser.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

static struct _Expression _parseExpression(struct _Lexer* const lexer)
{
	struct _Expression expression = {0};
	struct _Token token = {0};

	_nextToken(lexer, &token);
	fprintf(stderr, "%s\n", _stringifyToken(&token));

	switch (token.type)
	{
		case TOKEN_LEFT_PARENTHESIS:
		{

		} break;

		case TOKEN_STRING_LITERAL:
		{

		} break;

		default:
		{
			fprintf(stderr, "ERROR: unknown expression:\n\t%s!\n", _stringifyToken(&token));
			exit(1);
		} break;
	}

	if (token.type == TOKEN_STRING_LITERAL)
	{
		expression.type = EXPRESSION_STRING_LITERAL;
	}
	else if (token.type == TOKEN_IDENTIFIER)
	{
		_destroyToken(&token);
		_nextToken(lexer, &token);
		if (token.type == TOKEN_LEFT_PARENTHESIS)
		{
			expression.type = EXPRESSION_PROCEDURE_CALL;
		}
		else
		{
			_destroyToken(&token);
			fprintf(stderr, "ERROR: unknown expression:\n\t%s!\n", _stringifyToken(&token));
			exit(1);
		}
	}
	else
	{
		_destroyToken(&token);
		fprintf(stderr, "ERROR: unknown expression:\n\t%s!\n", _stringifyToken(&token));
		exit(1);
	}

	_destroyToken(&token);
	return expression;
}

static struct _Block* _parseBlock(struct _Lexer* const lexer)
{
	struct _Block* block = (struct _Block*)malloc(sizeof(struct _Block));

	struct _Block* begin = NULL;
	struct _Block* end = NULL;

	struct _Token token = {0};
	_expectToken(lexer, &token, TOKEN_LEFT_BRACE, 0);
	fprintf(stderr, "%s\n", _stringifyToken(&token));

	while (token.type != TOKEN_RIGHT_BRACE)
	{
		struct _Block* node = (struct _Block*)malloc(sizeof(struct _Block));
		node->statement.expression = _parseExpression(lexer);

		if (end != NULL)
		{
			end->next = node;
			end = node;
		}
		else
		{
			assert(begin == NULL);
			begin = end = node;
		}
	}

	_destroyToken(&token);
	_expectToken(lexer, &token, TOKEN_RIGHT_BRACE, 0);
	_destroyToken(&token);
	return block;
}

static struct _Procedure _parseProcedure(struct _Lexer* const lexer)
{
	struct _Procedure procedure = {0};

	struct _Token token = {0};
	_expectToken(lexer, &token, TOKEN_PROC_KEYWORD, 0);
	_expectToken(lexer, &token, TOKEN_IDENTIFIER, 0);
	procedure.name = token.value.identifier.buffer;
	_expectToken(lexer, &token, TOKEN_LEFT_PARENTHESIS, 0);
	_expectToken(lexer, &token, TOKEN_RIGHT_PARENTHESIS, 0);

	procedure.body = _parseBlock(lexer);

	_destroyToken(&token);
	return procedure;
}

static void _printProcedure(const struct _Procedure* const procedure)
{
	(void*)procedure;
}

static struct _Procedure _parseProcedure(struct _Lexer* const lexer)
{
	struct _Procedure procedure = {0};

	

	return procedure;
}

void _parse(const char* filePath)
{
	struct _Lexer lexer = _createLexer(filePath);

	_parseProcedure(&lexer);

	_destroyLexer(&lexer);

	/*
	struct _Lexer lexer = _createLexer(filePath);
	struct _Token token = {0};

	do
	{
		_nextToken(&lexer, &token);
		fprintf(stdout, "%s\n", _stringifyToken(&token));
		_destroyToken(&token);
	} while (token.type != TOKEN_END_OF_FILE);

	_destroyLexer(&lexer);
	*/
}
