#include "objects/instance_method.h"

#include <stdlib.h>
#include "objects/str.h"


typedef wsky_MethodDef MethodDef;
typedef wsky_Object Object;
typedef wsky_Value Value;
typedef wsky_ReturnValue ReturnValue;
typedef wsky_InstanceMethod InstanceMethod;


static wsky_Exception *construct(Object *object,
                                 unsigned paramCount,
                                 Value *params);
static void destroy(Object *object);


static ReturnValue toString(InstanceMethod *object, Value *value);


#define M(name, paramCount)                             \
  {#name, paramCount, (void *) &name}

static wsky_MethodDef methods[] = {
  M(toString, 0),
  {0, 0, 0},
};

#undef M

wsky_Class wsky_InstanceMethod_CLASS = {
  .super = &wsky_Object_CLASS,
  .name = "InstanceMethod",
  .constructor = &construct,
  .destructor = &destroy,
  .objectSize = sizeof(InstanceMethod),
  .methodDefs = methods,
  .gcAcceptFunction = NULL,
};



InstanceMethod *wsky_InstanceMethod_new(const MethodDef *method,
                                        Object *self) {
  ReturnValue r = wsky_Object_new(&wsky_InstanceMethod_CLASS, 0, NULL);
  if (r.exception)
    return NULL;
  InstanceMethod *instanceMethod = (InstanceMethod *) r.v.v.objectValue;
  instanceMethod->method = method;
  instanceMethod->self = self;
  return instanceMethod;
}

static wsky_Exception *construct(Object *object,
                                 unsigned paramCount,
                                 Value *params) {
  (void) paramCount;
  (void) params;

  InstanceMethod *self = (InstanceMethod *) object;
  self->method = NULL;
  self->self = NULL;
  return NULL;
}

static void destroy(Object *object) {
  (void) object;
}



static ReturnValue toString(InstanceMethod *object, Value *value) {
  (void) object;
  (void) value;
  wsky_RETURN_CSTRING("<InstanceMethod>");
}

bool wsky_isInstanceMethod(const Value value) {
  return value.type == wsky_Type_OBJECT &&
    wsky_Value_getClass(value) == &wsky_InstanceMethod_CLASS;
}
