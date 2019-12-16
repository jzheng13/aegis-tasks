#include <ctype.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "cache.h"

pthread_t tid[2];
pthread_mutex_t mutex;

void* check_cache(void *args)
{
    TimeCache* time_cache = (TimeCache *) args;
    size_t num_expired;
    while(true)
    {
        pthread_mutex_lock(&mutex);
        num_expired = remove_expired_items_from_timecache(time_cache);
        if (num_expired > 0)
            printf("Removed %u expired items.\n", num_expired);
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(EXIT_SUCCESS);
}

char* strip(char *input_string)
{
    // Increment start pointer for leading spaces
    while(isspace((unsigned char)* input_string))
        input_string++;

    // Return string if empty
    if (*input_string == 0)
        return input_string;

    // Decrement end pointer for trailing spaces
    char *end;
    end = input_string + strlen(input_string) - 1;
    while(end > input_string && isspace((unsigned char) *end)) 
        end--;

    // Set string terminator
    end[1] = '\0';

    return input_string;
}

void* read_stdin(void *args)
{
    TimeCache* time_cache = (TimeCache *) args;

    size_t buffer_size = 128;
    size_t chars_read;
    char *buffer = (char*) malloc(buffer_size * sizeof(char));
    char *stripped;
    int cache_item;

    if (buffer == NULL)
    {
        perror("Unable to allocate buffer to read stdin inputs.\n");
        pthread_exit(EXIT_SUCCESS);
    }

    // Read input from command line
    while (true) {
        printf("Enter an item ID to cache:\n");
        chars_read = getline(&buffer, &buffer_size, stdin);
        if (chars_read > 1)
        {
            stripped = strip(buffer);
            cache_item = strtoul(stripped, NULL, 10);
            pthread_mutex_lock(&mutex);
            bool success = add_item_to_timecache(time_cache, cache_item);
            printf("Add item %i %s.\n", cache_item, 
                                        success ? "success" : "failed");
            list_timecache_info(time_cache);
            pthread_mutex_unlock(&mutex);
        }
        else
        {
            printf("Empty input, terminating program.\n");
            break;
        }
    }
    free(buffer);
    pthread_exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
    size_t capacity = 10;
    size_t ttl = 30;

    // Read cmdline args
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

    // Init mutex for synchronisation
    if (pthread_mutex_init(&mutex, NULL) != 0) 
    {
        printf("Failed to initialise threads\n");
        return EXIT_FAILURE;
    }

    // Create time cache
    printf("Initialising cache with capacity %d and TTL %d.\n", capacity, ttl);
    TimeCache* time_cache = create_timecache(capacity, ttl);

    // Threads to simultaneously read cmdline inputs and clean up expired items
    pthread_create(&tid[0], NULL, read_stdin, (void*) time_cache);
    pthread_create(&tid[1], NULL, check_cache, (void*) time_cache);

    // Wait for stdin program to complete
    pthread_join(tid[0], NULL);

    // Free memory
    pthread_mutex_destroy(&mutex);

    printf("Freeing cache.\n");
    free_timecache(time_cache);

    return EXIT_SUCCESS;
}