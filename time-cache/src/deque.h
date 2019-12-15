#ifndef DEQUE_H
#define DEQUE_H

#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

typedef struct deque {
    int **elements;
    int head_offset;
    int tail_offset;
    size_t maximum_size;
    size_t size;
} Deque;

Deque* create_deque(size_t maximum_size);
void free_deque(Deque *deque);
int* pop_deque(Deque *deque);
int* peek_deque(Deque *deque);
bool push_deque(Deque *deque, int *element);

#endif
