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
	TOKEN_INCLUDE			= 0,
	TOKEN_PROCEDURE			= 1,
	TOKEN_END				= 2,
	TOKEN_DOUBLE_QUOTE		= 3,
	TOKEN_SEMICOLON			= 4,
	TOKEN_COLON				= 5,
	TOKEN_LITERAL			= 6,
	TOKEN_NAME				= 7,
	TOKEN_END_OF_FILE		= 8,
	TOKEN_ERROR				= 9,
	TOKEN_TYPES_COUNT,
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

#ifndef TOKEN_STRING_VALUE_CAPACITY
#	define TOKEN_STRING_VALUE_CAPACITY 256
#endif

union _TokenValue
{
	int i32;
	struct
	{
		char buffer[TOKEN_STRING_VALUE_CAPACITY];
	} s;
};

struct _Token
{
	enum _TokenType type;
	enum _TokenStorage storage;
	union _TokenValue value;
	struct _Location location;
};

const char* _stringifyToken(
	const struct _Token* const token);

#endif
