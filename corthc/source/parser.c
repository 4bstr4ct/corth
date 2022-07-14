#include "../include/parser.h"
#include "../include/lexer.h"
#include "../include/token.h"

#include <stdlib.h>
#include <stdio.h>

struct _Statement
{
	// TODO: remove!
	int temp;
};

// TODO: rework for heap and dynamic allocation!
#define PROCEDURE_ARGS_CAPACITY 64
#define PROCEDURE_STATEMENTS_CAPACITY 512

struct _Procedure
{
	struct _Token name;

	struct _Token args[PROCEDURE_ARGS_CAPACITY];
	unsigned int argsCount;

	struct _Statement statements[PROCEDURE_ARGS_CAPACITY];
	unsigned int statementsCount;
};

struct _Procedure _buildProcedure(struct _Lexer* const lexer)
{
	struct _Procedure procedure = {0};

	struct _Token token = {0};
	_nextToken(lexer, &token);

	if (token.type != TOKEN_IDENTIFIER)
	{
		fprintf(stderr, "ERROR: procedure must have a name as an identifier after keyword \'proc\'!\n");
		_destroyToken(&token);
		exit(1);
	}

	procedure.name = token;
	// Not destroying token, because procedure stores it.
	// _destroyToken(&token);
	_nextToken(lexer, &token);

	while (token.type != TOKEN_COLON)
	{
		if (token.type != TOKEN_IDENTIFIER)
		{
			fprintf(stderr, "ERROR: procedure's argument must be an identifier token!\n");
			_destroyToken(&token);
			exit(1);
		}

		procedure.args[procedure.argsCount++] = token;
		// Not destroying the token, because procedure stores it.
		// _destroyToken(&token);
		_nextToken(lexer, &token);
	}

	if (token.type != TOKEN_COLON)
	{
		fprintf(stderr, "ERROR: after procedure's args colon token must follow!\n");
		_destroyToken(&token);
		exit(1);
	}

	_destroyToken(&token);
	_nextToken(lexer, &token);

	while (token.type != TOKEN_END)
	{
		procedure.statements[procedure.statementsCount++] = (struct _Statement){ .temp = procedure.statementsCount };
		_destroyToken(&token);
		_nextToken(lexer, &token);
	}

	if (token.type != TOKEN_END)
	{
		fprintf(stderr, "ERROR: after procedure's body end token follow!\n");
		_destroyToken(&token);
		exit(1);
	}

	_destroyToken(&token);
	return procedure;
}

/*
struct _Statement _buildStatement(struct _Lexer* const lexer)
{
	struct _Statement statement = {0};
	return statement;
}
*/

static void _printProcedure(const struct _Procedure* const procedure)
{
	fprintf(stdout, "proc %s", procedure->name.value.s.buffer);

	for (unsigned int index = 0; index < procedure->argsCount; ++index)
	{
		fprintf(stdout, " %s", procedure->args[index].value.s.buffer);
	}

	fprintf(stdout, ":\n");

	for (unsigned int index = 0; index < procedure->statementsCount; ++index)
	{
		fprintf(stdout, "    // statement ...\n");
	}

	fprintf(stdout, "end\n\n");
}

void _parse(const char* filePath)
{
	struct _Lexer lexer = _createLexer(filePath);
	struct _Token token = {0};

	do
	{
		_nextToken(&lexer, &token);
		fprintf(stdout, "Working on a token:\n\t%s\n\n", _stringifyToken(&token));

		switch (token.type)
		{
			case TOKEN_PROC:
			{
				struct _Procedure procedure = _buildProcedure(&lexer);
				_printProcedure(&procedure);
			} break;

			case TOKEN_VAR:
			{
				// _buildStatement(lexer);
			} break;
		}

		_destroyToken(&token);
	} while (token.type != TOKEN_END_OF_FILE);

	/*
	struct _Lexer lexer = _createLexer(filePath);
	struct _Token current = {0};

	struct _Token tokens[128];
	unsigned int tokensCount = 0;

	do
	{
		_nextToken(&lexer, &current);
		fprintf(stdout, "%s\n", _stringifyToken(&current));
		tokens[tokensCount++] = current;
	} while (current.type != TOKEN_END_OF_FILE);
	*/
}
