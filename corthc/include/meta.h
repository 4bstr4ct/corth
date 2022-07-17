#ifndef _META_H_
#define _META_H_

#ifndef STRINGIFY
#	define STRINGIFY(expression) #expression
#endif

#ifndef STRINGIFY_ENUM
#	define STRINGIFY_ENUM(element) [element] = #element
#endif

#endif
