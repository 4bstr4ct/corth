#ifndef _LEXER_H_
#define _LEXER_H_

#include "./location.h"
#include "./token.h"

#ifndef LEXER_BUFFER_CAPACITY
#	define LEXER_BUFFER_CAPACITY 4096 * 2
#endif

struct _Lexer
{
	char* buffer;
	unsigned int length;
	const char* current;
	struct _Location location;
};

struct _Lexer _createLexer(
	const char* filePath);

void _destroyLexer(
	struct _Lexer* lexer);

void _nextToken(
	struct _Lexer* const lexer,
	struct _Token* const token);

void _peekToken(
	struct _Lexer* const lexer,
	struct _Token* const token,
	const unsigned int offset);

enum _TokenType _expectToken(
	struct _Lexer* const lexer,
	struct _Token* const token,
	const enum _TokenType type,
	const int strict);

#endif
