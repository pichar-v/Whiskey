#ifndef DICT_H_
# define DICT_H_

#include <stdbool.h>

typedef struct wsky_DictEntry_s wsky_DictEntry;

typedef struct wsky_Dict_s {
  wsky_DictEntry *first;
} wsky_Dict;

void wsky_Dict_init(wsky_Dict *this);
wsky_Dict *wsky_Dict_new(void);

void wsky_Dict_free(wsky_Dict *this);
void wsky_Dict_delete(wsky_Dict *this);

void wsky_Dict_apply(wsky_Dict *this,
                     void (*function)(const char *key, void *value));
bool wsky_Dict_contains(const wsky_Dict *this, const char *key);
void wsky_Dict_set(wsky_Dict *this, const char *key, void *value);
void *wsky_Dict_get(wsky_Dict *this, const char *key);
void *wsky_Dict_remove(wsky_Dict *this, const char *key);

#endif /* !DICT_H_ */