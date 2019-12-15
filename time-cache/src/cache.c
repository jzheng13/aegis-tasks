#include "cache.h"

TimeCache* create_timecache(size_t capacity, size_t ttl)
{
    // Allocate memory for time cache
    TimeCache *time_cache = (TimeCache*) malloc(sizeof(TimeCache));
    if (time_cache == NULL)
        return NULL;

    // Allocate memory for hash map
    GHashTable *cache_map = g_hash_table_new_full(NULL, 
                                                  g_int_equal, 
                                                  free, 
                                                  free);
    if (cache_map == NULL)
    {
        free(time_cache);
        return NULL;
    }

    // Allocate memory for deque
    Deque *deque = create_deque(capacity);
    if (deque == NULL)
    {
        free(time_cache);
        g_hash_table_destroy(cache_map);
        return NULL;
    }

    time_cache->capacity = capacity;
    time_cache->ttl = ttl;

    return time_cache;
}

void free_timecache(TimeCache *time_cache)
{
    if (time_cache != NULL)
    {
        g_hash_table_destroy(time_cache->cache_map);
        free_deque(time_cache->cache);
        free(time_cache);
    }
}

size_t timecache_size(TimeCache *time_cache)
{
    return time_cache->cache->size;
}

int* timecache_items(TimeCache *time_cache)
{
    return time_cache->cache->elements;
}

bool add_item_to_timecache(TimeCache *time_cache, int cache_item) 
{
    // Pop oldest element if cache is at or past capacity
    if (timecache_size(time_cache) >= time_cache->capacity) {
        int oldest_item = peek_deque(time_cache->cache);
        if (oldest_item != NULL)
        {
            g_hash_table_remove(time_cache->cache_map, &oldest_item);
            pop_deque(time_cache->cache);  
        } 
        else 
        {
            // Something wrong
            return false;
        }
        
    }

    // Insert cache item to cache with timestamp of insertion
    int *key = (int*) malloc(sizeof(int));
    int *val = (int*) malloc(sizeof(int));
    if ((key != NULL) && (val != NULL))
    {
        *key = cache_item;
        *val = (int) time(NULL);
        int inserted = g_hash_table_insert(time_cache->cache_map, key, val);
        if (inserted)
        {
            return push_deque(time_cache->cache, cache_item);
        }
        return inserted;
    }
    return false;
}

size_t remove_expired_items_from_timecache(TimeCache *time_cache)
{
    // Counter of expired items
    size_t expired_items_removed = 0;
    // Get current system epoch time
    int current_time = (int) time(NULL);
    //
    for (int i = 0; i++; i <= size)
    {
        // Peek at oldest item and get insertion time
        int oldest = peek_deque(time_cache->cache);
        int *insertion_time = (int *) g_hash_table_lookup(time_cache->cache_map, 
                                                          &oldest);
        if (insertion_time != NULL)
        {
            // Pop item from cache if expired
            if (*insertion_time + time_cache->ttl < current_time)
            {
                if (pop_deque(cache))
                {
                    expired_items_removed++;
                }
            }
            // Break early since cache is ordered
            else 
            {
                break;
            }
        }

    }
    return expired_items_removed;
}
