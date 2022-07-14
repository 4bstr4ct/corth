#include "../include/token.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

const char* _stringifyLocation(
	const struct _Location* const location)
{
	#define MAX_LENGTH 128
	static char buffer[MAX_LENGTH];
	const unsigned int length =
		snprintf(buffer, MAX_LENGTH, "file=%s, line=%d, column=%d", location->file, location->line, location->column);
	#undef MAX_LENGTH

	if (length > 0)
	{
		buffer[length] = '\0';
		return buffer;
	}

	return "UNSTRINGIFIED LOCATION";
}

static const char* const _stringifiedTokenTypes[] =
{
	[TOKEN_IDENTIFIER]			= "TOKEN_IDENTIFIER",
	[TOKEN_INT32_LITERAL]		= "TOKEN_INT32_LITERAL",
	[TOKEN_STRING_LITERAL]		= "TOKEN_STRING_LITERAL",
	[TOKEN_END_OF_FILE]			= "TOKEN_END_OF_FILE",
	[TOKEN_IF]					= "TOKEN_IF",
	[TOKEN_END]					= "TOKEN_END",
	[TOKEN_INCLUDE]				= "TOKEN_INCLUDE",
	[TOKEN_PROC]				= "TOKEN_PROC",
	[TOKEN_WHILE]				= "TOKEN_WHILE",
	[TOKEN_RETURN]				= "TOKEN_RETURN",
	[TOKEN_VAR]					= "TOKEN_VAR",
	[TOKEN_LEFT_PARENTHESIS]	= "TOKEN_LEFT_PARENTHESIS",
	[TOKEN_RIGHT_PARENTHESIS]	= "TOKEN_RIGHT_PARENTHESIS",
	[TOKEN_COMMA]				= "TOKEN_COMMA",
	[TOKEN_COLON]				= "TOKEN_COLON",
	[TOKEN_PLUS]				= "TOKEN_PLUS",
	[TOKEN_MINUS]				= "TOKEN_MINUS",
	[TOKEN_INVALID]				= "TOKEN_INVALID",
};

const char* _stringifyTokenType(
	const enum _TokenType type)
{
	assert(type >= 0 && type < TOKEN_TYPES_COUNT);
	return _stringifiedTokenTypes[type];
}

static const char* const _stringifiedTokenStorages[] =
{
	[STORAGE_NONE]				= "STORAGE_NONE",
	[STORAGE_INT32]				= "STORAGE_INT32",
	[STORAGE_STRING]			= "STORAGE_STRING",
};

const char* _stringifyTokenStorage(
	const enum _TokenStorage storage)
{
	assert(storage >= 0 && storage < TOKEN_STORAGES_COUNT);
	return _stringifiedTokenStorages[storage];
}

struct _Token _identifierToken(
	const char* const value,
	const unsigned int length,
	const struct _Location location)
{
	struct _Token token = {0};
	token.type = TOKEN_IDENTIFIER;
	token.storage = STORAGE_STRING;

	token.value.s.buffer = (char*)malloc((length + 1) * sizeof(char));
	memcpy(token.value.s.buffer, value, length);
	token.value.s.buffer[length] = '\0';
	token.value.s.length = length;

	token.location = location;
	return token;
}

struct _Token _int32LiteralToken(
	const char* const value,
	const unsigned int length,
	const struct _Location location)
{
	struct _Token token = {0};
	token.type = TOKEN_INT32_LITERAL;
	token.storage = STORAGE_INT32;

	char* valueBuffer = (char*)malloc((length + 1) * sizeof(char));
	memcpy(valueBuffer, value, length);
	valueBuffer[length] = '\0';
	sscanf(valueBuffer, "%d", &token.value.i32);
	free(valueBuffer);

	token.location = location;
	return token;
}

struct _Token _stringLiteralToken(
	const char* const value,
	const unsigned int length,
	const struct _Location location)
{
	struct _Token token = {0};
	token.type = TOKEN_STRING_LITERAL;
	token.storage = STORAGE_STRING;

	token.value.s.buffer = (char*)malloc((length + 1) * sizeof(char));
	memcpy(token.value.s.buffer, value, length);
	token.value.s.buffer[length] = '\0';
	token.value.s.length = length;

	token.location = location;
	return token;
}

void _destroyToken(
	struct _Token* token)
{
	assert(token != NULL);

	if (token->storage == STORAGE_STRING)
	{
		free(token->value.s.buffer);
		token->value.s.length = 0;
	}
}

const char* _stringifyToken(
	const struct _Token* const token)
{
	assert(token != NULL);

	#define VALUE_BUFFER_SIZE 128
	static char valueBuffer[VALUE_BUFFER_SIZE];

	switch (token->storage)
	{
		case STORAGE_NONE:
		{
			unsigned int length = snprintf(valueBuffer, VALUE_BUFFER_SIZE, "NO_VALUE");
			valueBuffer[length > VALUE_BUFFER_SIZE ? VALUE_BUFFER_SIZE : length] = '\0';
		} break;

		case STORAGE_INT32:
		{
			unsigned int length = snprintf(valueBuffer, VALUE_BUFFER_SIZE, "%d", token->value.i32);
			valueBuffer[length > VALUE_BUFFER_SIZE ? VALUE_BUFFER_SIZE : length] = '\0';
		} break;

		case STORAGE_STRING:
		{
			unsigned int length = snprintf(valueBuffer, VALUE_BUFFER_SIZE, "%s", token->value.s.buffer);
			valueBuffer[length > VALUE_BUFFER_SIZE ? VALUE_BUFFER_SIZE : length] = '\0';
		} break;

		default:
		{
			unsigned int length = snprintf(valueBuffer, VALUE_BUFFER_SIZE, "UNKNOWN_STORAGE!");
			valueBuffer[length > VALUE_BUFFER_SIZE ? VALUE_BUFFER_SIZE : length] = '\0';
		} break;
	}

	#undef VALUE_BUFFER_SIZE

	#define BUFFER_SIZE 256
	static char buffer[BUFFER_SIZE];

	unsigned int length = snprintf(buffer, BUFFER_SIZE, "<type=%s, storage=%s, value=%s, %s>",
		_stringifyTokenType(token->type), _stringifyTokenStorage(token->storage), valueBuffer, _stringifyLocation(&token->location));
	buffer[length > BUFFER_SIZE ? BUFFER_SIZE : length] = '\0';

	#undef BUFFER_SIZE
	return buffer;
}
