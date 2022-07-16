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

enum _StatementType
{
	STATEMENT_COMPOUND			=  0,
	STATEMENT_CONDITIONAL			,
	STATEMENT_LOOP					,
	STATEMENT_RETURN				,
	STATEMENT_VARIABLE_DEFINITION	,
	STATEMENT_EXPRESSION			,
	STATEMENT_NO_OPERATION			,

	STATEMENT_TYPES_COUNT
};

struct _Statement
{
	enum _StatementType type; 
	struct _Expression* expression;
	struct _Statement* next;
};

struct _ProcedureArg
{
	char* name;
	struct _ProcedureArg* next;
};

struct _Procedure
{
	const char* name;
	struct _ProcedureArg* arg;
	struct _Statement* statement;
};

void _parse(const char* filePath);

#endif
