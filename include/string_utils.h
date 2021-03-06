#ifndef STRING_UTILS_H_
# define STRING_UTILS_H_

# include <stddef.h>
# include <stdarg.h>

/**
 * @defgroup whiskey whiskey
 * @{
 */

/**
 * Like asprintf(), except that it returns the pointer instead of a parameter.
 * Remember to free() the pointer returned by this function.
*/
char *wsky_asprintf(const char *fmt, ...)
  __attribute__ ((format(printf, 1, 2)));

/** Like strdup() */
char *wsky_strdup(const char *string);

/** Like strndup() */
char *wsky_strndup(const char *string, size_t maximum);

/**
 * @}
 */

#endif /* !STRING_UTILS_H_ */
