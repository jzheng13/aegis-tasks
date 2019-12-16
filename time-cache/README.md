# Time-based Cache

`time-cache` is an executable written in C that mimicks a cache implementation.
The program listens for inputs, keeps them in its cache and removes oldest items
when maximum cache count is exceeded and also "expires" cached elements when
TTL is exceeded.

> **Note:** For simplicity of implementation, only integer cache items are 
> accepted for this exercise.

## Setup

The following steps are only required if you wish to compile the program from
scratch (in a Linux environment).

Install required dependencies by running the setup script
```sh
bash setup.sh
```

## Build

Build the executable by running the following command in the `time-cache`
directory
```sh
make
```

This should create an executable named `timecache` in `time-cache/bin`.

## How to run

The executable takes in two options
- `-c <capacity>` capacity of cache
- `-t <ttl>` maximum time-to-live in seconds

For instance, we can start the program with
```sh
./timecache -c 10 -t 300
```

This means that the cache keeps a maximum of 10 items with a maximum TTL of 5
minutes.

If the two values are not provided, `capacity` defaults to 10 and `ttl` defaults
to 30 seconds.

### Adding an item

To add an item to the cache, just type the item name after the program has
started then press ENTER. You should see the following message
```sh
Add item <item-name> success.
Cache is at m/n capacity.
1. Item <item-1> inserted at <item-1-insertion-time>.
2. Item <item-2> inserted at <item-2-insertion-time>.
...
m. Item <item-m> inserted at <item-m-insertion-time>.
```

### Exceeding cache capacity

If the addition of this new item results in the purge of the oldest item, you
should see
```sh
Cache limit has exceeded. <item-1> has been removed. 
```

### Expiring cached item

When an item has expired, you should see the following message
```sh
Removed x expired items.
```

## Known issues and possible solutions

If re-adding an existing element, insertion time should be refreshed. However,
presently, we simply "read" from cache without extending TTL of the record. 

We can go back to using just a hash table and update the hash value to solve
this issue, but this will mean that we need to iterate over the entire hash
table when trying to find expired items.

(Technically speaking with the current implementation there is not much use for
the hash map since we can just add another array as the insertion time.)
