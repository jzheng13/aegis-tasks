#include "cache.h"

TimeCache* create_timecache(size_t capacity, size_t ttl)
{
    // Allocate memory for time cache
    TimeCache *time_cache = (TimeCache*) malloc(sizeof(TimeCache));
    if (time_cache == NULL)
        return NULL;

    // Allocate memory for hash map
    GHashTable *hash_table = g_hash_table_new_full(g_int_hash, 
                                                   g_int_equal, 
                                                   free, 
                                                   free);
    if (hash_table == NULL)
    {
        free(time_cache);
        return NULL;
    }

    // Allocate memory for deque
    Deque *deque = create_deque(capacity);
    if (deque == NULL)
    {
        free(time_cache);
        g_hash_table_destroy(hash_table);
        return NULL;
    }

    time_cache->cache_map = hash_table;
    time_cache->cache = deque;
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
    if (time_cache != NULL)
        return time_cache->cache->size;
    return -1;
}

int** timecache_items(TimeCache *time_cache)
{
    if (time_cache != NULL)
        return time_cache->cache->elements;
    return NULL;
}

bool add_item_to_timecache(TimeCache *time_cache, int cache_item) 
{
    int *key = (int*) malloc(sizeof(int));
    if (key == NULL)
    {
        return false;
    }
    *key = cache_item;

    // Skip cache if item already exists
    if (g_hash_table_contains(time_cache->cache_map, key))
    {
        printf("Item %d already cached.\n", cache_item);
        free(key);
        return false;
    }

    // Pop oldest element if cache is at or past capacity
    if (timecache_size(time_cache) >= time_cache->capacity) {
        int *oldest = peek_deque(time_cache->cache);
        if (oldest != NULL)
        {
            g_hash_table_remove(time_cache->cache_map, oldest);
            pop_deque(time_cache->cache);
            printf("Cache limit has exceeded. %d has been removed.\n", *oldest);
        } 
        else 
        {
            // Something wrong
            return false;
        }
        
    }

    // Insert cache item to cache with timestamp of insertion
    gpointer *val = (gpointer*) malloc(sizeof(time_t));
    if (val == NULL)
    {
        free(key);
        return false;
    }
    
    *val = time(NULL);
    int inserted = g_hash_table_insert(time_cache->cache_map, key, val);
    if (inserted)
    {
        time_t *insertion_time = \
            (time_t*) g_hash_table_lookup(time_cache->cache_map, key);
        return push_deque(time_cache->cache, key);
    }
    free(key);
    free(val);
    return inserted;
}

size_t remove_expired_items_from_timecache(TimeCache *time_cache)
{
    // Counter of expired items
    size_t expired_items_removed = 0;
    // Get current system epoch time
    time_t current_time = (time_t) time(NULL);
    // Get init size of cache
    size_t size = timecache_size(time_cache);
    
    for (int i = 0; i < size; i++)
    {
        // Peek at oldest item and get insertion time
        int *oldest = peek_deque(time_cache->cache);
        time_t *insertion_time = \
            (time_t*) g_hash_table_lookup(time_cache->cache_map, oldest);
        if (insertion_time != NULL)
        {
            // Pop item from cache if expired
            if ((*insertion_time + time_cache->ttl) < current_time)
            {
                if (pop_deque(time_cache->cache) != NULL)
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

void list_timecache_info(TimeCache *time_cache)
{
    int head = time_cache->cache->head_offset;
    size_t size = timecache_size(time_cache);
    printf("Cache is at %d/%d capacity.\n", size, time_cache->capacity);

    for (int i = 0; i < size; i++)
    {
        int index = (head + i) % time_cache->capacity;
        int *element = time_cache->cache->elements[index];
        time_t *insertion_time = \
            (time_t*) g_hash_table_lookup(time_cache->cache_map, element);
        if (insertion_time != NULL)
            printf("%d. Item %d inserted at %ld.\n", i + 1, 
                                                     *element, 
                                                     *insertion_time);
    }
}
