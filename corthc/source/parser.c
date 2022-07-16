#include "../include/parser.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

/*
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
*/

static struct _ProcedureArg* _parseProcedureArg(struct _Lexer* const lexer)
{
	struct _ProcedureArg* arg = (struct _ProcedureArg*)malloc(sizeof(struct _ProcedureArg));
	struct _Token token = {0};
	_expectToken(lexer, &token, TOKEN_IDENTIFIER, 1);

	arg->name = (char*)malloc((token.value.identifier.length + 1) * sizeof(char));
	memcpy(arg->name, token.value.identifier.buffer, token.value.identifier.length);
	arg->name[token.value.identifier.length] = '\0';
	arg->next = NULL;

	_destroyToken(&token);
	return arg;
}

static struct _Statement* _parseStatement(struct _Lexer* const lexer)
{
	struct _Statement* statement = (struct _Statement*)malloc(sizeof(struct _Statement));
	struct _Token token = {0};
	_nextToken(lexer, &token);

	switch (token.type)
	{
		case TOKEN_LEFT_BRACE:
		{
			if (token.type != TOKEN_RIGHT_BRACE)
			{
				_parseStatement(lexer);
			}
			else
			{
				_expectToken(lexer, &token, TOKEN_RIGHT_BRACE, 1);
			}
		} break;

		case TOKEN_IF_KEYWORD:
		case TOKEN_WHILE_KEYWORD:
		{
			_expectToken(lexer, &token, TOKEN_LEFT_PARENTHESIS, 1);
			// Parse expression ...
			_expectToken(lexer, &token, TOKEN_RIGHT_PARENTHESIS, 1);
		} break;

		case TOKEN_RETURN_KEYWORD:
		{
			// Parse expression ...
			_expectToken(lexer, &token, TOKEN_SEMICOLON, 1);
		} break;

		case TOKEN_VAR_KEYWORD:
		{
			_expectToken(lexer, &token, TOKEN_VAR_KEYWORD, 1);
			_expectToken(lexer, &token, TOKEN_IDENTIFIER, 1);
			// Expect = token
			// Parse expression ...
			_expectToken(lexer, &token, TOKEN_SEMICOLON, 1);
		} break;

		default:
		{
			// Parse expression ...
			_expectToken(lexer, &token, TOKEN_SEMICOLON, 1);
		} break;
	}	

	_destroyToken(&token);
	return statement;
}

static struct _Procedure _parseProcedure(struct _Lexer* const lexer)
{
	struct _Procedure procedure = {0};

	struct _Token token = {0};

	// Parsing name
	enum _TokenType parsedType = _expectToken(lexer, &token, TOKEN_PROC_KEYWORD, 1);

	if (parsedType != TOKEN_PROC_KEYWORD)
	{
		_destroyToken(&token);
		fprintf(stderr, "ERROR: expected TOKEN_PROC_KEYWORD token, but parsed %s!\n", _stringifyTokenType(parsedType));
		exit(1);
	}

	_destroyToken(&token);
	parsedType = _expectToken(lexer, &token, TOKEN_IDENTIFIER, 1);

	if (parsedType != TOKEN_IDENTIFIER)
	{
		_destroyToken(&token);
		fprintf(stderr, "ERROR: expected TOKEN_IDENTIFIER token, but parsed %s!\n", _stringifyTokenType(parsedType));
		exit(1);
	}

	// Parsing args
	_destroyToken(&token);
	parsedType = _expectToken(lexer, &token, TOKEN_LEFT_PARENTHESIS, 1);

	if (parsedType != TOKEN_LEFT_PARENTHESIS)
	{
		_destroyToken(&token);
		fprintf(stderr, "ERROR: expected TOKEN_LEFT_PARENTHESIS token, but parsed %s!\n", _stringifyTokenType(parsedType));
		exit(1);
	}

	struct _ProcedureArg* argsBegin = NULL;
	struct _ProcedureArg* argsEnd = NULL;

	while (token.type != TOKEN_RIGHT_PARENTHESIS)
	{
		_peekToken(lexer, &token, 0);

		if (token.type == TOKEN_IDENTIFIER)
		{
			struct _ProcedureArg* argNode = _parseProcedureArg(lexer);

			if (argsEnd != NULL)
			{
				argsEnd->next = argNode;
				argsEnd = argNode;
			}
			else
			{
				assert(argsBegin == NULL);
				argsBegin = argsEnd = argNode;
			}

			_expectToken(lexer, &token, TOKEN_COMMA, 0);
		}

		_destroyToken(&token);
	}

	// Parsing body
	struct _Statement* statementsBegin = NULL;
	struct _Statement* statementsEnd = NULL;

	do
	{
		// _destroyToken(&token);
		// _nextToken(lexer, &token);
		struct _Statement* statementNode = _parseStatement(lexer);

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

	} while (token.type != TOKEN_RIGHT_BRACE);

	_destroyToken(&token);
	return procedure;
}

void _parse(const char* filePath)
{
#if 1
	struct _Lexer lexer = _createLexer(filePath);

	_parseProcedure(&lexer);

	_destroyLexer(&lexer);
#endif

#if 0
	struct _Lexer lexer = _createLexer(filePath);
	struct _Token token = {0};

	do
	{
		_nextToken(&lexer, &token);
		fprintf(stdout, "%s\n", _stringifyToken(&token));
		_destroyToken(&token);
	} while (token.type != TOKEN_END_OF_FILE);

	_destroyLexer(&lexer);
#endif
}
