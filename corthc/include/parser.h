#ifndef _PARSER_H_
#define _PARSER_H_

#include "./location.h"
#include "./token.h"
#include "./lexer.h"

enum _ExpressionType
{
	EXPRESSION_CHAR_LITERAL		=  0,
	EXPRESSION_INT8_LITERAL			,
	EXPRESSION_UINT8_LITERAL		,
	EXPRESSION_INT16_LITERAL		,
	EXPRESSION_UINT16_LITERAL		,
	EXPRESSION_INT32_LITERAL		,
	EXPRESSION_UINT32_LITERAL		,
	EXPRESSION_INT64_LITERAL		,
	EXPRESSION_UINT64_LITERAL		,
	EXPRESSION_FLOAT32_LITERAL		,
	EXPRESSION_FLOAT64_LITERAL		,
	EXPRESSION_STRING_LITERAL		,

	EXPRESSION_VARIABLE				,
	EXPRESSION_BINARY_OPERATION		,
	EXPRESSION_PROCEDURE_CALL		,

	EXPRESSION_TYPES_COUNT
};

struct _Expression
{
	enum _ExpressionType type;
};

struct _Statement
{
	struct _Expression expression;
};

struct _Block
{
	struct _Statement statement;
	struct _Block* next;
};

struct _Procedure
{
	const char* name;
	struct _Block* body;
};

void _parse(const char* filePath);

#endif
