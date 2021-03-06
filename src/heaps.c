#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include "heaps.h"

#define wsky_HEAPS_LOGGING 0

#if wsky_HEAPS_LOGGING == 1
static void heapsLog(const char *string, ...) {
  va_list list;
  va_start(list, string);
  fprintf(stderr, "heaps: ");
  vfprintf(stderr, string, list);
  va_end(list);
}
#else
static inline void heapsLog(const char *string, ...) {
  (void)string;
}
#endif

typedef union ObjectUnion_u {

  struct {
    wsky_OBJECT_HEAD
    union ObjectUnion_u *next;
   } free;

  AttributeError        attributeError;
  Class                 class;
  Exception             exception;
  Function              function;
  ImportError           importError;
  InstanceMethod        instanceMethod;
  Method                method;
  Module                module;
  NameError             nameError;
  NotImplementedError   notImplementedError;
  Object                object;
  ParameterError        parameterError;
  ProgramFile           programFile;
  Scope                 scope;
  String                string;
  Structure             structure;
  SyntaxErrorEx         syntaxError;
  TypeError             typeError;
  ValueError            valueError;
  ZeroDivisionError     zeroDivisionError;

} ObjectUnion;

static inline bool ObjectUnion_isFree(const ObjectUnion *u) {
  return u->free.class == NULL;
}

static inline void ObjectUnion_markAsFree(ObjectUnion *u) {
  u->free.class = NULL;
}

static void deleteObject(Object *object) {
  wsky_Class *class = object->class;
  assert(class);
  heapsLog("Destroying a %s at %p\n", class->name, (void *) object);
  while (class != wsky_Object_CLASS) {
    if (class->destructor)
      class->destructor(object);
    /*
    else
      printf("No destructor\n");
    */
    class = class->super;
  }

  if (!object->class->native) {
    wsky_ObjectFields_free(&object->fields);
  }

  wsky_heaps_freeObject(object);
}

static void ObjectUnion_delete(ObjectUnion *object) {
  assert(!ObjectUnion_isFree(object));
  assert(!object->object._gcMark);
  deleteObject(&object->object);
}



typedef struct Heap_s {

  ObjectUnion   *objects;
  size_t        count;
  struct Heap_s *next;

} Heap;


#define INITIAL_HEAP_SIZE 8


static void heaps_addToFreeObjectList(ObjectUnion *object);


static void Heap_init(Heap *heap, size_t heapSize, Heap *next) {
  heap->objects = wsky_safeMalloc(heapSize * sizeof(ObjectUnion));
  heap->count = heapSize;
  for (size_t i = 0; i < heapSize; i++) {
    ObjectUnion *object = heap->objects + i;
    ObjectUnion_markAsFree(object);
    heaps_addToFreeObjectList(object);
  }
  heap->next = next;
}

static Heap *Heap_new(size_t objectCount, Heap *next) {
  Heap *heap = wsky_safeMalloc(sizeof(Heap));
  Heap_init(heap, objectCount, next);
  return heap;
}

static bool Heap_areAllObjectsFreed(const Heap *heap) {
  for (size_t i = 0; i < heap->count; i++) {
    ObjectUnion *object = heap->objects + i;
    if (ObjectUnion_isFree(object))
      return true;
  }
  return false;
}

static void Heap_unmark(Heap *heap) {
  for (size_t i = 0; i < heap->count; i++) {
    ObjectUnion *object = heap->objects + i;
    if (!ObjectUnion_isFree(object))
      object->object._gcMark = false;
  }
}

static void Heap_deleteUnmarkedObjects(Heap *heap) {
  for (size_t i = 0; i < heap->count; i++) {
    ObjectUnion *object = heap->objects + i;
    if (!ObjectUnion_isFree(object) && !object->object._gcMark)
      ObjectUnion_delete(object);
  }
}

static void Heap_delete(Heap *heap) {
  assert(Heap_areAllObjectsFreed(heap));
  wsky_free(heap->objects);
  wsky_free(heap);
}



typedef struct {

  Heap          *heaps;

  size_t        heapSize;

  void          *lowestAddress;
  void          *highestAddress;

  ObjectUnion   *freeObjects;

} Heaps;

static Heaps heaps = {
  .heaps = NULL,

  .lowestAddress = NULL,
  .highestAddress = NULL,

  .heapSize = INITIAL_HEAP_SIZE,

  .freeObjects = NULL,
};

static void heaps_addHeap(void) {
  heapsLog("Add heap of size %lu\n", (unsigned long)heaps.heapSize);
  Heap *heap = Heap_new(heaps.heapSize, heaps.heaps);
  heaps.heaps = heap;
  heaps.heapSize *= 2;

  if (!heaps.lowestAddress || (void *)heap->objects < heaps.lowestAddress)
    heaps.lowestAddress = heap->objects;

  if (!heaps.highestAddress || (void *)heap->objects > heaps.highestAddress)
    heaps.highestAddress = heap->objects + heap->count - 1;
  assert(heaps.highestAddress > heaps.lowestAddress);
}

static void heaps_addToFreeObjectList(ObjectUnion *object) {
  object->free.next = heaps.freeObjects;
  heaps.freeObjects = object;
}

Object *wsky_heaps_allocateObject(const char *className) {
  if (heaps.freeObjects) {
    ObjectUnion *object = heaps.freeObjects;
    heaps.freeObjects = object->free.next;
    heapsLog("Allocating a %s at %p\n", className, (void *)&object->object);
    return &object->object;
  }

  heaps_addHeap();
  assert(heaps.freeObjects);
  return wsky_heaps_allocateObject(className);
}

void wsky_heaps_freeObject(Object *object_) {
  ObjectUnion *object = (ObjectUnion *)object_;
  ObjectUnion_markAsFree(object);
  heaps_addToFreeObjectList(object);
}

void wsky_heaps_unmark(void) {
  Heap *heap = heaps.heaps;
  while (heap) {
    Heap_unmark(heap);
    heap = heap->next;
  }
}

void wsky_heaps_deleteUnmarkedObjects(void) {
  Heap *heap = heaps.heaps;
  while (heap) {
    Heap *next = heap->next;
    Heap_deleteUnmarkedObjects(heap);
    heap = next;
  }
}


void wsky_heaps_free(void) {
  Heap *heap = heaps.heaps;
  while (heap) {
    Heap *next = heap->next;
    Heap_delete(heap);
    heap = next;
  }
}


static inline bool isAlignedWithHeap(void *pointer, const Heap *heap) {
  char *objects = (char *)heap->objects;
  assert(pointer >= (void *)objects);
  return ((size_t)((char *)pointer - objects) % sizeof(ObjectUnion)) == 0;
}

bool wsky_heaps_contains(void *pointer_) {
  char *pointer = (char *)pointer_;
  if (pointer < (char *)heaps.lowestAddress ||
      pointer > (char *)heaps.highestAddress)
    return false;

  Heap *heap = heaps.heaps;
  while (heap) {
    ObjectUnion *objects = heap->objects;
    if (pointer >= (char *)objects &&
        pointer < (char *)(objects + heap->count)) {
      if (isAlignedWithHeap(pointer_, heap)) {
        if (!ObjectUnion_isFree((ObjectUnion *)pointer_))
          return true;
      }
    }
    heap = heap->next;
  }
  return false;
}
