#include <poll.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include "connection.h"
#include "ui.h"

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

    for(size_t i = 0; i < kv_size(conn->lines); i++) {
        string_t* str = kv_A(conn->lines, i);
        printf("%s", str->buff);
        string_free(str);
    }

    if(kv_size(conn->lines)) {
        kv_resize(string_t*, conn->lines, 0);
    }
}

static void
run_loop(connection_t* conn)
{
    struct pollfd fds[2] = {
        { .fd = conn->sock, .events = POLLIN },
        { .fd = 0,          .events = POLLIN },
    };

    struct pollfd* conn_fd = &fds[0];
    struct pollfd* stdin_fd = &fds[1];
    (void)stdin_fd;

    while(1) {
        poll(fds, sizeof(fds) / sizeof(*fds), -1);
        if(conn_fd->revents & POLLIN) {
            handle_incoming_data(conn);
        }
        if(conn_fd->revents & (POLLERR | POLLHUP)) {
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

    ui_t* ui = new_ui();

    run_loop(conn);

    free_ui(ui);

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
