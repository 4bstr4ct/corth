#ifndef _TOKENIZER_H_
#define _TOKENIZER_H_

#include "./meta.h"
#include "./location.h"

/**
 * @addtogroup TOKEN
 * 
 * @{
 */

/**
 * Token's types enum.
 */
enum _TokenType
{
	TOKEN_IDENTIFIER				=  0,

	TOKEN_IF_KEYWORD					,
	TOKEN_IMPORT_KEYWORD				,
	TOKEN_EXPORT_KEYWORD				,
	TOKEN_PROC_KEYWORD					,
	TOKEN_WHILE_KEYWORD					,
	TOKEN_RETURN_KEYWORD				,
	TOKEN_VAR_KEYWORD					,

	TOKEN_CHAR_KEYWORD					,
	TOKEN_INT8_KEYWORD					,
	TOKEN_UINT8_KEYWORD					,
	TOKEN_INT16_KEYWORD					,
	TOKEN_UINT16_KEYWORD				,
	TOKEN_INT32_KEYWORD					,
	TOKEN_UINT32_KEYWORD				,
	TOKEN_INT64_KEYWORD					,
	TOKEN_UINT64_KEYWORD				,
	TOKEN_FLOAT32_KEYWORD				,
	TOKEN_FLOAT64_KEYWORD				,

	TOKEN_CHAR_LITERAL					,
	TOKEN_INT8_LITERAL					,
	TOKEN_UINT8_LITERAL					,
	TOKEN_INT16_LITERAL					,
	TOKEN_UINT16_LITERAL				,
	TOKEN_INT32_LITERAL					,
	TOKEN_UINT32_LITERAL				,
	TOKEN_INT64_LITERAL					,
	TOKEN_UINT64_LITERAL				,
	TOKEN_FLOAT32_LITERAL				,
	TOKEN_FLOAT64_LITERAL				,
	TOKEN_STRING_LITERAL				,

	TOKEN_LEFT_PARENTHESIS				,
	TOKEN_RIGHT_PARENTHESIS				,
	TOKEN_LEFT_BRACE					,
	TOKEN_RIGHT_BRACE					,
	TOKEN_LEFT_BRACKET					,
	TOKEN_RIGHT_BRACKET					,
	TOKEN_SEMICOLON						,
	TOKEN_COLON							,
	TOKEN_COMMA							,

	TOKEN_BITWISE_AND					,
	TOKEN_BITWISE_OR					,
	TOKEN_BITWISE_NOT					,

	TOKEN_LOGICAL_AND					,
	TOKEN_LOGICAL_OR					,
	TOKEN_LOGICAL_NOT					,

	TOKEN_EQUAL							,
	TOKEN_NOT_EQUAL						,
	TOKEN_LESS_THAN						,
	TOKEN_LESS_THAN_OR_EQUAL			,
	TOKEN_GREATER_THAN					,
	TOKEN_GREATER_THAN_OR_EQUAL			,

	TOKEN_ASSIGNMENT					,
	TOKEN_PLUS							,
	TOKEN_PLUS_EQUAL					,
	TOKEN_MINUS							,
	TOKEN_MINUS_EQUAL					,
	TOKEN_MULTIPLY						,
	TOKEN_MULTIPLY_EQUAL				,
	TOKEN_DIVIDE						,
	TOKEN_DIVIDE_EQUAL					,
	TOKEN_MODULUS						,
	TOKEN_MODULUS_EQUAL					,

	TOKEN_END_OF_FILE					,
	TOKEN_INVALID						,

	TOKEN_TYPES_COUNT
};

/**
 * Stringify token type.
 */
const char* _stringifyTokenType(const enum _TokenType element);

/**
 * Token's storages enum.
 */
enum _TokenStorage
{
	STORAGE_NONE					=  0,

	STORAGE_IDENTIFIER					,
	STORAGE_KEYWORD						,
	STORAGE_CHAR						,
	STORAGE_INT8						,
	STORAGE_UINT8						,
	STORAGE_INT16						,
	STORAGE_UINT16						,
	STORAGE_INT32						,
	STORAGE_UINT32						,
	STORAGE_INT64						,
	STORAGE_UINT64						,
	STORAGE_FLOAT32						,
	STORAGE_FLOAT64						,
	STORAGE_STRING						,

	TOKEN_STORAGES_COUNT
};

/**
 * Stringify token storage.
 */
const char* _stringifyTokenStorage(const enum _TokenStorage element);

/**
 * A union that holds an identifier, a keyword, and the literal values.
 * 
 * NOTE: literal value contains all supported literals under the hood.
 */
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

/**
 * A structure that holds token's @ref _TokenType "type", @ref _TokenStorage "storage",
 * @ref _Location "location", and a @ref _TokenValue "value". Certain types of tokens,
 * such as @ref TOKEN_IDENTIFIER "identifier token", @ref TOKEN_STRING_LITERAL "string
 * literal token", or keyword tokens have a buffer allocated which must be deallocated
 * by @ref _destroyToken() "token destructor".
 */
struct _Token
{
	enum _TokenType type;
	enum _TokenStorage storage;
	struct _Location location;
	union _TokenValue value;
};

/**
 * Create an identifier token with provided value - the name of the identifier, value's
 * length, a @ref TOKEN_IDENTIFIER "identifier token type", and @ref _Location "location"
 * of the token in the file.
 */
struct _Token _identifierToken(
	const char* const value,
	const unsigned int length,
	const struct _Location location);

/**
 * Create a keyword token with provided value - the name of the keyword, a type of the
 * token keyword, value's length, and @ref _Location "location" of the token in the file.
 */
struct _Token _keywordToken(
	const char* const value,
	const unsigned int length,
	const enum _TokenType type,
	const struct _Location location);

/**
 * Create a meta token which has no value yet, but has a specified storage, type, and
 * @ref _Location "location" of the token in the file.
 */
struct _Token _metaToken(
	const enum _TokenType type,
	const enum _TokenStorage storage,
	const struct _Location location);

/**
 * Create a char literal token which has a char value, a @ref TOKEN_CHAR_LITERAL "char
 * literal token type", @ref STORAGE_CHAR "char storage", and @ref _Location "location"
 * of the token in the file.
 */
struct _Token _charLiteralToken(
	const char* const value,
	const unsigned int length,
	const struct _Location location);

/**
 * Create an int8 literal token which has an int8 value, a @ref TOKEN_INT8_LITERAL "int8
 * literal token type", @ref STORAGE_INT8 "int8 storage", and @ref _Location "location"
 * of the token in the file.
 */
struct _Token _int8LiteralToken(
	const char* const value,
	const unsigned int length,
	const struct _Location location);

/**
 * Create an uint8 literal token which has an uint8 value, a @ref TOKEN_UINT8_LITERAL
 * "uint8 literal token type", @ref STORAGE_UINT8 "uint8 storage", and @ref _Location
 * "location" of the token in the file.
 */
struct _Token _uint8LiteralToken(
	const char* const value,
	const unsigned int length,
	const struct _Location location);

/**
 * Create an int16 literal token which has an int16 value, a @ref TOKEN_INT16_LITERAL
 * "int16 literal token type", @ref STORAGE_INT16 "int16 storage", and @ref _Location
 * "location" of the token in the file.
 */
struct _Token _int16LiteralToken(
	const char* const value,
	const unsigned int length,
	const struct _Location location);

/**
 * Create an uint16 literal token which has an uint16 value, a @ref TOKEN_UINT16_LITERAL
 * "uint16 literal token type", @ref STORAGE_UINT16 "uint16 storage", and @ref _Location
 * "location" of the token in the file.
 */
struct _Token _uint16LiteralToken(
	const char* const value,
	const unsigned int length,
	const struct _Location location);

/**
 * Create an int32 literal token which has an int32 value, a @ref TOKEN_INT32_LITERAL
 * "int32 literal token type", @ref STORAGE_INT32 "int32 storage", and @ref _Location
 * "location" of the token in the file.
 */
struct _Token _int32LiteralToken(
	const char* const value,
	const unsigned int length,
	const struct _Location location);

/**
 * Create an uint32 literal token which has an uint32 value, a @ref TOKEN_UINT32_LITERAL
 * "uint32 literal token type", @ref STORAGE_UINT32 "uint32 storage", and @ref _Location
 * "location" of the token in the file.
 */
struct _Token _uint32LiteralToken(
	const char* const value,
	const unsigned int length,
	const struct _Location location);

/**
 * Create an int64 literal token which has an int64 value, a @ref TOKEN_INT64_LITERAL
 * "int64 literal token type", @ref STORAGE_INT64 "int64 storage", and @ref _Location
 * "location" of the token in the file.
 */
struct _Token _int64LiteralToken(
	const char* const value,
	const unsigned int length,
	const struct _Location location);

/**
 * Create an uint64 literal token which has an uint64 value, a @ref TOKEN_UINT64_LITERAL
 * "uint64 literal token type", @ref STORAGE_UINT64 "uint64 storage", and @ref _Location
 * "location" of the token in the file.
 */
struct _Token _uint64LiteralToken(
	const char* const value,
	const unsigned int length,
	const struct _Location location);

/**
 * Create a float32 literal token which has a float32 value, a @ref TOKEN_FLOAT32_LITERAL
 * "float32 literal token type", @ref STORAGE_FLOAT32 "float32 storage", a @ref _Location
 * "location" of the token in the file.
 */
struct _Token _float32LiteralToken(
	const char* const value,
	const unsigned int length,
	const struct _Location location);

/**
 * Create a float64 literal token which has a float64 value, a @ref TOKEN_FLOAT64_LITERAL
 * "float64 literal token type", @ref STORAGE_FLOAT64 "float64 storage", a @ref _Location
 * "location" of the token in the file.
 */
struct _Token _float64LiteralToken(
	const char* const value,
	const unsigned int length,
	const struct _Location location);

/**
 * Create a string literal token which has a buffer and a length fields s a struct, a
 * @ref TOKEN_STRING_LITERAL "string literal token type", @ref STORAGE_STRING "string
 * storage", and @ref _Location "location" of the token in the file.
 */
struct _Token _stringLiteralToken(
	const char* const value,
	const unsigned int length,
	const struct _Location location);

/**
 * Create an invalid token that represent tokenizer error. Invalid token will not have a
 * value or a storage, only a type of @ref TOKEN_INVALID and storage type being set to
 * @ref STORAGE_NONE.
 */
struct _Token _invalidToken(
	const struct _Location location);

/**
 * Destroy and deallocate all dynamically allocated resources for a specified token,
 * thus freeing the memory and reseting it to 0.
 * 
 * NOTE: only mandatory for tokens which were of type identifier, string literal or
 * keyword. For more information about the token types see @ref _TokenType "types".
 */
void _destroyToken(
	struct _Token* const token);

/**
 * Stringify a token with all its fields into a static chars buffer and return a pointer
 * to that buffer's beginning.
 */
const char* _stringifyToken(
	const struct _Token* const token);

/**
 * @}
 */

/**
 * @addtogroup TOKENIZER
 * 
 * @{
 */

/**
 * A structure which stores a file's content, a pointer to the current symbol, a @ref
 * _Location "location" of the current symbol in the file, and a length of the file's
 * content.
 */
struct _Tokenizer
{
	char* buffer;
	unsigned int length;
	const char* current;
	struct _Location location;
};

/**
 * Create the tokenizer, read the file and store it's content in a dynamically allocated
 * buffer.
 * 
 * NOTE: the tokenizer must be deallocated by @ref _destroyTokenizer function to free all
 * it's resources.
 */
struct _Tokenizer _createTokenizer(
	const char* filePath);

/**
 * Deallocate and free all tokenizer's resources and fields that were allocated on heap.
 */
void _destroyTokenizer(
	struct _Tokenizer* const tokenizer);

/**
 * Parse and return next token.
 */
void _nextToken(
	struct _Tokenizer* const tokenizer,
	struct _Token* const token);

/**
 * Parse and peek the next token without moving the parser.
 */
void _peekToken(
	struct _Tokenizer* const tokenizer,
	struct _Token* const token,
	const unsigned int offset);

/**
 * Parse the next token and compare to the expected type. If strict flag is non-zero, the
 * program will terminate in case of type mis-match. The function returns the parsed
 * token's type.
 */
enum _TokenType _expectToken(
	struct _Tokenizer* const tokenizer,
	struct _Token* const token,
	const enum _TokenType type,
	const int strict);

/**
 * @}
 */

#endif
