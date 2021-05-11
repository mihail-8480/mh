#include "../../inc/mh_tcp.h"
#include "../../inc/mh_thread.h"
#include <stdlib.h>

#if defined(UNIX)

#include <stdio.h>
#include <signal.h>
#include <sys/socket.h>

void mh_tcp_sigpipe() {
    // Try to get the context of the thread where the SIGPIPE happened
    mh_context_t *context = mh_context_get_from_thread();
    if (context == NULL) {
        return;
    }
    // Report the error on that context
    MH_THROW(context, "Broken pipe.");
}

#elif defined(WIN32)
#include <ws2tcpip.h>
#else
#error Unsupported platform.
#endif

// The new thread's arguments
typedef struct mh_tcp_threaded_args {
    mh_socket_t socket;
    mh_tcp_listener_t *listener;
    mh_socket_address_t address;
    mh_on_connect_t on_connect;
    mh_context_t *context;

} mh_tcp_threaded_args_t;

void *mh_tcp_threaded_connect_invoke(void *ptr) {
    // Get the arguments
    MH_THIS(mh_tcp_threaded_args_t*, ptr);
    mh_context_bind_to_thread(this->context);

    // Call the on_connect method with the passed arguments
    this->on_connect(this->listener, this->context, this->socket, this->address);

    // Free the previously allocated memory
    mh_end(this->context);
    return NULL;
}

void mh_tcp_start(mh_tcp_listener_t *listener) {
    if (listener->running) {
        MH_THROW(listener->context, "This listener is already running.");
    }
    listener->running = true;

#if defined(UNIX)
    // Handle broken pipes
    signal(SIGPIPE, mh_tcp_sigpipe);
#endif

    mh_socket_t sock;

    socklen_t addr_len = sizeof(listener->address);

    // If the socket isn't made, crash the program
    if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == (mh_socket_t) -1) {
        MH_THROW(listener->context, "A socket could not be created successfully.");
    }

#if defined(LINUX)
    // Set the socket options, if it fails, crash the program
    int opt = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) == -1) {
        MH_THROW(listener->context, "Failed setting socket options.");
    }
#endif

    // Bind the socket to the address specified earlier
    if (bind(sock, (struct sockaddr *) &listener->address, addr_len) == -1) {
        MH_THROW(listener->context, "Could not use the specified address.");
    }

    // Start listening
    if (listen(sock, listener->max_clients) == -1) {
        MH_THROW(listener->context, "Failed listening for clients.");
    }

    // Forever... (until the program crashes)
    while (listener->running) {
        // Accept a client
        mh_socket_t client = accept(sock, (struct sockaddr *) &listener->address, &addr_len);

        // If the client is invalid, crash the program
        if (client == (mh_socket_t) -1) {
            MH_THROW(listener->context, "Could not accept client.");
        }

        // If the listener is about to exit, close the socket
        if (!listener->running) {
#if defined(WIN32)
            closesocket(client);
#elif defined(UNIX)
            close(client);
#else
#error Unsupported platform.
#endif
            break;
        }

        mh_context_t *client_context = mh_start();

        // Allocate the arguments that get passed to the new thread
        mh_tcp_threaded_args_t *args = mh_context_allocate(client_context, sizeof(mh_tcp_threaded_args_t), true).ptr;

        // Copy the needed arguments
        args->address = listener->address;
        args->socket = client;
        args->on_connect = listener->on_connect;
        args->context = client_context;
        args->listener = listener;

        // Create the thread with those arguments
        mh_thread_create(mh_tcp_threaded_connect_invoke, args);
    }

    // Close the currently listening socket
#if defined(WIN32)
    closesocket(sock);
#elif defined(UNIX)
    close(sock);
#else
#error Unsupported platform.
#endif
}

unsigned short mh_tcp_address_to_string(char *host, mh_socket_address_t address, size_t size) {
    inet_ntop(address.sin_family, &address.sin_addr, host, size);
    return ntohs(address.sin_port);
}

mh_socket_address_t mh_tcp_string_to_address(const char *host, unsigned short port) {
    mh_socket_address_t address = {
            .sin_family = AF_INET,
            .sin_port = htons(port)
    };
    inet_pton(address.sin_family, host, &address.sin_addr);
    return address;
}

void mh_tcp_init(MH_UNUSED mh_tcp_listener_t *listener) {
#if defined(WIN32)
    // Initiates Winsock
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        WSACleanup();
        MH_THROW(listener->context, "WSAStartup failed.");
    }
    if (LOBYTE(wsa.wVersion) != 2 || HIBYTE(wsa.wVersion) != 2) {
        WSACleanup();
        MH_THROW(listener->context, "Invalid WinSock version.");
    }
#endif
}

mh_stream_t* mh_tcp_connect(mh_tcp_client_t* client) {
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == (mh_socket_t)-1) {
        MH_THROW(client->context, "A socket could not be created successfully.");
    }
    int con_res = connect(sock, (struct sockaddr*)&client->address, sizeof(mh_socket_address_t));
    if (con_res < 0) {
        MH_THROW(client->context, "Could not connect to server.");
    }
    return mh_socket_stream_new(client->context, sock);
}

void mh_tcp_cleanup(MH_UNUSED mh_tcp_listener_t *listener) {
#if defined(WIN32)
    WSACleanup();
#endif
}
