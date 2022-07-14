#include "../include/cflag.h"
#include "../include/parser.h"

#include <stdio.h>

int main(int argc, char** argv)
{
	/*
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
	*/

	const char* sourceFlag = "D:\\Home\\projects\\corth\\examples\\example1.corth";
	_parse(sourceFlag);

	return 0;
}
