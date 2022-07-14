#ifndef _DEFS_H_
#define _DEFS_H_

struct _Location
{
	const char* file;
	unsigned int line;
	unsigned int column;
};

const char* _stringifyLocation(
	const struct _Location* const location);

enum _TokenType
{
	TOKEN_IDENTIFIER			=  0,
	TOKEN_INT32_LITERAL				,
	TOKEN_STRING_LITERAL			,
	TOKEN_END_OF_FILE				,

	TOKEN_IF						,
	TOKEN_END						,
	TOKEN_INCLUDE					,
	TOKEN_PROC						,
	TOKEN_WHILE						,
	TOKEN_RETURN					,
	TOKEN_VAR						,

	TOKEN_LEFT_PARENTHESIS			,
	TOKEN_RIGHT_PARENTHESIS			,
	TOKEN_COMMA						,
	TOKEN_COLON						,

	TOKEN_PLUS						,
	TOKEN_MINUS						,

	TOKEN_INVALID					,
	TOKEN_TYPES_COUNT
};

const char* _stringifyTokenType(
	const enum _TokenType type);

enum _TokenStorage
{
	STORAGE_NONE			= 0,
	STORAGE_INT32			= 1,
	STORAGE_STRING			= 2,
	TOKEN_STORAGES_COUNT,
};

const char* _stringifyTokenStorage(
	const enum _TokenStorage storage);

union _TokenValue
{
	int i32;
	struct
	{
		char* buffer;
		unsigned int length;
	} s;
};

struct _Token
{
	enum _TokenType type;
	enum _TokenStorage storage;
	union _TokenValue value;
	struct _Location location;
};

struct _Token _identifierToken(
	const char* const value,
	const unsigned int length,
	const struct _Location location);

struct _Token _int32LiteralToken(
	const char* const value,
	const unsigned int length,
	const struct _Location location);

struct _Token _stringLiteralToken(
	const char* const value,
	const unsigned int length,
	const struct _Location location);

void _destroyToken(
	struct _Token* token);

const char* _stringifyToken(
	const struct _Token* const token);

#endif
