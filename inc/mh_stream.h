#ifndef MHSERV_MH_STREAM_H
#define MHSERV_MH_STREAM_H

#include "mh_memory.h"
#include "mh_writer.h"
#include "mh_reader.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

/*
 * <mh_stream.h>
 * The libmh stream header.
 *
 * Contains stream types and functions for reading/writing to streams.
 */

// A stream.
MH_API_TYPE(mh_stream, struct mh_stream {
    // A destructor.
    mh_destructor_t destructor;
});

// Create a new memory stream.
MH_API_FUNC(mh_stream_t *mh_memory_stream_new(mh_context_t *context, size_t size, bool fixed));

// Create a new file stream.
MH_API_FUNC(mh_stream_t *mh_file_stream_new(mh_context_t *context, FILE *file, bool should_close));

// Get direct access to the memory stream's memory.
MH_API_FUNC(mh_memory_t *mh_memory_stream_get_memory(mh_stream_t *stream));

// Move the stream's position, if it returns false it means it failed.
MH_API_FUNC(bool mh_stream_seek(mh_stream_t *stream, size_t position));

// Read from a stream to memory.
MH_API_FUNC(void mh_stream_read(mh_stream_t *stream, mh_memory_t *buffer, size_t count));

// Write from memory to a stream.
MH_API_FUNC(void mh_stream_write(mh_stream_t *stream, mh_memory_t *buffer, size_t count));

// Flush the stream buffer (if there is one).
MH_API_FUNC(void mh_stream_flush(mh_stream_t *stream));

// Get the stream's position.
MH_API_FUNC(size_t mh_stream_get_position(mh_stream_t *stream));

// Get the stream's allocation_size.
MH_API_FUNC(size_t mh_stream_get_size(mh_stream_t *stream));

// Copy bytes from one stream to an other.
MH_API_FUNC(void mh_stream_copy_to(mh_stream_t *dest, mh_stream_t *src, size_t size));

// Write a string to a stream without copying it twice, return how many bytes were actually written.
MH_API_FUNC(size_t mh_stream_write_reference(mh_stream_t *stream, mh_ref_t ptr, size_t size));

// Get a writer from a stream.
MH_API_FUNC(mh_writer_t *mh_writer_from_stream(mh_stream_t *stream));

// Get a writer from a stream.
MH_API_FUNC(mh_reader_t *mh_reader_from_stream(mh_stream_t *stream));


#endif //MHSERV_MH_STREAM_H
