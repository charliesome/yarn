#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "connection.h"

static int
attempt_connection(connection_t* conn, struct addrinfo* addr)
{
    for(; addr; addr = addr->ai_next) {
        int sock = socket(addr->ai_family, SOCK_STREAM, 0);
        if(sock < 0) {
            conn->error = strerror(errno);
            continue;
        }

        int res = connect(sock, addr->ai_addr, addr->ai_addrlen);
        if(res < 0) {
            conn->error = strerror(errno);
            close(sock);
            continue;
        }

        conn->error = NULL;
        conn->sock = sock;
        return 0;
    }

    return -1;
}

static void
initialize_connection(connection_t* conn)
{
    conn->sock = -1;
    conn->error = NULL;
    string_init(&conn->line_buffer);
    vector_init(&conn->lines);
}

connection_t*
new_connection(const char* host, int port)
{
    int err;
    connection_t* conn = malloc(sizeof(*conn));
    initialize_connection(conn);

    if(port <= 0 || port > 65535) {
        conn->error = "Invalid port number";
        return conn;
    }

    struct addrinfo* addrs;
    char port_str[16];
    sprintf(port_str, "%d", port);
    err = getaddrinfo(host, port_str, NULL, &addrs);
    if(err) {
        conn->error = gai_strerror(err);
        return conn;
    }

    err = attempt_connection(conn, addrs);
    freeaddrinfo(addrs);
    if(err < 0) {
        /* conn->error is set by attempt_connection */
        return conn;
    }

    conn->error = NULL;
    return conn;
}

static void
connection_close(connection_t* conn)
{
    if(conn->sock >= 0) {
        shutdown(conn->sock, SHUT_RDWR);
        close(conn->sock);
        conn->sock = -1;
    }
}

void
free_connection(connection_t* conn)
{
    free(conn);
}

int
connection_recv_data(connection_t* conn)
{
    if(conn->sock == -1) {
        return -1;
    }

    char buff[4096];
    
    int n = recv(conn->sock, buff, 4096, 0);

    if(n <= 0) {
        connection_close(conn);
        return -1;
    }

    for(int i = 0; i < n; i++) {
        string_putc(&conn->line_buffer, buff[i]);

        if(buff[i] == '\n') {
            vector_push(&conn->lines, string_dup(&conn->line_buffer));
            string_clear(&conn->line_buffer);
        }
    }

    return 0;
}
