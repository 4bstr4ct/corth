#ifndef _TOKEN_H_
#define _TOKEN_H_

#include "./location.h"

enum _TokenType
{
	TOKEN_IDENTIFIER			=  0,

	TOKEN_IF_KEYWORD				,
	TOKEN_ELSE_KEYWORD				,
	TOKEN_IMPORT_KEYWORD			,
	TOKEN_EXPORT_KEYWORD			,
	TOKEN_PROC_KEYWORD				,
	TOKEN_WHILE_KEYWORD				,
	TOKEN_RETURN_KEYWORD			,

	TOKEN_CHAR_LITERAL				,
	TOKEN_INT8_LITERAL				,
	TOKEN_UINT8_LITERAL				,
	TOKEN_INT16_LITERAL				,
	TOKEN_UINT16_LITERAL			,
	TOKEN_INT32_LITERAL				,
	TOKEN_UINT32_LITERAL			,
	TOKEN_INT64_LITERAL				,
	TOKEN_UINT64_LITERAL			,
	TOKEN_FLOAT32_LITERAL			,
	TOKEN_FLOAT64_LITERAL			,
	TOKEN_STRING_LITERAL			,

	TOKEN_LEFT_PARENTHESIS			,
	TOKEN_RIGHT_PARENTHESIS			,
	TOKEN_LEFT_BRACE				,
	TOKEN_RIGHT_BRACE				,
	TOKEN_LEFT_BRACKET				,
	TOKEN_RIGHT_BRACKET				,
	TOKEN_SEMICOLON					,
	TOKEN_COLON						,
	TOKEN_COMMA						,

	TOKEN_END_OF_FILE				,
	TOKEN_INVALID					,

	TOKEN_TYPES_COUNT
};

const char* _stringifyTokenType(
	const enum _TokenType type);

enum _TokenStorage
{
	STORAGE_NONE				= 0 ,

	STORAGE_IDENTIFIER				,
	STORAGE_KEYWORD					,

	STORAGE_CHAR					,
	STORAGE_INT8					,
	STORAGE_UINT8					,
	STORAGE_INT16					,
	STORAGE_UINT16					,
	STORAGE_INT32					,
	STORAGE_UINT32					,
	STORAGE_INT64					,
	STORAGE_UINT64					,
	STORAGE_FLOAT32					,
	STORAGE_FLOAT64					,
	STORAGE_STRING					,

	TOKEN_STORAGES_COUNT			,
};

const char* _stringifyTokenStorage(
	const enum _TokenStorage storage);

union _TokenValue
{
	struct
	{
		char* buffer;
		unsigned int length;
	} identifier;
	struct
	{
		char* buffer;
		unsigned int length;
	} keyword;
	union
	{
		char c;
		signed char i8;
		unsigned char u8;
		signed short i16;
		unsigned short u16;
		signed int i32;
		unsigned int u32;
		signed long long i64;
		unsigned long long u64;
		float f32;
		double f64;
		struct
		{
			char* buffer;
			unsigned int length;
		} string;
	} literal;
};

struct _Token
{
	enum _TokenType type;
	enum _TokenStorage storage;
	struct _Location location;
	union _TokenValue value;
};

struct _Token _identifierToken(
	const char* const value,
	const unsigned int length,
	const struct _Location location);

struct _Token _keywordToken(
	const char* const value,
	const unsigned int length,
	const enum _TokenType type,
	const struct _Location location);

struct _Token _metaToken(
	const enum _TokenType type,
	const enum _TokenStorage storage,
	const struct _Location location);

struct _Token _charLiteralToken(
	const char* const value,
	const unsigned int length,
	const struct _Location location);

struct _Token _int8LiteralToken(
	const char* const value,
	const unsigned int length,
	const struct _Location location);

struct _Token _uint8LiteralToken(
	const char* const value,
	const unsigned int length,
	const struct _Location location);

struct _Token _int16LiteralToken(
	const char* const value,
	const unsigned int length,
	const struct _Location location);

struct _Token _uint16LiteralToken(
	const char* const value,
	const unsigned int length,
	const struct _Location location);

struct _Token _int32LiteralToken(
	const char* const value,
	const unsigned int length,
	const struct _Location location);

struct _Token _uint32LiteralToken(
	const char* const value,
	const unsigned int length,
	const struct _Location location);

struct _Token _int64LiteralToken(
	const char* const value,
	const unsigned int length,
	const struct _Location location);

struct _Token _uint64LiteralToken(
	const char* const value,
	const unsigned int length,
	const struct _Location location);

struct _Token _float32LiteralToken(
	const char* const value,
	const unsigned int length,
	const struct _Location location);

struct _Token _float64LiteralToken(
	const char* const value,
	const unsigned int length,
	const struct _Location location);

struct _Token _stringLiteralToken(
	const char* const value,
	const unsigned int length,
	const struct _Location location);

struct _Token _invalidToken(
	const struct _Location location);

void _destroyToken(
	struct _Token* token);

const char* _stringifyToken(
	const struct _Token* const token);

#endif
