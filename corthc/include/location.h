#ifndef _LOCATION_H_
#define _LOCATION_H_

struct _Location
{
	const char* file;
	unsigned int line;
	unsigned int column;
};

const char* _stringifyLocation(
	const struct _Location* const location);

#endif
