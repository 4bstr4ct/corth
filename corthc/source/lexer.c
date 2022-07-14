#include "../include/lexer.h"
#include "../include/lexer.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <stdio.h>

struct _Lexer _createLexer(
	const char* filePath)
{
	struct _Lexer lexer;
	lexer.location.file = filePath;
	lexer.location.line = 1;
	lexer.location.column = 1;
	lexer.length = 0;

	FILE* file = fopen(filePath, "r");

	if (!file)
	{
		fprintf(stderr, "ERROR: failed to open source file `%s`!", filePath);
		exit(1);
	}

	fseek(file, 0, SEEK_END);
	unsigned int fileSize = ftell(file);
	fileSize = fileSize >= LEXER_BUFFER_CAPACITY ? LEXER_BUFFER_CAPACITY : fileSize;
	fseek(file, 0, SEEK_SET);
	fread(lexer.buffer, fileSize, 1, file);
	fclose(file);

	lexer.buffer[fileSize] = '\0';
	lexer.length = fileSize;
	lexer.current = lexer.buffer;
	return lexer;
}

static char _current(
	struct _Lexer* const lexer)
{
	assert(lexer != NULL);
	assert(lexer->current != NULL);
	return *(lexer->current);
}

static int _isWhitespace(
	struct _Lexer* const lexer)
{
	assert(lexer != NULL);
	assert(lexer->current != NULL);
	return _current(lexer) == 13
		|| _current(lexer) == 10
		|| _current(lexer) == ' '
		|| _current(lexer) == '\t';
}

static void _moveBy(
	struct _Lexer* const lexer,
	const unsigned int offset)
{
	assert(lexer != NULL);
	assert(lexer->current != NULL);

	for (unsigned int counter = 0; counter < offset; ++counter)
	{
		++lexer->location.column;

		if (_current(lexer) == '\n')
		{
			++lexer->location.line;
			lexer->location.column = 1;
		}

		++lexer->current;
	}
}

static void _skipWhitespaces(
	struct _Lexer* const lexer)
{
	assert(lexer != NULL);
	assert(lexer->current != NULL);

	while (_isWhitespace(lexer))
	{
		_moveBy(lexer, 1);
	}
}

static char _peek(
	struct _Lexer* const lexer,
	const unsigned int offset)
{
	assert(lexer != NULL);
	assert(lexer->current != NULL);
	return *(lexer->current + offset);
}

static void _skipSingleLineComment(
	struct _Lexer* const lexer)
{
	assert(lexer != NULL);
	assert(lexer->current != NULL);

	if (_current(lexer) == '/' && _peek(lexer, 1) == '/')
	{
		for (; _current(lexer) != '\n' && _current(lexer) != '\0';)
		{
			_moveBy(lexer, 1);
		}

		if (_current(lexer) == '\n')
		{
			_moveBy(lexer, 1);
		}
	}
}

static int _createIdentifierToken(
	struct _Lexer* const lexer,
	struct _Token* const token)
{
	assert(lexer != NULL);
	assert(lexer->current != NULL);
	assert(token != NULL);

	if (!isalpha(_current(lexer)))
	{
		return 0;
	}

	unsigned int length = 0;
	for (; isalpha(_peek(lexer, length)); ++length);

	*token = _identifierToken(lexer->current, length, lexer->location);
	_moveBy(lexer, length);
	return length;
}

static int _createIntLiteralToken(
	struct _Lexer* const lexer,
	struct _Token* const token)
{
	assert(lexer != NULL);
	assert(lexer->current != NULL);
	assert(token != NULL);

	if (!isdigit(_current(lexer)))
	{
		return 0;
	}

	unsigned int length = 0;
	for (; isdigit(_peek(lexer, length)); ++length);

	*token = _int32LiteralToken(lexer->current, length, lexer->location);
	_moveBy(lexer, length);
	return length;
}

static int _createStringLiteralToken(
	struct _Lexer* const lexer,
	struct _Token* const token)
{
	assert(lexer != NULL);
	assert(lexer->current != NULL);
	assert(token != NULL);

	assert(_current(lexer) == '\"');
	_moveBy(lexer, 1);

	unsigned int length = 0;
	for (; _peek(lexer, length) != '\"'; ++length);

	// length = length >= TOKEN_STRING_VALUE_CAPACITY ? TOKEN_STRING_VALUE_CAPACITY : length;
	// memcpy(token->value.s.buffer, lexer->current, length);
	// token->value.s.buffer[length] = '\0';

	*token = _identifierToken(lexer->current, length, lexer->location);
	_moveBy(lexer, length + 1);
	return length + 1;
}

static void _createTokenWithoutStorageAndMoveBy(
	struct _Lexer* const lexer,
	struct _Token* const token,
	const enum _TokenType type,
	const unsigned int offset)
{
	assert(lexer != NULL);
	assert(token != NULL);

	token->type = type;
	token->storage = STORAGE_NONE;
	token->location = lexer->location;
	_moveBy(lexer, offset);
}

static void _tryToTurnIdentifierTokenToKeywordToken(
	struct _Token* const token)
{
	assert(token != NULL);
	assert(token->type == TOKEN_IDENTIFIER);
	assert(token->storage == STORAGE_STRING);

	#define KEYWORDS_COUNT 7
	static const char* keywords[KEYWORDS_COUNT] =
	{ "if", "end", "include", "proc", "while", "return", "var" };
	static enum _TokenType types[KEYWORDS_COUNT] =
	{ TOKEN_IF, TOKEN_END, TOKEN_INCLUDE, TOKEN_PROC, TOKEN_WHILE, TOKEN_RETURN, TOKEN_VAR };
	
	for (unsigned int index = 0; index < KEYWORDS_COUNT; ++index)
	{
		if (strcmp(token->value.s.buffer, keywords[index]) == 0)
		{
			unsigned int length = strlen(keywords[index]);

			if (token->value.s.length <= length)
			{
				free(token->value.s.buffer);
				token->value.s.buffer = (char*)malloc((length + 1) * sizeof(char));
			}

			memcpy(token->value.s.buffer, keywords[index], length);
			token->value.s.buffer[length] ='\0';
			token->value.s.length = length;
			token->type = types[index];
			break;
		}
	}

	#undef KEYWORDS_COUNT
}

void _nextToken(
	struct _Lexer* const lexer,
	struct _Token* const token)
{
start:
	assert(lexer != NULL);
	assert(lexer->current != NULL);
	assert(token != NULL);

	_skipWhitespaces(lexer);

	// 1. Check if token type is name
	if (_createIdentifierToken(lexer, token) > 0)
	{
		_tryToTurnIdentifierTokenToKeywordToken(token);
		return;
	}

	// 2. Check if token type is literal
	if (_createIntLiteralToken(lexer, token) > 0)
	{
		return;
	}

	// 3. Handle symbols such as semicolon..
	switch (_current(lexer))
	{
		case '(':
		{
			_createTokenWithoutStorageAndMoveBy(lexer, token, TOKEN_LEFT_PARENTHESIS, 1);
		} break;

		case ')':
		{
			_createTokenWithoutStorageAndMoveBy(lexer, token, TOKEN_RIGHT_PARENTHESIS, 1);
		} break;

		case ',':
		{
			_createTokenWithoutStorageAndMoveBy(lexer, token, TOKEN_COMMA, 1);
		} break;

		case ':':
		{
			_createTokenWithoutStorageAndMoveBy(lexer, token, TOKEN_COLON, 1);
		} break;

		case '+':
		{
			_createTokenWithoutStorageAndMoveBy(lexer, token, TOKEN_PLUS, 1);
		} break;

		case '-':
		{
			_createTokenWithoutStorageAndMoveBy(lexer, token, TOKEN_MINUS, 1);
		} break;

		case '/':
		{
			switch (_peek(lexer, 1))
			{
				case '/':
				{
					_skipSingleLineComment(lexer);
					goto start;
				} break;

				default:
				{
					_createTokenWithoutStorageAndMoveBy(lexer, token, TOKEN_INVALID, 2);
				} break;
			}
		} break;

		case '\"':
		{
			_createStringLiteralToken(lexer, token);
		} break;

		case '\0':
		{
			_createTokenWithoutStorageAndMoveBy(lexer, token, TOKEN_END_OF_FILE, 1);
		} break;

		default:
		{
			_createTokenWithoutStorageAndMoveBy(lexer, token, TOKEN_INVALID, 1);
			fprintf(stderr, "ERROR: unknown symbol at location %s! Cannot tokenize!\n", _stringifyLocation(&lexer->location));
		} break;
	}
}
