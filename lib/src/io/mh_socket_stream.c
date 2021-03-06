#include "../../../inc/mh_tcp.h"
#include "../../inc/mh_stream_private.h"

// The socket stream structure
typedef struct mh_socket_stream {
    mh_stream_private_t base;
    mh_socket_t socket;
} mh_socket_stream_t;

void mh_socket_stream_read(mh_ref_t stream, mh_memory_t *buffer, size_t count) {
    MH_THIS(mh_socket_stream_t*, stream);


    // Read from the socket
#if defined(UNIX)
    ssize_t size = read(this->socket, buffer->address, count);
#elif defined(WIN32)
    int size = recv((SOCKET) this->socket, buffer->address, (int) count, 0);
#else
#error Unsupported platform.
#endif
    // If the allocation_size is negative, something went wrong
    if (size == -1) {
        MH_THROW(this->base.context, "Failed reading from the socket, it is probably closed.");
    }

    // Change the buffer offset
    buffer->offset = size;
}

void mh_socket_stream_write(mh_ref_t stream, mh_memory_t *buffer, size_t count) {
    MH_THIS(mh_socket_stream_t*, stream);

    // Read from the socket
#if defined(UNIX)
    ssize_t size = write(this->socket, buffer->address, count);
#elif defined(WIN32)
    int size = send((SOCKET) this->socket, buffer->address, (int) count, 0);
#else
#error Unsupported platform.
#endif
    // See above.
    if (size == -1) {
        MH_THROW(this->base.context, "Failed writing to the socket, it is probably closed.");
    }

    // Change the buffer offset
    buffer->offset = size;
}

void mh_socket_stream_free(mh_ref_t stream) {
    MH_THIS(mh_socket_stream_t*, stream);
    // Shutdown the socket
#if defined (LINUX) || defined (HAIKU)
    shutdown(this->socket, SHUT_WR);
#endif
    // Close the socket
#if defined(UNIX)
    close(this->socket);
#elif defined(WIN32)
    closesocket(this->socket);
#else
#error Unsupported platform.
#endif
}

mh_stream_t *mh_socket_stream_new(mh_context_t *context, mh_socket_t socket) {
    MH_THIS(mh_socket_stream_t*, mh_context_allocate(context, sizeof(mh_socket_stream_t), false).ptr);

    this->base.base.destructor.free = mh_socket_stream_free;
    this->base.context = context;
    mh_context_add_destructor(context, &this->base.base.destructor);

    // Cannot flush
    this->base.flush = NULL;

    // Override and enable reading
    this->base.can_read = true;
    this->base.read = mh_socket_stream_read;

    // Override and enable writing
    this->base.can_write = true;
    this->base.write = mh_socket_stream_write;

    // Set to null and disable seeking
    this->base.can_seek = false;
    this->base.seek = NULL;
    this->base.get_position = NULL;
    this->base.get_size = NULL;

    this->socket = socket;
    return (mh_stream_t *) this;
}
