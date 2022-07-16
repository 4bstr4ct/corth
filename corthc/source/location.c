#include "../include/location.h"

#include <stdio.h>

const char* _stringifyLocation(
	const struct _Location* const location)
{
	#define MAX_LENGTH 128
	static char buffer[MAX_LENGTH];
	const unsigned int length =
		snprintf(buffer, MAX_LENGTH, "file=%s, line=%d, column=%d", location->file, location->line, location->column);
	#undef MAX_LENGTH

	if (length > 0)
	{
		buffer[length] = '\0';
		return buffer;
	}

	return "UNSTRINGIFIED LOCATION";
}
