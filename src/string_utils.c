#include <string.h>
#include "whiskey_private.h"

#ifdef __GNUC__
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wformat-nonliteral"
#elif __clang__
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wformat-nonliteral"
#endif

static inline int wsky_vsprintf(char *str, const char *format, va_list ap) {
  return vsprintf(str, format, ap);
}

static inline int wsky_vsnprintf(char *str, size_t size,
                                 const char *format,
                                 va_list ap) {
  return vsnprintf(str, size, format, ap);
}

#ifdef __GNUC__
# pragma GCC diagnostic pop
#elif __clang__
# pragma clang diagnostic pop
#endif

char *wsky_asprintf(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);

  va_list copy;
  va_copy(copy, ap);
  int length = wsky_vsnprintf(NULL, 0, fmt, copy);
  va_end(copy);
  if (length < 0)
    return NULL;
  char *ptr = wsky_safeMalloc((size_t)length + 1);
  if (!ptr)
    return NULL;
  int out = wsky_vsprintf(ptr, fmt, ap);
  if (out < 0) {
    wsky_free(ptr);
    return NULL;
  }
  va_end(ap);
  return ptr;
}

char *wsky_strdup(const char *string) {
  size_t length = strlen(string);
  char *newString = wsky_malloc(length + 1);
  if (!newString)
    return NULL;
  strcpy(newString, string);
  return newString;
}

char *wsky_strndup(const char *string, size_t maximum) {
  size_t length = strlen(string);
  if (maximum < length)
    length = maximum;
  char *newString = wsky_malloc(length + 1);
  if (!newString)
    return NULL;
  strncpy(newString, string, length);
  newString[length] = '\0';
  return newString;
}
