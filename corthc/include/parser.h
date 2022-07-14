#ifndef _PARSER_H_
#define _PARSER_H_

#include "./token.h"

enum _ExpressionType
{
	EXPRESSION_CALL				=  0,
	EXPRESSION_IF					,
	EXPRESSION_WHILE				,
	EXPRESSION_RETURN				,
	EXPRESSION_COMPOUND				,
	EXPRESSION_TYPES_COUNT
}

struct _ASTExpression
{

};

void _parse(const char* filePath);

#endif
