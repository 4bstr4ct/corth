#ifndef _CFLAG_H_
#define _CFLAG_H_

/**
 * A typedef for boolean.
 */
typedef enum { false, true } bool;

/**
 * A typedef for signed long long.
 */
typedef signed long long int64;

/**
 * Function _boolFlag() defines a boolean flag with specified name, default
 * value, and description string. It returns an address of a @ref bool variable
 * that stores the value of the flag.
 */
bool* _boolFlag(const char* name, const char* usage, const bool defaultValue, const char* description);

/**
 * Function _int64Flag() defines a 64-bit signed long long flag with specified
 * name, default value, and description string. It returns an address of an @ref int64
 * variable that stores the value of the flag.
 */
int64* _int64Flag(const char* name, const char* usage, const int64 defaultValue, const char* description);

/**
 * Function _doubleFlag() defines a double flag with specified name, default
 * value, and description string. It returns an address of a double variable
 * that stores the value of the flag.
 */
double* _doubleFlag(const char* name, const char* usage, const double defaultValue, const char* description);

/**
 * Function _stringFlag() defines a string flag with specified name, default
 * value, and description string. It returns an address of a cstring's first
 * element (pointer to char array) that stores the value of the flag.
 */
const char* _stringFlag(const char* name, const char* usage, const char* const defaultValue, const char* description);

/**
 * Parsing flag to determine error level and behavior when parsing the arguments.
 * The strict parsing method will throw errors and complain if there are unknown
 * flags/arguments. The lenient option will overlook all the errors listed above
 * and focus only on trying to assign flags' values if they are present in
 * command-line arguments list.
 */
enum _ParseOption
{
	PARSE_STRICT,
	PARSE_LENIENT
};

/**
 * Function _parseFlags() emits first command-line arguments from provided argv
 * array and the rest of the arguments to flags that are stored in flags cache.
 * It must be called after all flags are defined and before flags are accessed
 * by the program. 
 */
void _parseFlags(int argc, char** argv, const enum _ParseOption option, const char* const usage);

#endif
