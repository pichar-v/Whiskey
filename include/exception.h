#ifndef EXCEPTION_H_
# define EXCEPTION_H_

# include "object.h"


extern wsky_Class wsky_Exception_CLASS;

void wsky_String_initClass(void);
void wsky_String_freeClass(void);



# define wsky_Exception_HEAD                    \
  wsky_OBJECT_HEAD                              \
  char *message;                                \
  wsky_Exception *cause;

struct wsky_Exception_s {
  wsky_Exception_HEAD
};



wsky_Exception *wsky_Exception_new(const char *message,
                                   wsky_Exception *cause);
wsky_Exception *wsky_Exception_construct(wsky_Object *object,
                                         unsigned paramCount,
                                         wsky_Value *params);

void wsky_Exception_destroy(wsky_Object *this);



void wsky_Exception_print(const wsky_Exception *this);

#endif /* !EXCEPTION_H_ */
