#ifndef _PARSER_H_
#define _PARSER_H_

#include "./location.h"
#include "./tokenizer.h"





/*
enum _LiteralExpressionType
{
	LITERAL_EXPRESSION_IDENTIFIER	=  0,
	LITERAL_EXPRESSION_CHAR				,
	LITERAL_EXPRESSION_INT8				,
	LITERAL_EXPRESSION_UINT8			,
	LITERAL_EXPRESSION_INT16			,
	LITERAL_EXPRESSION_UINT16			,
	LITERAL_EXPRESSION_INT32			,
	LITERAL_EXPRESSION_UINT32			,
	LITERAL_EXPRESSION_INT64			,
	LITERAL_EXPRESSION_UINT64			,
	LITERAL_EXPRESSION_FLOAT32			,
	LITERAL_EXPRESSION_FLOAT64			,
	LITERAL_EXPRESSION_STRING			,

	LITERAL_EXPRESSION_TYPES_COUNT
};

const char* _stringifyLiteralExpressionType(
	const enum _LiteralExpressionType type);

struct _LiteralExpression
{
	enum _LiteralExpressionType type;
	struct
	{
		char* buffer;
		unsigned int length;
	} identifier;
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

enum _ExpressionType
{
	EXPRESSION_LITERAL				=  0,
	EXPRESSION_INDEXING					,
	EXPRESSION_PROCEDURE_CALL			,
	EXPRESSION_BINARY					,
	EXPRESSION_UNARY					,

	EXPRESSION_TYPES_COUNT
};

const char* _stringifyExpressionType(
	const enum _ExpressionType type);

struct _Expression
{
	enum _ExpressionType type;
	union
	{
		struct _LiteralExpression literal;
	} kind;
};





enum _StatementType
{
	STATEMENT_COMPOUND				=  0,
	STATEMENT_CONDITIONAL				,
	STATEMENT_LOOP						,
	STATEMENT_RETURN					,
	STATEMENT_VARIABLE_DEFINITION		,
	STATEMENT_EXPRESSION				,
	STATEMENT_NO_OPERATION				,

	STATEMENT_TYPES_COUNT
};

const char* _stringifyStatementType(
	const enum _StatementType type);

struct _Statement
{
	enum _StatementType type; 
	struct _Expression* expression;
	struct _Statement* next;
};





struct _Procedure
{
	const char* name;
	struct _Statement* statement;
};
*/





void _parse(const char* filePath);

#endif
