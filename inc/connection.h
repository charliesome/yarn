#ifndef CONNECTION_H
#define CONNECTION_H

#include "string.h"
#include "vector.h"

typedef struct {
    const char* error;

    int sock;

    string_t line_buffer;
    vector_t lines;
}
connection_t;

connection_t*
new_connection(const char* host, int port);

void
free_connection(connection_t* conn);

int
connection_recv_data(connection_t* conn);

#endif
