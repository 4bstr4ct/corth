#include "../include/cflag.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

enum _FlagType
{
	FLAG_BOOL,
	FLAG_INT64,
	FLAG_DOUBLE,
	FLAG_STRING
};

#ifndef STRING_FLAG_CAPACITY
#	define STRING_FLAG_CAPACITY 128
#endif

union _FlagValue
{
	bool b;
	int64 i64;
	double d;
	char s[STRING_FLAG_CAPACITY + 1];
};

struct _Flag
{
	const char* name;
	const char* usage;
	enum _FlagType type;
	union _FlagValue value;
	const char* description;
};

#ifndef FLAGS_CACHE_CAPACITY
#	define FLAGS_CACHE_CAPACITY 128
#endif

struct _FlagsCache
{
	struct _Flag flags[FLAGS_CACHE_CAPACITY];
	unsigned int count;
};

#ifndef HELP_BUFFER_CAPACITY
#	define HELP_BUFFER_CAPACITY 1024
#endif

struct _HelpBuffer
{
	char buffer[1024];
	unsigned int length;
};

static struct _FlagsCache _flagsCache = { .count = 0 };
static struct _HelpBuffer _helpBuffer = { .length = 0 };

static const char* const _stringifiedFlagTypes[] =
{
	[FLAG_BOOL]		= "bool",
	[FLAG_INT64]	= "int64",
	[FLAG_DOUBLE]	= "double",
	[FLAG_STRING]	= "string"
};

static const char* _formatHelp(const char* const usage)
{
	_helpBuffer.length = 0;
	_helpBuffer.length += snprintf(
			_helpBuffer.buffer + _helpBuffer.length, HELP_BUFFER_CAPACITY,
			"Usage: %s\nOptions:\n", usage);

	for (unsigned int index = 0; index < _flagsCache.count; ++index)
	{
		const struct _Flag* const flag = &_flagsCache.flags[index];
		unsigned int written = snprintf(
			_helpBuffer.buffer + _helpBuffer.length,
			HELP_BUFFER_CAPACITY,
			"    %s\n        Usage: %s\n        [ %s ] - %s\n\n",
			flag->name,
			flag->usage,
			_stringifiedFlagTypes[flag->type],
			flag->description);
		_helpBuffer.length += written;
	}

	_helpBuffer.buffer[_helpBuffer.length] = '\0';
	return _helpBuffer.buffer;
}

static void _printUsage(FILE* const stream)
{
	fprintf(stream, "%s", _helpBuffer.buffer);
}

static struct _Flag* const _metaFlag(const char* name, const char* usage, const char* description)
{
	assert(_flagsCache.count < FLAGS_CACHE_CAPACITY);
	struct _Flag* const flag = &_flagsCache.flags[_flagsCache.count++];
	flag->name = name;
	flag->usage = usage;
	flag->description = description;
	return flag;
}

bool* _boolFlag(const char* name, const char* usage, const bool defaultValue, const char* description)
{
	struct _Flag* const flag = _metaFlag(name, usage, description);
	flag->type = FLAG_BOOL;
	flag->value.b = defaultValue;
	return &flag->value.b;
}

int64* _int64Flag(const char* name, const char* usage, const int64 defaultValue, const char* description)
{
	struct _Flag* const flag = _metaFlag(name, usage, description);
	flag->type = FLAG_INT64;
	flag->value.i64 = defaultValue;
	return &flag->value.i64;
}

double* _doubleFlag(const char* name, const char* usage, const double defaultValue, const char* description)
{
	struct _Flag* const flag = _metaFlag(name, usage, description);
	flag->type = FLAG_DOUBLE;
	flag->value.d = defaultValue;
	return &flag->value.d;
}

const char* _stringFlag(const char* name, const char* usage, const char* const defaultValue, const char* description)
{
	struct _Flag* const flag = _metaFlag(name, usage, description);
	flag->type = FLAG_STRING;
	unsigned int length = strlen(defaultValue);
	length = length >= STRING_FLAG_CAPACITY ? STRING_FLAG_CAPACITY : length;
	memcpy(flag->value.s, defaultValue, length);
	flag->value.s[length] = '\0';
	return (const char*)(flag->value.s);
}

static const char* _shift(int* argc, char*** argv)
{
	const char* current = ((void*)0);

	if (argc > 0)
	{
		current = **argv;
		*argc -= 1;
		*argv += 1;
	}

	return current;
}

static int _handleFlag(struct _Flag* const flag, const char* argValue)
{
	switch (flag->type)
	{
		case FLAG_BOOL:
		{
			char value[6];

			if (sscanf(argValue, "%s", value) == 1)
			{
				if (strcmp(value, "true") == 0)
				{
					flag->value.b = true;
					return 1;
				}
				else if (strcmp(value, "false") == 0)
				{
					flag->value.b = false;
					return 1;
				}
				else
				{
					fprintf(stderr, "ERROR: failed to parse boolean value for a flag %s!\n", flag->name);
					exit(1);
				}
			}
			else
			{
				fprintf(stderr, "ERROR: failed to parse value for a flag %s!\n", flag->name);
				exit(1);
			}
		} break;

		case FLAG_INT64:
		{
			int64 value = 0;

			if (sscanf(argValue, "%lld", &value) == 1)
			{
				flag->value.i64 = value;
				return 1;
			}
			else
			{
				fprintf(stderr, "ERROR: failed to parse value for a flag %s!\n", flag->name);
				exit(1);
			}
		} break;

		case FLAG_DOUBLE:
		{
			double value = 0;

			if (sscanf(argValue, "%lf", &value) == 1)
			{
				flag->value.d = value;
				return 1;
			}
			else
			{
				fprintf(stderr, "ERROR: failed to parse value for a flag %s!\n", flag->name);
				exit(1);
			}
		} break;

		case FLAG_STRING:
		{
			if (argValue != ((void*)0))
			{
				unsigned int length = strlen(argValue);
				length = length >= STRING_FLAG_CAPACITY ? STRING_FLAG_CAPACITY : length;
				memcpy(flag->value.s, argValue, length);
				flag->value.s[length] = '\0';
				return 1;
			}
			else
			{
				fprintf(stderr, "ERROR: failed to parse value for a flag %s!\n", flag->name);
				exit(1);
			}
		} break;

		default:
		{
			fprintf(stderr, "ERROR: %s - unknown flag type!\n", _stringifiedFlagTypes[flag->type]);
			exit(1);
		} break;
	}

	return 0;
}

void _parseFlags(int argc, char** argv, const enum _ParseOption option, const char* const usage)
{
	_formatHelp(usage);

	if (option == PARSE_STRICT && argc <= 0)
	{
		fprintf(stderr, "ERROR: no arguments were provided!\n");
		_printUsage(stderr);
		exit(1);
	}

	while (argc > 0)
	{
		const char* arg = _shift(&argc, &argv);

		if (arg == ((void*)0))
		{
			fprintf(stderr, "ERROR: unreachable argument!\n");
			_printUsage(stderr);
			exit(1);
		}

		if (strcmp(arg, "--help") == 0)
		{
			_printUsage(stdout);
			exit(0);
		}

		int invalidArgumentEncountered = 0;

		for (unsigned int flagIndex = 0; flagIndex < _flagsCache.count; ++flagIndex)
		{
			struct _Flag* const flag = &_flagsCache.flags[flagIndex];

			if (strcmp(flag->name, arg) == 0)
			{
				const char* argValue = _shift(&argc, &argv);

				if (argValue == ((void*)0))
				{
					fprintf(stderr, "ERROR: value was not found for flag %s!\n", flag->name);
					_printUsage(stderr);
					exit(1);
				}

				if (_handleFlag(flag, argValue))
				{
					invalidArgumentEncountered = 0;
					break;
				}
			}
			else
			{
				++invalidArgumentEncountered;
			}
		}

		if (option == PARSE_STRICT && invalidArgumentEncountered >= _flagsCache.count)
		{
			fprintf(stderr, "ERROR: unknown argument `%s` encountered for flags cache!\n", arg);
			_printUsage(stderr);
			exit(1);
		}
	}
}
