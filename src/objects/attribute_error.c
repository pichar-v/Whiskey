#include <string.h>
#include "../whiskey_private.h"


static Result construct(Object *object,
                             unsigned paramCount,
                             const Value *params);

static Result destroy(Object *object);



static MethodDef methods[] = {
  {0, 0, 0, 0},
};

const ClassDef wsky_AttributeError_CLASS_DEF = {
  .super = &wsky_Exception_CLASS_DEF,
  .name = "AttributeError",
  .final = false,
  .constructor = &construct,
  .privateConstructor = false,
  .destructor = &destroy,
  .methodDefs = methods,
  .gcAcceptFunction = NULL,
};

Class *wsky_AttributeError_CLASS;



AttributeError *wsky_AttributeError_new(const char *message) {
  Value v = wsky_buildValue("s", message);
  Result r;
  r = wsky_Object_new(wsky_AttributeError_CLASS, 1, &v);
  if (r.exception)
    abort();
  return (AttributeError *) r.v.v.objectValue;
}

AttributeError *wsky_AttributeError_newNoAttr(const char *className,
                                              const char *attribute) {
  char *message = wsky_asprintf("'%s' object has no attribute '%s'",
           className, attribute);
  AttributeError *e = wsky_AttributeError_new(message);
  free(message);
  return e;
}


Result wsky_AttributeError_raiseNoAttr(const char *className,
                                            const char *name) {
  Exception *e = (Exception *)wsky_AttributeError_newNoAttr(className, name);
  RAISE_EXCEPTION(e);
}


static Result construct(wsky_Object *object,
                             unsigned paramCount,
                             const Value *params) {
  wsky_Exception_CLASS_DEF.constructor(object, paramCount, params);
  RETURN_NULL;
}

static Result destroy(wsky_Object *object) {
  (void) object;
  RETURN_NULL;
}
