#include "../include/lexer.h"

#include <stdlib.h>
#include <memory.h>
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

static void _skipInlineComment(
	struct _Lexer* const lexer)
{
	assert(lexer != NULL);
	assert(lexer->current != NULL);

	assert(_current(lexer) == '#');
	for (; _current(lexer) != '\n' || _current(lexer) != '\0';)
	{
		_moveBy(lexer, 1);
	}
}

static int _createNameToken(
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

	length = length >= TOKEN_STRING_VALUE_CAPACITY ? TOKEN_STRING_VALUE_CAPACITY : length;
	memcpy(token->value.s.buffer, lexer->current, length);
	token->value.s.buffer[length] = '\0';

	token->type = TOKEN_NAME;
	token->storage = STORAGE_STRING;
	token->location = lexer->location;
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

	length = length >= TOKEN_STRING_VALUE_CAPACITY ? TOKEN_STRING_VALUE_CAPACITY : length;
	char valueBuffer[TOKEN_STRING_VALUE_CAPACITY + 1];
	memcpy(valueBuffer, lexer->current, length);
	valueBuffer[length] = '\0';
	sscanf(valueBuffer, "%d", &token->value.i32);
	token->value.s.buffer[length] = '\0';

	token->type = TOKEN_LITERAL;
	token->storage = STORAGE_INT32;
	token->location = lexer->location;
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

	length = length >= TOKEN_STRING_VALUE_CAPACITY ? TOKEN_STRING_VALUE_CAPACITY : length;
	memcpy(token->value.s.buffer, lexer->current, length);
	token->value.s.buffer[length] = '\0';

	token->type = TOKEN_LITERAL;
	token->storage = STORAGE_STRING;
	token->location = lexer->location;
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

void _nextToken(
	struct _Lexer* const lexer,
	struct _Token* const token)
{
	assert(lexer != NULL);
	assert(lexer->current != NULL);
	assert(token != NULL);

	_skipWhitespaces(lexer);

	// 1. Check if token type is name
	if (_createNameToken(lexer, token) > 0)
	{
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
		case '#':
		{
			_skipInlineComment(lexer);
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
			_createTokenWithoutStorageAndMoveBy(lexer, token, TOKEN_ERROR, 1);
			fprintf(stderr, "ERROR: unknown symbol at location %s! Cannot tokenize!\n", _stringifyLocation(&lexer->location));
		} break;
	}

	// token->type = -1;
	// token->storage = -1;
	// token->value = ;
	// token->location = lexer->location;

	// if (_current(lexer) == '\0')
	// {
	// 	_createTokenWithoutStorageWithoutMovingCurrent(lexer, token, TOKEN_END_OF_FILE);
	// 		_moveBy(lexer, 1);
	// 	return;
	// }
}
