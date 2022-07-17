#include "../include/lexer.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <stdio.h>

struct _Lexer _createLexer(
	const char* filePath)
{
	struct _Lexer lexer;
	lexer.location.file = filePath;
	lexer.location.line = 1;
	lexer.location.column = 1;
	lexer.length = 0;

	FILE* file = fopen(filePath, "r");

	if (!file)
	{
		fprintf(stderr, "ERROR: failed to open source file `%s`!\n", filePath);
		exit(1);
	}

	fseek(file, 0, SEEK_END);
	const unsigned int fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);
	lexer.buffer = (char*)malloc((fileSize + 1) * sizeof(char));
	fread(lexer.buffer, fileSize, sizeof(char), file);
	fclose(file);

	lexer.buffer[fileSize] = '\0';
	lexer.length = fileSize;
	lexer.current = lexer.buffer;
	return lexer;
}

void _destroyLexer(
	struct _Lexer* lexer)
{
	assert(lexer != NULL);
	free(lexer->buffer);
	lexer->length = 0;
}

static char _current(
	struct _Lexer* const lexer)
{
	assert(lexer != NULL);
	assert(lexer->current != NULL);
	return *(lexer->current);
}

static int _isWhitespace(
	struct _Lexer* const lexer)
{
	assert(lexer != NULL);
	assert(lexer->current != NULL);
	return _current(lexer) == 13
		|| _current(lexer) == 10
		|| _current(lexer) == ' '
		|| _current(lexer) == '\t'
		|| _current(lexer) == '\n';
}

static void _moveBy(
	struct _Lexer* const lexer,
	const unsigned int offset)
{
	assert(lexer != NULL);
	assert(lexer->current != NULL);

	for (unsigned int counter = 0; counter < offset; ++counter)
	{
		++lexer->location.column;

		if (_current(lexer) == '\n')
		{
			++lexer->location.line;
			lexer->location.column = 1;
		}

		++lexer->current;
	}
}

static void _skipWhitespaces(
	struct _Lexer* const lexer)
{
	assert(lexer != NULL);
	assert(lexer->current != NULL);

	while (_isWhitespace(lexer))
	{
		_moveBy(lexer, 1);
	}
}

static char _peekBy(
	struct _Lexer* const lexer,
	const unsigned int offset)
{
	assert(lexer != NULL);
	assert(lexer->current != NULL);
	return *(lexer->current + offset);
}

static void _skipSingleLineComments(
	struct _Lexer* const lexer)
{
	assert(lexer != NULL);
	assert(lexer->current != NULL);

	while (_current(lexer) == '/' && _peekBy(lexer, 1) == '/')
	{
		for (; _current(lexer) != '\n' && _current(lexer) != '\0';)
		{
			_moveBy(lexer, 1);
		}

		if (_current(lexer) == '\n')
		{
			_moveBy(lexer, 1);
		}

		_skipWhitespaces(lexer);
	}
}

static void _tryCreateMetaToken(
	struct _Lexer* const lexer,
	struct _Token* const token,
	const enum _TokenType type,
	const enum _TokenStorage storage,
	const unsigned int offset)
{
	assert(lexer != NULL);
	assert(token != NULL);
	*token = _metaToken(type, storage, lexer->location);
	_moveBy(lexer, offset);
}

static void _tryCreateInvalidToken(
	struct _Lexer* const lexer,
	struct _Token* const token,
	const unsigned int offset)
{
	assert(lexer != NULL);
	assert(token != NULL);
	*token = _metaToken(TOKEN_INVALID, STORAGE_NONE, lexer->location);
	_moveBy(lexer, offset);
}

static int _tryCreateIdentifierOrKeywordToken(
	struct _Lexer* const lexer,
	struct _Token* const token)
{
	assert(lexer != NULL);
	assert(lexer->current != NULL);
	assert(token != NULL);

	if (!isalpha(_current(lexer)))
	{
		return 0;
	}

	unsigned int length = 0;
	for (; isalpha(_peekBy(lexer, length)) || isdigit(_peekBy(lexer, length)); ++length);

	static_assert(TOKEN_TYPES_COUNT == 65, "TOKEN_TYPES_COUNT is higher than accounted in _tryCreateIdentifierOrKeywordToken!");
	#define KEYWORDS_COUNT 18
	static const char* const keywords[KEYWORDS_COUNT] =
	{ "if", "import", "export", "proc", "while", "return", "var",
	  "char", "int8", "uint8", "int16", "uint16", "int32", "uint32",
	  "int64", "uint64", "float32", "float64" };
	static const enum _TokenType types[KEYWORDS_COUNT] =
	{ TOKEN_IF_KEYWORD, TOKEN_IMPORT_KEYWORD, TOKEN_EXPORT_KEYWORD, TOKEN_PROC_KEYWORD, TOKEN_WHILE_KEYWORD, TOKEN_RETURN_KEYWORD, TOKEN_VAR_KEYWORD,
	  TOKEN_CHAR_KEYWORD, TOKEN_INT8_KEYWORD, TOKEN_UINT8_KEYWORD, TOKEN_INT16_KEYWORD, TOKEN_UINT16_KEYWORD, TOKEN_INT32_KEYWORD, TOKEN_UINT32_KEYWORD,
	  TOKEN_INT64_KEYWORD, TOKEN_UINT64_KEYWORD, TOKEN_FLOAT32_KEYWORD, TOKEN_FLOAT64_KEYWORD };

	for (unsigned int index = 0; index < KEYWORDS_COUNT; ++index)
	{
		if (strncmp(lexer->current, keywords[index], length) == 0)
		{
			*token = _keywordToken(lexer->current, length, types[index], lexer->location);
			_moveBy(lexer, length);
			return length;
		}
	}
	#undef KEYWORDS_COUNT

	*token = _identifierToken(lexer->current, length, lexer->location);
	_moveBy(lexer, length);
	return length;
}

/**
 * TODO: rework and optimize this function for numeric literals!
 */
static int _tryCreateLiteralToken(
	struct _Lexer* const lexer,
	struct _Token* const token)
{
	assert(lexer != NULL);
	assert(lexer->current != NULL);
	assert(token != NULL);

	if (_current(lexer) == '\'')
	{
		if (_peekBy(lexer, 2) == '\'')
		{
			_moveBy(lexer, 1);
			*token = _charLiteralToken(lexer->current, 1, lexer->location);
			_moveBy(lexer, 2);
			return TOKEN_CHAR_LITERAL;
		}
		else
		{
			return 0;
		}
	}
	else if (isdigit(_current(lexer)) || (_current(lexer) == '-' && isdigit(_peekBy(lexer, 1))))
	{
		int dotsCount = 0;
		unsigned int length = 0;

		if (_current(lexer) == '-')
		{
			++length;
		}

		for (; isdigit(_peekBy(lexer, length)) || _peekBy(lexer, length) == '.'; ++length)
		{
			if (_peekBy(lexer, length) == '.') ++dotsCount;
		}

		if (dotsCount > 1)
		{
			fprintf(stderr, "ERROR: faiure while parsing numeric literal!\n");
			exit(1);
		}
		else if (dotsCount == 1)
		{
			*token = _float32LiteralToken(lexer->current, length, lexer->location);
			_moveBy(lexer, length);
			return TOKEN_FLOAT32_LITERAL;
		}
		else
		{
			*token = _int32LiteralToken(lexer->current, length, lexer->location);
			_moveBy(lexer, length);
			return TOKEN_INT32_LITERAL;
		}

		return 0;
	}
	else if (_current(lexer) == '\"')
	{
		_moveBy(lexer, 1);
		unsigned int length = 0;
		for (; _peekBy(lexer, length) != '\"'; ++length);
		*token = _stringLiteralToken(lexer->current, length, lexer->location);
		_moveBy(lexer, length + 1);
		return TOKEN_STRING_LITERAL;
	}
	else
	{
		return 0;
	}
}

void _nextToken(
	struct _Lexer* const lexer,
	struct _Token* const token)
{
	assert(lexer != NULL);
	assert(lexer->current != NULL);
	assert(token != NULL);

	// 1. Handle whitespaces
	_skipWhitespaces(lexer);

	// 2. Handle comments
	_skipSingleLineComments(lexer);

	// 3. Handle identifier token
	if (_tryCreateIdentifierOrKeywordToken(lexer, token) > 0)
	{
		return;
	}

	// 4. Handle literal tokens
	if (_tryCreateLiteralToken(lexer, token) > 0)
	{
		return;
	}

	// 5. Handle all the other tokens
	switch (_current(lexer))
	{
		case '(':
		{
			_tryCreateMetaToken(lexer, token, TOKEN_LEFT_PARENTHESIS, STORAGE_NONE, 1);
		} break;

		case ')':
		{
			_tryCreateMetaToken(lexer, token, TOKEN_RIGHT_PARENTHESIS, STORAGE_NONE, 1);
		} break;

		case '[':
		{
			_tryCreateMetaToken(lexer, token, TOKEN_LEFT_BRACKET, STORAGE_NONE, 1);
		} break;

		case ']':
		{
			_tryCreateMetaToken(lexer, token, TOKEN_RIGHT_BRACKET, STORAGE_NONE, 1);
		} break;

		case '{':
		{
			_tryCreateMetaToken(lexer, token, TOKEN_LEFT_BRACE, STORAGE_NONE, 1);
		} break;

		case '}':
		{
			_tryCreateMetaToken(lexer, token, TOKEN_RIGHT_BRACE, STORAGE_NONE, 1);
		} break;

		case ';':
		{
			_tryCreateMetaToken(lexer, token, TOKEN_SEMICOLON, STORAGE_NONE, 1);
		} break;

		case ':':
		{
			_tryCreateMetaToken(lexer, token, TOKEN_COLON, STORAGE_NONE, 1);
		} break;

		case ',':
		{
			_tryCreateMetaToken(lexer, token, TOKEN_COMMA, STORAGE_NONE, 1);
		} break;

		case '&':
		{
			switch (_peekBy(lexer, 1))
			{
				case '&':
				{
					_tryCreateMetaToken(lexer, token, TOKEN_LOGICAL_AND, STORAGE_NONE, 2);
				} break;

				default:
				{
					_tryCreateMetaToken(lexer, token, TOKEN_BITWISE_AND, STORAGE_NONE, 1);
				} break;
			}
		} break;

		case '|':
		{
			switch (_peekBy(lexer, 1))
			{
				case '|':
				{
					_tryCreateMetaToken(lexer, token, TOKEN_LOGICAL_OR, STORAGE_NONE, 2);
				} break;

				default:
				{
					_tryCreateMetaToken(lexer, token, TOKEN_BITWISE_OR, STORAGE_NONE, 1);
				} break;
			}
		} break;

		case '~':
		{
			_tryCreateMetaToken(lexer, token, TOKEN_BITWISE_NOT, STORAGE_NONE, 1);
		} break;

		case '!':
		{
			switch (_peekBy(lexer, 1))
			{
				case '=':
				{
					_tryCreateMetaToken(lexer, token, TOKEN_NOT_EQUAL, STORAGE_NONE, 2);
				} break;

				default:
				{
					_tryCreateMetaToken(lexer, token, TOKEN_LOGICAL_NOT, STORAGE_NONE, 1);
				} break;
			}
		} break;

		case '=':
		{
			switch (_peekBy(lexer, 1))
			{
				case '=':
				{
					_tryCreateMetaToken(lexer, token, TOKEN_EQUAL, STORAGE_NONE, 2);
				} break;

				default:
				{
					_tryCreateMetaToken(lexer, token, TOKEN_ASSIGNMENT, STORAGE_NONE, 1);
				} break;
			}
		} break;

		case '<':
		{
			switch (_peekBy(lexer, 1))
			{
				case '=':
				{
					_tryCreateMetaToken(lexer, token, TOKEN_LESS_THAN_OR_EQUAL, STORAGE_NONE, 2);
				} break;

				default:
				{
					_tryCreateMetaToken(lexer, token, TOKEN_LESS_THAN, STORAGE_NONE, 1);
				} break;
			}
		} break;

		case '>':
		{
			switch (_peekBy(lexer, 1))
			{
				case '=':
				{
					_tryCreateMetaToken(lexer, token, TOKEN_GREATER_THAN_OR_EQUAL, STORAGE_NONE, 2);
				} break;

				default:
				{
					_tryCreateMetaToken(lexer, token, TOKEN_GREATER_THAN, STORAGE_NONE, 1);
				} break;
			}
		} break;

		case '+':
		{
			switch (_peekBy(lexer, 1))
			{
				case '=':
				{
					_tryCreateMetaToken(lexer, token, TOKEN_PLUS_EQUAL, STORAGE_NONE, 2);
				} break;

				default:
				{
					_tryCreateMetaToken(lexer, token, TOKEN_PLUS, STORAGE_NONE, 1);
				} break;
			}
		} break;

		case '-':
		{
			switch (_peekBy(lexer, 1))
			{
				case '=':
				{
					_tryCreateMetaToken(lexer, token, TOKEN_MINUS_EQUAL, STORAGE_NONE, 2);
				} break;

				default:
				{
					_tryCreateMetaToken(lexer, token, TOKEN_MINUS, STORAGE_NONE, 1);
				} break;
			}
		} break;

		case '*':
		{
			switch (_peekBy(lexer, 1))
			{
				case '=':
				{
					_tryCreateMetaToken(lexer, token, TOKEN_MULTIPLY_EQUAL, STORAGE_NONE, 2);
				} break;

				default:
				{
					_tryCreateMetaToken(lexer, token, TOKEN_MULTIPLY, STORAGE_NONE, 1);
				} break;
			}
		} break;

		case '/':
		{
			switch (_peekBy(lexer, 1))
			{
				case '=':
				{
					_tryCreateMetaToken(lexer, token, TOKEN_DIVIDE_EQUAL, STORAGE_NONE, 2);
				} break;

				default:
				{
					_tryCreateMetaToken(lexer, token, TOKEN_DIVIDE, STORAGE_NONE, 1);
				} break;
			}
		} break;

		case '%':
		{
			switch (_peekBy(lexer, 1))
			{
				case '=':
				{
					_tryCreateMetaToken(lexer, token, TOKEN_MODULUS_EQUAL, STORAGE_NONE, 2);
				} break;

				default:
				{
					_tryCreateMetaToken(lexer, token, TOKEN_MODULUS, STORAGE_NONE, 1);
				} break;
			}
		} break;

		case '\0':
		{
			_tryCreateMetaToken(lexer, token, TOKEN_END_OF_FILE, STORAGE_NONE, 1);
		} break;

		default:
		{
			_tryCreateInvalidToken(lexer, token, 1);
			fprintf(stderr, "ERROR: unknown symbol `%c` at location %s! Cannot tokenize!\n", *lexer->current, _stringifyLocation(&lexer->location));
		} break;
	}
}

void _peekToken(
	struct _Lexer* const lexer,
	struct _Token* const token,
	const unsigned int offset)
{
	assert(lexer != NULL);
	assert(lexer->current != NULL);
	assert(token != NULL);

	const char* current = lexer->current;
	struct _Location location = lexer->location;

	_nextToken(lexer, token);

	for (unsigned int index = 0; index < offset; ++index)
	{
		_destroyToken(token);
		_nextToken(lexer, token);
	}

	lexer->current = current;
	lexer->location = location;
}

enum _TokenType _expectToken(
	struct _Lexer* const lexer,
	struct _Token* const token,
	const enum _TokenType type,
	const int strict)
{
	_nextToken(lexer, token);

	if (token->type != type)
	{
		fprintf(stderr, "ERROR: expected token of type %s, but parsed %s!\n",
			_stringifyTokenType(type), _stringifyTokenType(token->type));

		if (strict)
		{
			exit(1);
		}
	}

	return token->type;
}
