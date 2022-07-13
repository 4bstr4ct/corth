#ifndef _LEXER_H_
#define _LEXER_H_

#include "./defs.h"

#ifndef LEXER_BUFFER_CAPACITY
#	define LEXER_BUFFER_CAPACITY 4069 * 2
#endif

struct _Lexer
{
	char buffer[LEXER_BUFFER_CAPACITY];
	unsigned int length;
	const char* current;
	struct _Location location;
};

struct _Lexer _createLexer(
	const char* filePath);

void _nextToken(
	struct _Lexer* const lexer,
	struct _Token* const token);

#endif
