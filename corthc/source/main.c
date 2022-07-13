#include "../include/cflag.h"
#include "../include/lexer.h"

#include <stdio.h>

int main(int argc, char** argv)
{
	const char* outputFlag = _stringFlag(
		"--output", "--output <path>", "main.out",
		"Set output file path");

	const char* sourceFlag = _stringFlag(
		"--source", "--source <path>", "",
		"Set source file path");

	--argc; ++argv;
	_parseFlags(argc, argv, PARSE_STRICT, "corth [OPTIONS] --source <path>");

	fprintf(stdout, "%s\n", outputFlag);
	fprintf(stdout, "%s\n", sourceFlag);

	// const char* sourceFlag = "D:\\Home\\projects\\corth\\examples\\example1.corth";
	struct _Lexer lexer = _createLexer(sourceFlag);
	struct _Token token;

	do
	{
		_nextToken(&lexer, &token);
		fprintf(stdout, "%s\n", _stringifyToken(&token));
	} while (token.type != TOKEN_END_OF_FILE);

	return 0;
}
