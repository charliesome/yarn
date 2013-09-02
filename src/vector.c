#include <string.h>
#include "vector.h"

void
vector_init(vector_t* vector)
{
    vector_init2(vector, 2);
}

void
vector_init2(vector_t* vector, size_t capacity)
{
    vector->len = 0;
    vector->cap = capacity;
    vector->elements = malloc(sizeof(void*) * capacity);
}

void
vector_free(vector_t* vector)
{
    free(vector->elements);
    vector->cap = 0;
    vector->len = 0;
}

void
vector_clear(vector_t* vector)
{
    vector_free(vector);
    vector_init(vector);
}

void
vector_push(vector_t* vector, void* element)
{
    if(vector->len + 1 >= vector->cap) {
        vector->cap *= 2;
        vector->elements = realloc(vector->elements, vector->cap * sizeof(void*));
    }
    vector->elements[vector->len++] = element;
}

void*
vector_shift(vector_t* vector)
{
    if(vector->len == 0) {
        return NULL;
    }

    void* first = vector->elements[0];
    memmove(vector->elements, vector->elements + 1, vector->len - 1);
    vector->len--;
    return first;
}
