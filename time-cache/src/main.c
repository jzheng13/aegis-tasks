#include <errno.h>
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

    return EXIT_SUCCESS;
}