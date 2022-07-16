#include "../include/token.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

static_assert(TOKEN_TYPES_COUNT == 31, "TOKEN_TYPES_COUNT is higher than accounted in _stringifiedTokenTypes!");
static const char* const _stringifiedTokenTypes[TOKEN_TYPES_COUNT] =
{
	[TOKEN_IDENTIFIER]			= "TOKEN_IDENTIFIER",

	[TOKEN_IF_KEYWORD]			= "TOKEN_IF_KEYWORD",
	[TOKEN_ELSE_KEYWORD]		= "TOKEN_ELSE_KEYWORD",
	[TOKEN_IMPORT_KEYWORD]		= "TOKEN_IMPORT_KEYWORD",
	[TOKEN_EXPORT_KEYWORD]		= "TOKEN_EXPORT_KEYWORD",
	[TOKEN_PROC_KEYWORD]		= "TOKEN_PROC_KEYWORD",
	[TOKEN_WHILE_KEYWORD]		= "TOKEN_WHILE_KEYWORD",
	[TOKEN_RETURN_KEYWORD]		= "TOKEN_RETURN_KEYWORD",

	[TOKEN_CHAR_LITERAL]		= "TOKEN_CHAR_LITERAL",
	[TOKEN_INT8_LITERAL]		= "TOKEN_INT8_LITERAL",
	[TOKEN_UINT8_LITERAL]		= "TOKEN_UINT8_LITERAL",
	[TOKEN_INT16_LITERAL]		= "TOKEN_INT16_LITERAL",
	[TOKEN_UINT16_LITERAL]		= "TOKEN_UINT16_LITERAL",
	[TOKEN_INT32_LITERAL]		= "TOKEN_INT32_LITERAL",
	[TOKEN_UINT32_LITERAL]		= "TOKEN_UINT32_LITERAL",
	[TOKEN_INT64_LITERAL]		= "TOKEN_INT64_LITERAL",
	[TOKEN_UINT64_LITERAL]		= "TOKEN_UINT64_LITERAL",
	[TOKEN_FLOAT32_LITERAL]		= "TOKEN_FLOAT32_LITERAL",
	[TOKEN_FLOAT64_LITERAL]		= "TOKEN_FLOAT64_LITERAL",
	[TOKEN_STRING_LITERAL]		= "TOKEN_STRING_LITERAL",

	[TOKEN_LEFT_PARENTHESIS]	= "TOKEN_LEFT_PARENTHESIS",
	[TOKEN_RIGHT_PARENTHESIS]	= "TOKEN_RIGHT_PARENTHESIS",
	[TOKEN_LEFT_BRACKET]		= "TOKEN_LEFT_BRACKET",
	[TOKEN_RIGHT_BRACKET]		= "TOKEN_RIGHT_BRACKET",
	[TOKEN_LEFT_BRACE]			= "TOKEN_LEFT_BRACE",
	[TOKEN_RIGHT_BRACE]			= "TOKEN_RIGHT_BRACE",
	[TOKEN_SEMICOLON]			= "TOKEN_SEMICOLON",
	[TOKEN_COLON]				= "TOKEN_COLON",
	[TOKEN_COMMA]				= "TOKEN_COMMA",

	[TOKEN_END_OF_FILE]			= "TOKEN_END_OF_FILE",
	[TOKEN_INVALID]				= "TOKEN_INVALID"
};
static_assert(sizeof(_stringifiedTokenTypes) / sizeof(const char* const), "Size of _stringifiedTokenTypes is not equal to TOKEN_TYPES_COUNT!");

const char* _stringifyTokenType(
	const enum _TokenType type)
{
	assert(type >= 0 && type < TOKEN_TYPES_COUNT);
	return _stringifiedTokenTypes[type];
}

static_assert(TOKEN_STORAGES_COUNT == 15, "TOKEN_STORAGES_COUNT is higher than accounted in _stringifiedTokenStorages!");
static const char* const _stringifiedTokenStorages[TOKEN_STORAGES_COUNT] =
{
	[STORAGE_NONE]				= "STORAGE_NONE",

	[STORAGE_IDENTIFIER]		= "STORAGE_IDENTIFIER",
	[STORAGE_KEYWORD]			= "STORAGE_KEYWORD",

	[STORAGE_CHAR]				= "STORAGE_CHAR",
	[STORAGE_INT8]				= "STORAGE_INT8",
	[STORAGE_UINT8]				= "STORAGE_UINT8",
	[STORAGE_INT16]				= "STORAGE_INT16",
	[STORAGE_UINT16]			= "STORAGE_UINT16",
	[STORAGE_INT32]				= "STORAGE_INT32",
	[STORAGE_UINT32]			= "STORAGE_UINT32",
	[STORAGE_INT64]				= "STORAGE_INT64",
	[STORAGE_UINT64]			= "STORAGE_UINT64",
	[STORAGE_FLOAT32]			= "STORAGE_FLOAT32",
	[STORAGE_FLOAT64]			= "STORAGE_FLOAT64",
	[STORAGE_STRING]			= "STORAGE_STRING"
};
static_assert(sizeof(_stringifiedTokenStorages) / sizeof(const char* const), "Size of _stringifiedTokenStorages is not equal to TOKEN_STORAGES_COUNT!");

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
	struct _Token token = _metaToken(
		TOKEN_IDENTIFIER,
		STORAGE_IDENTIFIER,
		location
	);

	assert(value != NULL);
	token.value.identifier.buffer = (char*)malloc((length + 1) * sizeof(char));
	memcpy(token.value.identifier.buffer, value, length);
	token.value.identifier.buffer[length] = '\0';
	token.value.identifier.length = length;
	return token;
}

struct _Token _keywordToken(
	const char* const value,
	const unsigned int length,
	const enum _TokenType type,
	const struct _Location location)
{
	struct _Token token = _metaToken(
		type,
		STORAGE_KEYWORD,
		location
	);

	assert(value != NULL);
	token.value.keyword.buffer = (char*)malloc((length + 1) * sizeof(char));
	memcpy(token.value.keyword.buffer, value, length);
	token.value.keyword.buffer[length] = '\0';
	token.value.keyword.length = length;
	return token;
}

struct _Token _metaToken(
	const enum _TokenType type,
	const enum _TokenStorage storage,
	const struct _Location location)
{
	struct _Token token = {0};
	token.type = type;
	token.storage = storage;
	token.location = location;
	return token;
}

struct _Token _charLiteralToken(
	const char* const value,
	const unsigned int length,
	const struct _Location location)
{
	struct _Token token = _metaToken(
		TOKEN_CHAR_LITERAL,
		STORAGE_CHAR,
		location
	);

	assert(value != NULL);
	assert(length == 1);
	token.value.literal.c = *value;
	return token;
}

struct _Token _int8LiteralToken(
	const char* const value,
	const unsigned int length,
	const struct _Location location)
{
	struct _Token token = _metaToken(
		TOKEN_INT8_LITERAL,
		STORAGE_INT8,
		location
	);

	assert(value != NULL);
	assert(length < 256); // Last char is for \0!
	char buffer[256];
	memcpy(buffer, value, length);
	buffer[length] = '\0';
	signed int temp = 0;
	sscanf(buffer, "%d", &temp);
	token.value.literal.i8 = (signed char)temp;
	return token;
}

struct _Token _uint8LiteralToken(
	const char* const value,
	const unsigned int length,
	const struct _Location location)
{
	struct _Token token = _metaToken(
		TOKEN_UINT8_LITERAL,
		STORAGE_UINT8,
		location
	);

	assert(value != NULL);
	assert(length < 256); // Last char is for \0!
	char buffer[256];
	memcpy(buffer, value, length);
	buffer[length] = '\0';
	unsigned int temp = 0;
	sscanf(buffer, "%u", &temp);
	token.value.literal.u8 = (unsigned char)temp;
	return token;
}

struct _Token _int16LiteralToken(
	const char* const value,
	const unsigned int length,
	const struct _Location location)
{
	struct _Token token = _metaToken(
		TOKEN_INT16_LITERAL,
		STORAGE_INT16,
		location
	);

	assert(value != NULL);
	assert(length < 256); // Last char is for \0!
	char buffer[256];
	memcpy(buffer, value, length);
	buffer[length] = '\0';
	sscanf(buffer, "%hd", &token.value.literal.i16);
	return token;
}

struct _Token _uint16LiteralToken(
	const char* const value,
	const unsigned int length,
	const struct _Location location)
{
	struct _Token token = _metaToken(
		TOKEN_UINT16_LITERAL,
		STORAGE_UINT16,
		location
	);

	assert(value != NULL);
	assert(length < 256); // Last char is for \0!
	char buffer[256];
	memcpy(buffer, value, length);
	buffer[length] = '\0';
	sscanf(buffer, "%hu", &token.value.literal.u16);
	return token;
}

struct _Token _int32LiteralToken(
	const char* const value,
	const unsigned int length,
	const struct _Location location)
{
	struct _Token token = _metaToken(
		TOKEN_INT32_LITERAL,
		STORAGE_INT32,
		location
	);

	assert(value != NULL);
	assert(length < 256); // Last char is for \0!
	char buffer[256];
	memcpy(buffer, value, length);
	buffer[length] = '\0';
	sscanf(buffer, "%d", &token.value.literal.i32);
	return token;
}

struct _Token _uint32LiteralToken(
	const char* const value,
	const unsigned int length,
	const struct _Location location)
{
	struct _Token token = _metaToken(
		TOKEN_UINT32_LITERAL,
		STORAGE_UINT32,
		location
	);

	assert(value != NULL);
	assert(length < 256); // Last char is for \0!
	char buffer[256];
	memcpy(buffer, value, length);
	buffer[length] = '\0';
	sscanf(buffer, "%u", &token.value.literal.u32);
	return token;
}

struct _Token _int64LiteralToken(
	const char* const value,
	const unsigned int length,
	const struct _Location location)
{
	struct _Token token = _metaToken(
		TOKEN_INT64_LITERAL,
		STORAGE_INT64,
		location
	);

	assert(value != NULL);
	assert(length < 256); // Last char is for \0!
	char buffer[256];
	memcpy(buffer, value, length);
	buffer[length] = '\0';
	sscanf(buffer, "%lld", &token.value.literal.i64);
	return token;
}

struct _Token _uint64LiteralToken(
	const char* const value,
	const unsigned int length,
	const struct _Location location)
{
	struct _Token token = _metaToken(
		TOKEN_UINT64_LITERAL,
		STORAGE_UINT64,
		location
	);

	assert(value != NULL);
	assert(length < 256); // Last char is for \0!
	char buffer[256];
	memcpy(buffer, value, length);
	buffer[length] = '\0';
	sscanf(buffer, "%llu", &token.value.literal.u64);
	return token;
}

struct _Token _float32LiteralToken(
	const char* const value,
	const unsigned int length,
	const struct _Location location)
{
	struct _Token token = _metaToken(
		TOKEN_FLOAT32_LITERAL,
		STORAGE_FLOAT32,
		location
	);

	assert(value != NULL);
	assert(length < 256); // Last char is for \0!
	char buffer[256];
	memcpy(buffer, value, length);
	buffer[length] = '\0';
	sscanf(buffer, "%f", &token.value.literal.f32);
	return token;
}

struct _Token _float64LiteralToken(
	const char* const value,
	const unsigned int length,
	const struct _Location location)
{
	struct _Token token = _metaToken(
		TOKEN_FLOAT64_LITERAL,
		STORAGE_FLOAT64,
		location
	);

	assert(value != NULL);
	assert(length < 256); // Last char is for \0!
	char buffer[256];
	memcpy(buffer, value, length);
	buffer[length] = '\0';
	sscanf(buffer, "%lf", &token.value.literal.f64);
	return token;
}

struct _Token _stringLiteralToken(
	const char* const value,
	const unsigned int length,
	const struct _Location location)
{
	struct _Token token = _metaToken(
		TOKEN_STRING_LITERAL,
		STORAGE_STRING,
		location
	);

	assert(value != NULL);
	token.value.literal.string.buffer = (char*)malloc((length + 1) * sizeof(char));
	memcpy(token.value.literal.string.buffer, value, length);
	token.value.literal.string.buffer[length] = '\0';
	token.value.literal.string.length = length;
	return token;
}

struct _Token _invalidToken(
	const struct _Location location)
{
	return _metaToken(
		TOKEN_INVALID,
		STORAGE_NONE,
		location
	);	
}

void _destroyToken(
	struct _Token* token)
{
	assert(token != NULL);

	switch (token->storage)
	{
		case STORAGE_IDENTIFIER:
		{
			free(token->value.identifier.buffer);
			token->value.identifier.length = 0;
		} break;

		case STORAGE_KEYWORD:
		{
			free(token->value.keyword.buffer);
			token->value.keyword.length = 0;
		} break;

		case STORAGE_STRING:
		{
			free(token->value.literal.string.buffer);
			token->value.literal.string.length = 0;
		} break;

		default:
		{
		} break;
	}
}

const char* _stringifyToken(
	const struct _Token* const token)
{
	assert(token != NULL);

	#define VALUE_BUFFER_SIZE 128
	static char valueBuffer[VALUE_BUFFER_SIZE];

	static_assert(TOKEN_STORAGES_COUNT == 15, "TOKEN_STORAGES_COUNT is higher than accounted in _stringifyToken!");
	switch (token->storage)
	{
		case STORAGE_NONE:
		{
			unsigned int length = snprintf(valueBuffer, VALUE_BUFFER_SIZE, "NO_VALUE");
			valueBuffer[length > VALUE_BUFFER_SIZE ? VALUE_BUFFER_SIZE : length] = '\0';
		} break;

		case STORAGE_IDENTIFIER:
		{
			unsigned int length = snprintf(valueBuffer, VALUE_BUFFER_SIZE, "%s", token->value.identifier.buffer);
			valueBuffer[length > VALUE_BUFFER_SIZE ? VALUE_BUFFER_SIZE : length] = '\0';
		} break;

		case STORAGE_KEYWORD:
		{
			unsigned int length = snprintf(valueBuffer, VALUE_BUFFER_SIZE, "%s", token->value.keyword.buffer);
			valueBuffer[length > VALUE_BUFFER_SIZE ? VALUE_BUFFER_SIZE : length] = '\0';
		} break;

		case STORAGE_CHAR:
		{
			unsigned int length = snprintf(valueBuffer, VALUE_BUFFER_SIZE, "%c", token->value.literal.c);
			valueBuffer[length > VALUE_BUFFER_SIZE ? VALUE_BUFFER_SIZE : length] = '\0';
		} break;

		case STORAGE_INT8:
		{
			unsigned int length = snprintf(valueBuffer, VALUE_BUFFER_SIZE, "%d", token->value.literal.i8);
			valueBuffer[length > VALUE_BUFFER_SIZE ? VALUE_BUFFER_SIZE : length] = '\0';
		} break;

		case STORAGE_UINT8:
		{
			unsigned int length = snprintf(valueBuffer, VALUE_BUFFER_SIZE, "%u", token->value.literal.u8);
			valueBuffer[length > VALUE_BUFFER_SIZE ? VALUE_BUFFER_SIZE : length] = '\0';
		} break;

		case STORAGE_INT16:
		{
			unsigned int length = snprintf(valueBuffer, VALUE_BUFFER_SIZE, "%hd", token->value.literal.i16);
			valueBuffer[length > VALUE_BUFFER_SIZE ? VALUE_BUFFER_SIZE : length] = '\0';
		} break;

		case STORAGE_UINT16:
		{
			unsigned int length = snprintf(valueBuffer, VALUE_BUFFER_SIZE, "%hu", token->value.literal.u16);
			valueBuffer[length > VALUE_BUFFER_SIZE ? VALUE_BUFFER_SIZE : length] = '\0';
		} break;

		case STORAGE_INT32:
		{
			unsigned int length = snprintf(valueBuffer, VALUE_BUFFER_SIZE, "%d", token->value.literal.i32);
			valueBuffer[length > VALUE_BUFFER_SIZE ? VALUE_BUFFER_SIZE : length] = '\0';
		} break;

		case STORAGE_UINT32:
		{
			unsigned int length = snprintf(valueBuffer, VALUE_BUFFER_SIZE, "%u", token->value.literal.u32);
			valueBuffer[length > VALUE_BUFFER_SIZE ? VALUE_BUFFER_SIZE : length] = '\0';
		} break;

		case STORAGE_INT64:
		{
			unsigned int length = snprintf(valueBuffer, VALUE_BUFFER_SIZE, "%lld", token->value.literal.i64);
			valueBuffer[length > VALUE_BUFFER_SIZE ? VALUE_BUFFER_SIZE : length] = '\0';
		} break;

		case STORAGE_UINT64:
		{
			unsigned int length = snprintf(valueBuffer, VALUE_BUFFER_SIZE, "%llu", token->value.literal.u64);
			valueBuffer[length > VALUE_BUFFER_SIZE ? VALUE_BUFFER_SIZE : length] = '\0';
		} break;

		case STORAGE_FLOAT32:
		{
			unsigned int length = snprintf(valueBuffer, VALUE_BUFFER_SIZE, "%f", token->value.literal.f32);
			valueBuffer[length > VALUE_BUFFER_SIZE ? VALUE_BUFFER_SIZE : length] = '\0';
		} break;

		case STORAGE_FLOAT64:
		{
			unsigned int length = snprintf(valueBuffer, VALUE_BUFFER_SIZE, "%lf", token->value.literal.f64);
			valueBuffer[length > VALUE_BUFFER_SIZE ? VALUE_BUFFER_SIZE : length] = '\0';
		} break;

		case STORAGE_STRING:
		{
			unsigned int length = snprintf(valueBuffer, VALUE_BUFFER_SIZE, "%s", token->value.literal.string.buffer);
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
		_stringifyTokenType(token->type),
		_stringifyTokenStorage(token->storage), valueBuffer,
		_stringifyLocation(&token->location));
	buffer[length > BUFFER_SIZE ? BUFFER_SIZE : length] = '\0';

	#undef BUFFER_SIZE
	return buffer;
}
