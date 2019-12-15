#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "cache.h"

int main(int argc, char *argv[])
{
    size_t capacity = 10;
    size_t ttl = 30;

    int opt;
    while ((opt = getopt(argc, argv, "c:t:")) != -1)
    {
        switch (opt)
        {
            case 'c':
                capacity = strtoul(optarg, NULL, 10);
                break;
            case 't':
                ttl = strtoul(optarg, NULL, 10);
                break;
        }
    }

    printf("Initialising cache with capacity %d and TTL %d.\n", capacity, ttl);
    TimeCache* timecache = create_timecache(capacity, ttl);
    bool success = add_item_to_timecache(timecache, 1);
    printf("Add item %i %s.\n", 1, success ? "success" : "failed");
    list_timecache_info(timecache);
    printf("Freeing cache.\n");
    free_timecache(timecache);

    return EXIT_SUCCESS;
}