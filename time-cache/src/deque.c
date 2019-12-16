#include "deque.h"

Deque* create_deque(size_t maximum_size)
{
    // Allocate memory for structure
    Deque *deque = (Deque*) malloc(sizeof(Deque));
    if (deque == NULL)
        return NULL;

    // Allocate memory for element array
    deque->elements = (int**) malloc(maximum_size * sizeof(int*));
    if (deque->elements == NULL)
    {
        free(deque);
        return NULL;
    }
    
    // Init deque attributes
    deque->head_offset = 0;
    deque->tail_offset = 0;
    deque->maximum_size = maximum_size;
    deque->size = 0;

    return deque;
}

void free_deque(Deque *deque)
{
    if (deque != NULL)
    {
        free(deque->elements);
        free(deque);
    }
}

int* peek_deque(Deque *deque) 
{
    // Only "peek" deque is non-empty
    if (deque->size > 0)
    {
        return deque->elements[deque->head_offset];
    }
    return NULL;
}

int* pop_deque(Deque *deque)
{
     // Only "pop" element if deque is non-empty
    if (deque->size > 0)
    {
        // Get previous "head" and increment if > 1 element in array
        int prev_head = deque->head_offset;
        if (deque->size > 1)
            deque->head_offset = (prev_head + 1) % deque->maximum_size;
        // Decrement count
        deque->size--;
        // Return element
        return deque->elements[prev_head];
    }
    return NULL;
}

bool push_deque(Deque *deque, int *element) 
{
    // Fail to push if maximum size exceeded  
    if (deque->size >= deque->maximum_size)
        return false;
    
    // If current size is 0, head == tail when pushed
    int next_tail = deque->tail_offset;
    // Else, increment to get element input position
    if (deque->size > 0)
        next_tail = (next_tail + 1) % deque->maximum_size;
    
    // Push item
    deque->elements[next_tail] = element;
    deque->tail_offset = next_tail;
    deque->size++;

    return true;
}
