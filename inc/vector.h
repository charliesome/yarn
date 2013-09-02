#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>

#define vector_get(vector, type, index) ((type)((vector)->elements[(index)]))

typedef struct {
    size_t len;
    size_t cap;
    void** elements;
}
vector_t;

void
vector_init(vector_t*);

void
vector_init2(vector_t*, size_t capacity);

void
vector_free(vector_t*);

void
vector_clear(vector_t*);

void
vector_push(vector_t*, void* element);

void*
vector_shift(vector_t*);

#endif
