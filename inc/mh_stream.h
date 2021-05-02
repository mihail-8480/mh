#ifndef MHSERV_MH_STREAM_H
#define MHSERV_MH_STREAM_H

#include "mh_memory.h"
#include "mh_tcp.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

// The memory stream structure
MH_API_TYPE(mh_stream, struct mh_stream {
    mh_destructor_t destructor;
});

// Create a new memory stream
MH_API_FUNC(mh_stream_t *mh_memory_stream_new(mh_context_t *context, size_t size, bool fixed));

// Create a new socket stream (will probably work with normal file descriptors too)
MH_API_FUNC(mh_stream_t *mh_socket_stream_new(mh_context_t *context, mh_socket_t sock));

// Create a new file stream
MH_API_FUNC(mh_stream_t *mh_file_stream_new(mh_context_t *context, FILE *file, bool should_close));

// Get direct access to the memory stream's memory
MH_API_FUNC(mh_memory_t *mh_memory_stream_get_memory(mh_stream_t *stream));

// Move the stream's position, if it returns false it means it failed
MH_API_FUNC(bool mh_stream_seek(mh_stream_t *stream, size_t position));

// Read from a stream to memory
MH_API_FUNC(void mh_stream_read(mh_stream_t *stream, mh_memory_t *buffer, size_t count));

// Write from memory to a stream
MH_API_FUNC(void mh_stream_write(mh_stream_t *stream, mh_memory_t *buffer, size_t count));

// Get the stream's position
MH_API_FUNC(size_t mh_stream_get_position(mh_stream_t *stream));

// Get the stream's allocation_size
MH_API_FUNC(size_t mh_stream_get_size(mh_stream_t *stream));

// Copy bytes from one stream to an other
MH_API_FUNC(void mh_stream_copy_to(mh_stream_t *dest, mh_stream_t *src, size_t size));

// Write a string to a stream without copying it, return how many bytes were actually written
MH_API_FUNC(size_t mh_stream_write_reference(mh_stream_t *stream, const void *ptr, size_t size));

#endif //MHSERV_MH_STREAM_H
