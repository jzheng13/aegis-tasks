#ifndef CACHE_H
#define CACHE_H

#include <glib.h>

#include "deque.h"

typedef struct time_cache {
    GHashTable* cache_map;
    Deque* cache;
    size_t capacity;
    size_t ttl;
} TimeCache;

TimeCache create_timecache(size_t capacity, size_t ttl);

char** remove_expired_items(TimeCache time_cache);



#endif