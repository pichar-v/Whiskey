#include "../whiskey_private.h"


static Result construct(Object *object,
                             unsigned paramCount,
                             const Value *params);
static Result destroy(Object *object);

static void acceptGC(Object *object);

static Result toString(InstanceMethod *object);


#define M(name, flags, paramCount)                      \
  {#name, paramCount, flags, (wsky_Method0)&name}

static MethodDef methods[] = {
  M(toString, wsky_MethodFlags_GET | wsky_MethodFlags_PUBLIC, 0),
  {0, 0, 0, 0},
};

#undef M

const ClassDef wsky_InstanceMethod_CLASS_DEF = {
  .super = &wsky_Object_CLASS_DEF,
  .name = "InstanceMethod",
  .final = true,
  .constructor = &construct,
  .privateConstructor = true,
  .destructor = &destroy,
  .methodDefs = methods,
  .gcAcceptFunction = &acceptGC,
};

Class *wsky_InstanceMethod_CLASS;


InstanceMethod *wsky_InstanceMethod_new(Method *method, Value self) {
  Result r = wsky_Object_new(wsky_InstanceMethod_CLASS, 0, NULL);
  if (r.exception)
    return NULL;
  InstanceMethod *instanceMethod = (InstanceMethod *) r.v.v.objectValue;
  instanceMethod->method = method;
  instanceMethod->self = self;

  return instanceMethod;
}

static Result construct(Object *object,
                             unsigned paramCount,
                             const Value *params) {
  (void) paramCount;
  (void) params;

  InstanceMethod *self = (InstanceMethod *) object;
  self->method = NULL;
  self->self = Value_NULL;
  RETURN_NULL;
}

static Result destroy(Object *object) {
  (void) object;
  RETURN_NULL;
}

static void acceptGC(Object *object) {
  InstanceMethod *self = (InstanceMethod *) object;
  wsky_GC_visitObject(self->method);
  wsky_GC_visitValue(self->self);
}



static Result toString(InstanceMethod *object) {
  (void) object;
  RETURN_C_STRING("<InstanceMethod>");
}

bool wsky_isInstanceMethod(Value value) {
  return wsky_getClass(value) == wsky_InstanceMethod_CLASS;
}
