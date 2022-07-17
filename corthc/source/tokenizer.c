#include "../include/tokenizer.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <stdio.h>

static_assert(TOKEN_TYPES_COUNT == 65, "Outdated TOKEN_TYPES_COUNT value!");

static const char* const _stringifiedTokenTypes[TOKEN_TYPES_COUNT] =
{
	STRINGIFY_ENUM(TOKEN_IDENTIFIER),

	STRINGIFY_ENUM(TOKEN_IF_KEYWORD),
	STRINGIFY_ENUM(TOKEN_IMPORT_KEYWORD),
	STRINGIFY_ENUM(TOKEN_EXPORT_KEYWORD),
	STRINGIFY_ENUM(TOKEN_PROC_KEYWORD),
	STRINGIFY_ENUM(TOKEN_WHILE_KEYWORD),
	STRINGIFY_ENUM(TOKEN_RETURN_KEYWORD),
	STRINGIFY_ENUM(TOKEN_VAR_KEYWORD),

	STRINGIFY_ENUM(TOKEN_CHAR_KEYWORD),
	STRINGIFY_ENUM(TOKEN_INT8_KEYWORD),
	STRINGIFY_ENUM(TOKEN_UINT8_KEYWORD),
	STRINGIFY_ENUM(TOKEN_INT16_KEYWORD),
	STRINGIFY_ENUM(TOKEN_UINT16_KEYWORD),
	STRINGIFY_ENUM(TOKEN_INT32_KEYWORD),
	STRINGIFY_ENUM(TOKEN_UINT32_KEYWORD),
	STRINGIFY_ENUM(TOKEN_INT64_KEYWORD),
	STRINGIFY_ENUM(TOKEN_UINT64_KEYWORD),
	STRINGIFY_ENUM(TOKEN_FLOAT32_KEYWORD),
	STRINGIFY_ENUM(TOKEN_FLOAT64_KEYWORD),

	STRINGIFY_ENUM(TOKEN_CHAR_LITERAL),
	STRINGIFY_ENUM(TOKEN_INT8_LITERAL),
	STRINGIFY_ENUM(TOKEN_UINT8_LITERAL),
	STRINGIFY_ENUM(TOKEN_INT16_LITERAL),
	STRINGIFY_ENUM(TOKEN_UINT16_LITERAL),
	STRINGIFY_ENUM(TOKEN_INT32_LITERAL),
	STRINGIFY_ENUM(TOKEN_UINT32_LITERAL),
	STRINGIFY_ENUM(TOKEN_INT64_LITERAL),
	STRINGIFY_ENUM(TOKEN_UINT64_LITERAL),
	STRINGIFY_ENUM(TOKEN_FLOAT32_LITERAL),
	STRINGIFY_ENUM(TOKEN_FLOAT64_LITERAL),
	STRINGIFY_ENUM(TOKEN_STRING_LITERAL),

	STRINGIFY_ENUM(TOKEN_LEFT_PARENTHESIS),
	STRINGIFY_ENUM(TOKEN_RIGHT_PARENTHESIS),
	STRINGIFY_ENUM(TOKEN_LEFT_BRACE),
	STRINGIFY_ENUM(TOKEN_RIGHT_BRACE),
	STRINGIFY_ENUM(TOKEN_LEFT_BRACKET),
	STRINGIFY_ENUM(TOKEN_RIGHT_BRACKET),
	STRINGIFY_ENUM(TOKEN_SEMICOLON),
	STRINGIFY_ENUM(TOKEN_COLON),
	STRINGIFY_ENUM(TOKEN_COMMA),
	STRINGIFY_ENUM(TOKEN_BITWISE_AND),
	STRINGIFY_ENUM(TOKEN_BITWISE_OR),
	STRINGIFY_ENUM(TOKEN_BITWISE_NOT),

	STRINGIFY_ENUM(TOKEN_LOGICAL_AND),
	STRINGIFY_ENUM(TOKEN_LOGICAL_OR),
	STRINGIFY_ENUM(TOKEN_LOGICAL_NOT),

	STRINGIFY_ENUM(TOKEN_EQUAL),
	STRINGIFY_ENUM(TOKEN_NOT_EQUAL),
	STRINGIFY_ENUM(TOKEN_LESS_THAN),
	STRINGIFY_ENUM(TOKEN_LESS_THAN_OR_EQUAL),
	STRINGIFY_ENUM(TOKEN_GREATER_THAN),
	STRINGIFY_ENUM(TOKEN_GREATER_THAN_OR_EQUAL),

	STRINGIFY_ENUM(TOKEN_ASSIGNMENT),
	STRINGIFY_ENUM(TOKEN_PLUS),
	STRINGIFY_ENUM(TOKEN_PLUS_EQUAL),
	STRINGIFY_ENUM(TOKEN_MINUS),
	STRINGIFY_ENUM(TOKEN_MINUS_EQUAL),
	STRINGIFY_ENUM(TOKEN_MULTIPLY),
	STRINGIFY_ENUM(TOKEN_MULTIPLY_EQUAL),
	STRINGIFY_ENUM(TOKEN_DIVIDE),
	STRINGIFY_ENUM(TOKEN_DIVIDE_EQUAL),
	STRINGIFY_ENUM(TOKEN_MODULUS),
	STRINGIFY_ENUM(TOKEN_MODULUS_EQUAL),

	STRINGIFY_ENUM(TOKEN_END_OF_FILE),
	STRINGIFY_ENUM(TOKEN_INVALID)
};

static_assert(TOKEN_TYPES_COUNT == (sizeof(_stringifiedTokenTypes) / sizeof(const char* const)), "Outdated _stringifiedTokenTypes size!");

const char* _stringifyTokenType(const enum _TokenType element)
{
	return element < 0 || element > TOKEN_TYPES_COUNT ?
		"UNKNOWN_TOKEN_TYPE" : _stringifiedTokenTypes[element];
}

static_assert(TOKEN_STORAGES_COUNT == 15, "Outdated TOKEN_STORAGES_COUNT value!");

static const char* const _stringifiedTokenStorages[TOKEN_STORAGES_COUNT] =
{
	STRINGIFY_ENUM(STORAGE_NONE),

	STRINGIFY_ENUM(STORAGE_IDENTIFIER),
	STRINGIFY_ENUM(STORAGE_KEYWORD),
	STRINGIFY_ENUM(STORAGE_CHAR),
	STRINGIFY_ENUM(STORAGE_INT8),
	STRINGIFY_ENUM(STORAGE_UINT8),
	STRINGIFY_ENUM(STORAGE_INT16),
	STRINGIFY_ENUM(STORAGE_UINT16),
	STRINGIFY_ENUM(STORAGE_INT32),
	STRINGIFY_ENUM(STORAGE_UINT32),
	STRINGIFY_ENUM(STORAGE_INT64),
	STRINGIFY_ENUM(STORAGE_UINT64),
	STRINGIFY_ENUM(STORAGE_FLOAT32),
	STRINGIFY_ENUM(STORAGE_FLOAT64),
	STRINGIFY_ENUM(STORAGE_STRING)
};

static_assert(TOKEN_STORAGES_COUNT == (sizeof(_stringifiedTokenStorages) / sizeof(const char* const)), "Outdated _stringifiedTokenStorages size!");

const char* _stringifyTokenStorage(const enum _TokenStorage element)
{
	return element < 0 || element > TOKEN_STORAGES_COUNT ?
		"UNKNOWN_TOKEN_STORAGE" : _stringifiedTokenStorages[element];
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
	char* buffer = (char*)malloc((length + 1) * sizeof(char));
	assert(buffer != NULL);
	memcpy(buffer, value, length);
	buffer[length] = '\0';
	signed int temp = 0;
	sscanf(buffer, "%d", &temp);
	free(buffer);
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
	char* buffer = (char*)malloc((length + 1) * sizeof(char));
	assert(buffer != NULL);
	memcpy(buffer, value, length);
	buffer[length] = '\0';
	unsigned int temp = 0;
	sscanf(buffer, "%u", &temp);
	free(buffer);
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
	char* buffer = (char*)malloc((length + 1) * sizeof(char));
	assert(buffer != NULL);
	memcpy(buffer, value, length);
	buffer[length] = '\0';
	sscanf(buffer, "%hd", &token.value.literal.i16);
	free(buffer);
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
	char* buffer = (char*)malloc((length + 1) * sizeof(char));
	assert(buffer != NULL);
	memcpy(buffer, value, length);
	buffer[length] = '\0';
	sscanf(buffer, "%hu", &token.value.literal.u16);
	free(buffer);
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
	char* buffer = (char*)malloc((length + 1) * sizeof(char));
	assert(buffer != NULL);
	memcpy(buffer, value, length);
	buffer[length] = '\0';
	sscanf(buffer, "%d", &token.value.literal.i32);
	free(buffer);
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
	char* buffer = (char*)malloc((length + 1) * sizeof(char));
	assert(buffer != NULL);
	memcpy(buffer, value, length);
	buffer[length] = '\0';
	sscanf(buffer, "%u", &token.value.literal.u32);
	free(buffer);
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
	char* buffer = (char*)malloc((length + 1) * sizeof(char));
	assert(buffer != NULL);
	memcpy(buffer, value, length);
	buffer[length] = '\0';
	sscanf(buffer, "%lld", &token.value.literal.i64);
	free(buffer);
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
	char* buffer = (char*)malloc((length + 1) * sizeof(char));
	assert(buffer != NULL);
	memcpy(buffer, value, length);
	buffer[length] = '\0';
	sscanf(buffer, "%llu", &token.value.literal.u64);
	free(buffer);
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
	char* buffer = (char*)malloc((length + 1) * sizeof(char));
	assert(buffer != NULL);
	memcpy(buffer, value, length);
	buffer[length] = '\0';
	sscanf(buffer, "%f", &token.value.literal.f32);
	free(buffer);
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
	char* buffer = (char*)malloc((length + 1) * sizeof(char));
	assert(buffer != NULL);
	memcpy(buffer, value, length);
	buffer[length] = '\0';
	sscanf(buffer, "%lf", &token.value.literal.f64);
	free(buffer);
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
	struct _Token* const token)
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

struct _Tokenizer _createTokenizer(
	const char* filePath)
{
	struct _Tokenizer tokenizer;
	tokenizer.location.file = filePath;
	tokenizer.location.line = 1;
	tokenizer.location.column = 1;
	tokenizer.length = 0;

	FILE* file = fopen(filePath, "r");

	if (!file)
	{
		fprintf(stderr, "ERROR: failed to open source file `%s`!\n", filePath);
		exit(1);
	}

	fseek(file, 0, SEEK_END);
	const unsigned int fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);
	tokenizer.buffer = (char*)malloc((fileSize + 1) * sizeof(char));
	fread(tokenizer.buffer, fileSize, sizeof(char), file);
	fclose(file);

	tokenizer.buffer[fileSize] = '\0';
	tokenizer.length = fileSize;
	tokenizer.current = tokenizer.buffer;
	return tokenizer;
}

void _destroyTokenizer(
	struct _Tokenizer* tokenizer)
{
	assert(tokenizer != NULL);
	free(tokenizer->buffer);
	tokenizer->length = 0;
}

static char _current(
	struct _Tokenizer* const tokenizer)
{
	assert(tokenizer != NULL);
	assert(tokenizer->current != NULL);
	return *(tokenizer->current);
}

static int _isWhitespace(
	struct _Tokenizer* const tokenizer)
{
	assert(tokenizer != NULL);
	assert(tokenizer->current != NULL);
	return _current(tokenizer) == 13
		|| _current(tokenizer) == 10
		|| _current(tokenizer) == ' '
		|| _current(tokenizer) == '\t'
		|| _current(tokenizer) == '\n';
}

static void _moveBy(
	struct _Tokenizer* const tokenizer,
	const unsigned int offset)
{
	assert(tokenizer != NULL);
	assert(tokenizer->current != NULL);

	for (unsigned int counter = 0; counter < offset; ++counter)
	{
		++tokenizer->location.column;

		if (_current(tokenizer) == '\n')
		{
			++tokenizer->location.line;
			tokenizer->location.column = 1;
		}

		++tokenizer->current;
	}
}

static void _skipWhitespaces(
	struct _Tokenizer* const tokenizer)
{
	assert(tokenizer != NULL);
	assert(tokenizer->current != NULL);

	while (_isWhitespace(tokenizer))
	{
		_moveBy(tokenizer, 1);
	}
}

static char _peekBy(
	struct _Tokenizer* const tokenizer,
	const unsigned int offset)
{
	assert(tokenizer != NULL);
	assert(tokenizer->current != NULL);
	return *(tokenizer->current + offset);
}

static void _skipSingleLineComments(
	struct _Tokenizer* const tokenizer)
{
	assert(tokenizer != NULL);
	assert(tokenizer->current != NULL);

	while (_current(tokenizer) == '/' && _peekBy(tokenizer, 1) == '/')
	{
		for (; _current(tokenizer) != '\n' && _current(tokenizer) != '\0';)
		{
			_moveBy(tokenizer, 1);
		}

		if (_current(tokenizer) == '\n')
		{
			_moveBy(tokenizer, 1);
		}

		_skipWhitespaces(tokenizer);
	}
}

static void _tryCreateMetaToken(
	struct _Tokenizer* const tokenizer,
	struct _Token* const token,
	const enum _TokenType type,
	const enum _TokenStorage storage,
	const unsigned int offset)
{
	assert(tokenizer != NULL);
	assert(token != NULL);
	*token = _metaToken(type, storage, tokenizer->location);
	_moveBy(tokenizer, offset);
}

static void _tryCreateInvalidToken(
	struct _Tokenizer* const tokenizer,
	struct _Token* const token,
	const unsigned int offset)
{
	assert(tokenizer != NULL);
	assert(token != NULL);
	*token = _metaToken(TOKEN_INVALID, STORAGE_NONE, tokenizer->location);
	_moveBy(tokenizer, offset);
}

static int _tryCreateIdentifierOrKeywordToken(
	struct _Tokenizer* const tokenizer,
	struct _Token* const token)
{
	assert(tokenizer != NULL);
	assert(tokenizer->current != NULL);
	assert(token != NULL);

	if (!isalpha(_current(tokenizer)))
	{
		return 0;
	}

	unsigned int length = 0;
	for (; isalpha(_peekBy(tokenizer, length)) || isdigit(_peekBy(tokenizer, length)); ++length);

	static_assert(TOKEN_TYPES_COUNT == 65, "TOKEN_TYPES_COUNT is higher than accounted in _tryCreateIdentifierOrKeywordToken!");
	#define KEYWORDS_COUNT 18
	static const char* const keywords[KEYWORDS_COUNT] =
	{ "if", "import", "export", "proc", "while", "return", "var",
	  "char", "int8", "uint8", "int16", "uint16", "int32", "uint32",
	  "int64", "uint64", "float32", "float64" };
	static const enum _TokenType types[KEYWORDS_COUNT] =
	{ TOKEN_IF_KEYWORD, TOKEN_IMPORT_KEYWORD, TOKEN_EXPORT_KEYWORD, TOKEN_PROC_KEYWORD, TOKEN_WHILE_KEYWORD, TOKEN_RETURN_KEYWORD, TOKEN_VAR_KEYWORD,
	  TOKEN_CHAR_KEYWORD, TOKEN_INT8_KEYWORD, TOKEN_UINT8_KEYWORD, TOKEN_INT16_KEYWORD, TOKEN_UINT16_KEYWORD, TOKEN_INT32_KEYWORD, TOKEN_UINT32_KEYWORD,
	  TOKEN_INT64_KEYWORD, TOKEN_UINT64_KEYWORD, TOKEN_FLOAT32_KEYWORD, TOKEN_FLOAT64_KEYWORD };

	for (unsigned int index = 0; index < KEYWORDS_COUNT; ++index)
	{
		if (strncmp(tokenizer->current, keywords[index], length) == 0)
		{
			*token = _keywordToken(tokenizer->current, length, types[index], tokenizer->location);
			_moveBy(tokenizer, length);
			return length;
		}
	}
	#undef KEYWORDS_COUNT

	*token = _identifierToken(tokenizer->current, length, tokenizer->location);
	_moveBy(tokenizer, length);
	return length;
}

/**
 * TODO: rework and optimize this function for numeric literals!
 */
static int _tryCreateLiteralToken(
	struct _Tokenizer* const tokenizer,
	struct _Token* const token)
{
	assert(tokenizer != NULL);
	assert(tokenizer->current != NULL);
	assert(token != NULL);

	if (_current(tokenizer) == '\'')
	{
		if (_peekBy(tokenizer, 2) == '\'')
		{
			_moveBy(tokenizer, 1);
			*token = _charLiteralToken(tokenizer->current, 1, tokenizer->location);
			_moveBy(tokenizer, 2);
			return TOKEN_CHAR_LITERAL;
		}
		else
		{
			return 0;
		}
	}
	else if (isdigit(_current(tokenizer)) || (_current(tokenizer) == '-' && isdigit(_peekBy(tokenizer, 1))))
	{
		int dotsCount = 0;
		unsigned int length = 0;

		if (_current(tokenizer) == '-')
		{
			++length;
		}

		for (; isdigit(_peekBy(tokenizer, length)) || _peekBy(tokenizer, length) == '.'; ++length)
		{
			if (_peekBy(tokenizer, length) == '.') ++dotsCount;
		}

		if (dotsCount > 1)
		{
			fprintf(stderr, "ERROR: faiure while parsing numeric literal!\n");
			exit(1);
		}
		else if (dotsCount == 1)
		{
			*token = _float32LiteralToken(tokenizer->current, length, tokenizer->location);
			_moveBy(tokenizer, length);
			return TOKEN_FLOAT32_LITERAL;
		}
		else
		{
			*token = _int32LiteralToken(tokenizer->current, length, tokenizer->location);
			_moveBy(tokenizer, length);
			return TOKEN_INT32_LITERAL;
		}

		return 0;
	}
	else if (_current(tokenizer) == '\"')
	{
		_moveBy(tokenizer, 1);
		unsigned int length = 0;
		for (; _peekBy(tokenizer, length) != '\"'; ++length);
		*token = _stringLiteralToken(tokenizer->current, length, tokenizer->location);
		_moveBy(tokenizer, length + 1);
		return TOKEN_STRING_LITERAL;
	}
	else
	{
		return 0;
	}
}

void _nextToken(
	struct _Tokenizer* const tokenizer,
	struct _Token* const token)
{
	assert(tokenizer != NULL);
	assert(tokenizer->current != NULL);
	assert(token != NULL);

	// 1. Handle whitespaces
	_skipWhitespaces(tokenizer);

	// 2. Handle comments
	_skipSingleLineComments(tokenizer);

	// 3. Handle identifier token
	if (_tryCreateIdentifierOrKeywordToken(tokenizer, token) > 0)
	{
		return;
	}

	// 4. Handle literal tokens
	if (_tryCreateLiteralToken(tokenizer, token) > 0)
	{
		return;
	}

	// 5. Handle all the other tokens
	switch (_current(tokenizer))
	{
		case '(':
		{
			_tryCreateMetaToken(tokenizer, token, TOKEN_LEFT_PARENTHESIS, STORAGE_NONE, 1);
		} break;

		case ')':
		{
			_tryCreateMetaToken(tokenizer, token, TOKEN_RIGHT_PARENTHESIS, STORAGE_NONE, 1);
		} break;

		case '[':
		{
			_tryCreateMetaToken(tokenizer, token, TOKEN_LEFT_BRACKET, STORAGE_NONE, 1);
		} break;

		case ']':
		{
			_tryCreateMetaToken(tokenizer, token, TOKEN_RIGHT_BRACKET, STORAGE_NONE, 1);
		} break;

		case '{':
		{
			_tryCreateMetaToken(tokenizer, token, TOKEN_LEFT_BRACE, STORAGE_NONE, 1);
		} break;

		case '}':
		{
			_tryCreateMetaToken(tokenizer, token, TOKEN_RIGHT_BRACE, STORAGE_NONE, 1);
		} break;

		case ';':
		{
			_tryCreateMetaToken(tokenizer, token, TOKEN_SEMICOLON, STORAGE_NONE, 1);
		} break;

		case ':':
		{
			_tryCreateMetaToken(tokenizer, token, TOKEN_COLON, STORAGE_NONE, 1);
		} break;

		case ',':
		{
			_tryCreateMetaToken(tokenizer, token, TOKEN_COMMA, STORAGE_NONE, 1);
		} break;

		case '&':
		{
			switch (_peekBy(tokenizer, 1))
			{
				case '&':
				{
					_tryCreateMetaToken(tokenizer, token, TOKEN_LOGICAL_AND, STORAGE_NONE, 2);
				} break;

				default:
				{
					_tryCreateMetaToken(tokenizer, token, TOKEN_BITWISE_AND, STORAGE_NONE, 1);
				} break;
			}
		} break;

		case '|':
		{
			switch (_peekBy(tokenizer, 1))
			{
				case '|':
				{
					_tryCreateMetaToken(tokenizer, token, TOKEN_LOGICAL_OR, STORAGE_NONE, 2);
				} break;

				default:
				{
					_tryCreateMetaToken(tokenizer, token, TOKEN_BITWISE_OR, STORAGE_NONE, 1);
				} break;
			}
		} break;

		case '~':
		{
			_tryCreateMetaToken(tokenizer, token, TOKEN_BITWISE_NOT, STORAGE_NONE, 1);
		} break;

		case '!':
		{
			switch (_peekBy(tokenizer, 1))
			{
				case '=':
				{
					_tryCreateMetaToken(tokenizer, token, TOKEN_NOT_EQUAL, STORAGE_NONE, 2);
				} break;

				default:
				{
					_tryCreateMetaToken(tokenizer, token, TOKEN_LOGICAL_NOT, STORAGE_NONE, 1);
				} break;
			}
		} break;

		case '=':
		{
			switch (_peekBy(tokenizer, 1))
			{
				case '=':
				{
					_tryCreateMetaToken(tokenizer, token, TOKEN_EQUAL, STORAGE_NONE, 2);
				} break;

				default:
				{
					_tryCreateMetaToken(tokenizer, token, TOKEN_ASSIGNMENT, STORAGE_NONE, 1);
				} break;
			}
		} break;

		case '<':
		{
			switch (_peekBy(tokenizer, 1))
			{
				case '=':
				{
					_tryCreateMetaToken(tokenizer, token, TOKEN_LESS_THAN_OR_EQUAL, STORAGE_NONE, 2);
				} break;

				default:
				{
					_tryCreateMetaToken(tokenizer, token, TOKEN_LESS_THAN, STORAGE_NONE, 1);
				} break;
			}
		} break;

		case '>':
		{
			switch (_peekBy(tokenizer, 1))
			{
				case '=':
				{
					_tryCreateMetaToken(tokenizer, token, TOKEN_GREATER_THAN_OR_EQUAL, STORAGE_NONE, 2);
				} break;

				default:
				{
					_tryCreateMetaToken(tokenizer, token, TOKEN_GREATER_THAN, STORAGE_NONE, 1);
				} break;
			}
		} break;

		case '+':
		{
			switch (_peekBy(tokenizer, 1))
			{
				case '=':
				{
					_tryCreateMetaToken(tokenizer, token, TOKEN_PLUS_EQUAL, STORAGE_NONE, 2);
				} break;

				default:
				{
					_tryCreateMetaToken(tokenizer, token, TOKEN_PLUS, STORAGE_NONE, 1);
				} break;
			}
		} break;

		case '-':
		{
			switch (_peekBy(tokenizer, 1))
			{
				case '=':
				{
					_tryCreateMetaToken(tokenizer, token, TOKEN_MINUS_EQUAL, STORAGE_NONE, 2);
				} break;

				default:
				{
					_tryCreateMetaToken(tokenizer, token, TOKEN_MINUS, STORAGE_NONE, 1);
				} break;
			}
		} break;

		case '*':
		{
			switch (_peekBy(tokenizer, 1))
			{
				case '=':
				{
					_tryCreateMetaToken(tokenizer, token, TOKEN_MULTIPLY_EQUAL, STORAGE_NONE, 2);
				} break;

				default:
				{
					_tryCreateMetaToken(tokenizer, token, TOKEN_MULTIPLY, STORAGE_NONE, 1);
				} break;
			}
		} break;

		case '/':
		{
			switch (_peekBy(tokenizer, 1))
			{
				case '=':
				{
					_tryCreateMetaToken(tokenizer, token, TOKEN_DIVIDE_EQUAL, STORAGE_NONE, 2);
				} break;

				default:
				{
					_tryCreateMetaToken(tokenizer, token, TOKEN_DIVIDE, STORAGE_NONE, 1);
				} break;
			}
		} break;

		case '%':
		{
			switch (_peekBy(tokenizer, 1))
			{
				case '=':
				{
					_tryCreateMetaToken(tokenizer, token, TOKEN_MODULUS_EQUAL, STORAGE_NONE, 2);
				} break;

				default:
				{
					_tryCreateMetaToken(tokenizer, token, TOKEN_MODULUS, STORAGE_NONE, 1);
				} break;
			}
		} break;

		case '\0':
		{
			_tryCreateMetaToken(tokenizer, token, TOKEN_END_OF_FILE, STORAGE_NONE, 1);
		} break;

		default:
		{
			_tryCreateInvalidToken(tokenizer, token, 1);
			fprintf(stderr, "ERROR: unknown symbol `%c` at location %s! Cannot tokenize!\n", *tokenizer->current, _stringifyLocation(&tokenizer->location));
		} break;
	}
}

void _peekToken(
	struct _Tokenizer* const tokenizer,
	struct _Token* const token,
	const unsigned int offset)
{
	assert(tokenizer != NULL);
	assert(tokenizer->current != NULL);
	assert(token != NULL);

	const char* current = tokenizer->current;
	struct _Location location = tokenizer->location;

	_nextToken(tokenizer, token);

	for (unsigned int index = 0; index < offset; ++index)
	{
		_destroyToken(token);
		_nextToken(tokenizer, token);
	}

	tokenizer->current = current;
	tokenizer->location = location;
}

enum _TokenType _expectToken(
	struct _Tokenizer* const tokenizer,
	struct _Token* const token,
	const enum _TokenType type,
	const int strict)
{
	_nextToken(tokenizer, token);

	if (token->type != type)
	{
		fprintf(stderr, "ERROR: expected token of type %s, but parsed %s!\n",
			_stringifyTokenType(type), _stringifyTokenType(token->type));

		if (strict)
		{
			exit(1);
		}
	}

	return token->type;
}
