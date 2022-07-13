#include "../include/defs.h"

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
	[TOKEN_INCLUDE]			= "TOKEN_INCLUDE",
	[TOKEN_PROCEDURE]		= "TOKEN_PROCEDURE",
	[TOKEN_END]				= "TOKEN_END",
	[TOKEN_DOUBLE_QUOTE]	= "TOKEN_DOUBLE_QUOTE",
	[TOKEN_SEMICOLON]		= "TOKEN_SEMICOLON",
	[TOKEN_COLON]			= "TOKEN_COLON",
	[TOKEN_LITERAL]			= "TOKEN_LITERAL",
	[TOKEN_NAME]			= "TOKEN_NAME",
	[TOKEN_END_OF_FILE]		= "TOKEN_END_OF_FILE",
	[TOKEN_ERROR]			= "TOKEN_ERROR"
};

const char* _stringifyTokenType(
	const enum _TokenType type)
{
	assert(type >= 0 && type < TOKEN_TYPES_COUNT);
	return _stringifiedTokenTypes[type];
}

static const char* const _stringifiedTokenStorages[] =
{
	[STORAGE_NONE]			= "STORAGE_NONE",
	[STORAGE_INT32]			= "STORAGE_INT32",
	[STORAGE_STRING]		= "STORAGE_STRING",
};

const char* _stringifyTokenStorage(
	const enum _TokenStorage storage)
{
	assert(storage >= 0 && storage < TOKEN_STORAGES_COUNT);
	return _stringifiedTokenStorages[storage];
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
