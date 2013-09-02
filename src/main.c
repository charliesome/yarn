#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include "connection.h"
#include "vector.h"

static void
usage()
{
    printf("Usage: yarn <server> [<port>]\n");
    exit(EXIT_SUCCESS);
}

static void
handle_incoming_data(connection_t* conn)
{
    connection_recv_data(conn);

    for(size_t i = 0; i < conn->lines.len; i++) {
        string_t* str = conn->lines.elements[i];
        printf("%s", str->buff);
        string_free(str);
    }

    if(conn->lines.len) {
        vector_clear(&conn->lines);
    }
}

static void
run_loop(connection_t* conn)
{
    struct pollfd fds[1] = {
        { .fd = conn->sock, .events = POLLIN },
    };

    while(1) {
        poll(fds, sizeof(fds) / sizeof(*fds), -1);
        if(fds[0].revents & POLLIN) {
            handle_incoming_data(conn);
        }
        if(fds[0].revents & (POLLERR | POLLHUP)) {
            fprintf(stderr, "server hung up or errored\n");
            break;
        }
    }
}

static void
start_client(const char* host, int port)
{
    connection_t* conn = new_connection(host, port);
    if(conn->error) {
        fprintf(stderr, "Could not connect to %s:%d: %s\n", host, port, conn->error);
        exit(EXIT_FAILURE);
    }

    run_loop(conn);

    free_connection(conn);
}

int
main(int argc, const char** argv)
{
    const char* host;
    int port = 6667;

    if(argc <= 1) {
        usage();
    }

    host = argv[1];

    if(argc > 2) {
        port = atoi(argv[2]);
    }

    start_client(host, port);
}
