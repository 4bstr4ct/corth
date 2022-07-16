#include "../include/cflag.h"
#include "../include/location.h"
#include "../include/token.h"
#include "../include/lexer.h"
#include "../include/parser.h"

#include <stdio.h>

int main(int argc, char** argv)
{
	const char* outputFile = _stringFlag(
		"--output", "--output <path>", "output.nasm",
		"Set output file path");

	const char* outputFormat = _stringFlag(
		"--format", "--format <nasm-linux-x86_64|nasm-freebsd-x86_64>", "nasm",
		"Set output file path");

	const char* sourceFile = _stringFlag(
		"--source", "--source <path>", "",
		"Set source file path");

	--argc; ++argv;
	_parseFlags(argc, argv, PARSE_LENIENT, "corthc [OPTIONS] source=<path>");

	fprintf(stdout, "%s\n", outputFile);
	fprintf(stdout, "%s\n", outputFormat);
	fprintf(stdout, "%s\n", sourceFile);

	// const char* sourceFlag = "D:\\Home\\projects\\corth\\examples\\example1.corth";
	_parse(sourceFile);

	return 0;
}
