#ifndef _META_H_
#define _META_H_

/**
 * @addtogroup META
 * 
 * @{
 */

/**
 * Stringify an expression.
 */
#ifndef STRINGIFY
#	define STRINGIFY(expression) #expression
#endif

/**
 * Stringify an enum element and set as array value by element's index.
 */
#ifndef STRINGIFY_ENUM
#	define STRINGIFY_ENUM(element) [element] = #element
#endif

/**
 * @}
 */

#endif
