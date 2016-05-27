#ifndef _STRMAP_H_
#define _STRMAP_H_

#include <stdlib.h>
#include <string.h>

typedef struct StrMap StrMap;
typedef void(*sm_enum_func)(const char*key, const char *value, const void *obj);
StrMap *sm_new(unsigned int capacity);
void sm_delete(StrMap *map);
int sm_get(const StrMap *map, const char *key, char *out_buf, unsigned int n_out_buf);
int sm_exists(const StrMap *map, const char *key);
int sm_put(StrMap *map, const char *key, const char *value);
int sm_get_count(const StrMap *map);
int sm_enum(const StrMap *map, sm_enum_func enum_func, const void *obj);

#endif