#ifndef DEQUE_H
#define DEQUE_H

typedef struct deque {
    char ** elements;
    int head_offset;
    int tail_offset;
    size_t maximum_size;
    size_t size;
} Deque;

Deque create_deque(size_t maximum_size);
char* pop_deque(Deque deque);
char* push_deque(Deque deque, char* element);
size_t deque_size(Deque deque);
char** deque_elements(Deque deque);

#endif