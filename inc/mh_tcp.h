#ifndef MHSERV_MH_TCP_H
#define MHSERV_MH_TCP_H

#include "mh_context.h"
#include "mh_stream.h"
#include <stdint.h>
#include <stdbool.h>

#if defined(UNIX)

#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#elif defined(WIN32)
#include <winsock2.h>
#else
#error Unsupported platform.
#endif


/*
 * <mh_tcp.h>
 * The libmh TCP header.
 *
 * Contains an implementation of a TCP listener.
 */

// The address of a socket.
MH_API_TYPE(mh_socket_address, struct sockaddr_in);

// The socket type.
#if defined(UNIX)
MH_API_TYPE(mh_socket, int);
#elif defined(WIN32)
MH_API_TYPE(mh_socket, SOCKET);
#else
#error Unsupported platform.
#endif


// The TCP listener struct.
MH_API_TYPE(mh_tcp_listener, struct mh_tcp_listener);

// The TCP client struct.
MH_API_TYPE(mh_tcp_client, struct mh_tcp_client {
    // The context where the TCP client is running.
    mh_context_t *context;
    // The address that the TCP client should connect to.
    mh_socket_address_t address;
});

// A function pointer type for mh_tcp_start.
typedef void (*mh_on_connect_t)(mh_tcp_listener_t *, mh_context_t *, mh_socket_t, mh_socket_address_t);


// The implementation of the TCP listener struct.
struct mh_tcp_listener {
    // The context where the TCP listener is allocated in.
    mh_context_t *context;

    // The address the TCP listener is bound to.
    mh_socket_address_t address;

    // The maximal amount of clients that can be queued.
    int max_clients;

    // Is the TCP listener running?
    bool running;

    // The function that gets called when a client connects.
    mh_on_connect_t on_connect;
};


// Convert a socket address to a string.
MH_API_FUNC(unsigned short mh_tcp_address_to_string(char *dest, mh_socket_address_t address, size_t size));

// Convert a string to a socket address.
MH_API_FUNC(mh_socket_address_t mh_tcp_string_to_address(mh_const_string_t str, unsigned short port));

// Start a TCP listener.
MH_API_FUNC(void mh_tcp_start(mh_tcp_listener_t *listener));

// Connect a TCP client to a server.
MH_API_FUNC(mh_socket_t mh_tcp_connect(mh_tcp_client_t *client));

// Create a new socket stream (will probably work with normal file descriptors on unix too).
MH_API_FUNC(mh_stream_t *mh_socket_stream_new(mh_context_t *context, mh_socket_t sock));


#endif //MHSERV_MH_TCP_H
