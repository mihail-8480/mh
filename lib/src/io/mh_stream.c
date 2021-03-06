#include "../../../inc/mh_stream.h"
#include "../../inc/mh_writer_private.h"
#include "../../inc/mh_reader_private.h"
#include "../../inc/mh_stream_private.h"

void mh_stream_flush(mh_stream_t *stream) {
    MH_THIS(mh_stream_private_t*, stream);
    if (this->flush != NULL) {
        this->flush(this);
    }
}

bool mh_stream_seek(mh_stream_t *stream, size_t position) {
    MH_THIS(mh_stream_private_t*, stream);
    if (this->can_seek) {
        this->seek(this, position);
        return true;
    }
    return false;
}

void mh_stream_read(mh_stream_t *stream, mh_memory_t *buffer, size_t count) {
    MH_THIS(mh_stream_private_t*, stream);

    if (this->can_read) {
        this->read(this, buffer, count);
        return;
    }

    // Cannot read, report error
    MH_THROW(this->context, "Cannot read from this stream.");
}

void mh_stream_write(mh_stream_t *stream, mh_memory_t *buffer, size_t count) {
    MH_THIS(mh_stream_private_t*, stream);
    if (this->can_write) {
        this->write(this, buffer, count);
        return;
    }

    // Cannot write.
    MH_THROW(this->context, "Cannot write to this stream.");
}

size_t mh_stream_get_position(mh_stream_t *stream) {
    MH_THIS(mh_stream_private_t*, stream);
    // Get the current position if it's possible
    if (this->can_seek) {
        return this->get_position(this);
    }
    return -1;
}

size_t mh_stream_get_size(mh_stream_t *stream) {
    MH_THIS(mh_stream_private_t*, stream);
    // Get the current allocation_size if it's possible
    if (this->can_seek) {
        return this->get_size(this);
    }
    return -1;
}

void mh_stream_copy_to(mh_stream_t *dest, mh_stream_t *src, size_t size) {
    mh_stream_private_t *src_stream = (mh_stream_private_t *) src;
    mh_stream_private_t *dest_stream = (mh_stream_private_t *) dest;

    // If the streams are seekable, check if this operation is possible
    if (src_stream->can_seek && size > src_stream->get_size(src_stream)) {
        MH_THROW(src_stream->context,
                 "Not enough memory in src_stream to preform a mh_stream_copy_to operation.");
    }
    if (dest_stream->can_seek && size > dest_stream->get_size(dest_stream)) {
        MH_THROW(dest_stream->context,
                 "Not enough memory in dest_stream to preform a mh_stream_copy_to operation.");
    }

    mh_memory_t *buffer = mh_memory_new(dest_stream->context, size, false);
    mh_stream_read(src, buffer, size);
    mh_stream_write(dest, buffer, buffer->offset);
}

size_t mh_stream_write_reference(mh_stream_t *stream, mh_ref_t ptr, size_t size) {
    // Create a memory reference
    mh_memory_t memory = mh_memory_reference((mh_ref_t) ptr, size);
    // Write to the stream
    mh_stream_write(stream, &memory, memory.size);
    return memory.offset;
}

void mh_stream_writer_write(mh_ref_t instance, mh_memory_t *memory, size_t size) {
    MH_THIS(mh_stream_t*, instance);
    mh_stream_write(this, memory, size);
    mh_stream_flush(this);
}

void mh_stream_reader_read(mh_ref_t instance, mh_memory_t *memory, size_t size) {
    MH_THIS(mh_stream_t*, instance);
    mh_stream_read(this, memory, size);
}

mh_writer_t *mh_writer_from_stream(mh_stream_t *stream) {
    MH_THIS(mh_stream_private_t*, stream);
    if (!this->can_write) {
        MH_THROW(this->context, "Cannot get a writer from a stream that cannot be written to.");
    }
    mh_writer_t *writer = mh_context_allocate(this->context, sizeof(mh_writer_t), false).ptr;
    *writer = (mh_writer_t) {
            .instance = stream,
            .write = mh_stream_writer_write
    };
    return writer;
}

mh_reader_t *mh_reader_from_stream(mh_stream_t *stream) {
    MH_THIS(mh_stream_private_t*, stream);
    if (!this->can_read) {
        MH_THROW(this->context, "Cannot get a reader from a stream that cannot be written to.");
    }
    mh_reader_t *reader = mh_context_allocate(this->context, sizeof(mh_reader_t), false).ptr;
    *reader = (mh_reader_t) {
            .instance = stream,
            .read = mh_stream_reader_read
    };
    return reader;
}
