#ifndef _LOCATION_H_
#define _LOCATION_H_

/**
 * A structure that defines a location of a character or beginning of a word or
 * a sequence in a file. It holds a file name, a line, and a column number.
 */
struct _Location
{
	const char* file;
	unsigned int line;
	unsigned int column;
};

/**
 * Stringify a location structure to the format 'file=%s, line=%d, column=%d'.
 * It works with a static char buffer with max size of 256 chars.
 * 
 * NOTE: it cuts the out-of-bounds part of the string and returns a stripped
 * version if the whole string is longer than the max capacity of the buffer.
 */
const char* _stringifyLocation(
	const struct _Location* const location);

#endif
