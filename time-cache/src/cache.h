#ifndef CACHE_H
#define CACHE_H

#include <glib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "deque.h"

typedef struct time_cache {
    GHashTable *cache_map;
    Deque *cache;
    size_t capacity;
    size_t ttl;
} TimeCache;

TimeCache* create_timecache(size_t capacity, size_t ttl);
void free_timecache(TimeCache *time_cache);
size_t timecache_size(TimeCache *time_cache);
int** timecache_items(TimeCache *time_cache);
bool add_item_to_timecache(TimeCache *time_cache, int cache_item);
size_t remove_expired_items_from_timecache(TimeCache *time_cache);
void list_timecache_info(TimeCache *time_cache);

#endif
