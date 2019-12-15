#include <ctype.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "cache.h"

void check_cache(TimeCache *cache)
{
}

char* strip(char *input_string)
{
  while(isspace((unsigned char)* input_string))
      input_string++;

  if (*input_string == 0)
      return input_string;

  char *end;
  end = input_string + strlen(input_string) - 1;
  while(end > input_string && isspace((unsigned char) *end)) 
      end--;
  end[1] = '\0';

  return input_string;
}

int read_stdin(TimeCache *time_cache)
{
    size_t buffer_size = 128;
    size_t chars_read;
    char *buffer = (char*) malloc(buffer_size * sizeof(char));
    char *stripped;
    int cache_item;

    if (buffer == NULL)
    {
        perror("Unable to allocate buffer to read stdin inputs.");
        return EXIT_FAILURE;
    }

    // Read input from command line
    while (true) {
        printf("Enter an item ID to cache:\n");
        chars_read = getline(&buffer, &buffer_size, stdin);
        if (chars_read > 1)
        {
            stripped = strip(buffer);
            cache_item = strtoul(stripped, NULL, 10);
            bool success = add_item_to_timecache(time_cache, cache_item);
            printf("Add item %i %s.\n", cache_item, 
                                        success ? "success" : "failed");
            list_timecache_info(time_cache);
        }
        else
        {
            printf("Empty input, terminating program.\n");
            break;
        }
    }
    return EXIT_SUCCESS;
}

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
    TimeCache* time_cache = create_timecache(capacity, ttl);
    read_stdin(time_cache);
    printf("Freeing cache.\n");
    free_timecache(time_cache);

    return EXIT_SUCCESS;
}