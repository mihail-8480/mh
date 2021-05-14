#ifndef MH_MH_WRITER_H
#define MH_MH_WRITER_H

#include "mh_memory.h"
#include "mh_convert.h"

/*
 * <mh_writer.h>
 * The libmh writer/format header.
 */

// The type of something that can be written.
typedef enum mh_writable_type {
    // Nothing.
    MH_WR_NULL = 0,

    // String.
    MH_WR_STRING = 1,

    // Signed integer.
    MH_WR_INT = 2,

    // Unsigned integer.
    MH_WR_UINT = 3,

    // mh_memory_t instance.
    MH_WR_MEM = 4,

    // Boolean.
    MH_WR_BOOL = 5,

    // mh_code_location_t instance.
    MH_WR_LOC = 6
} mh_writable_type_t;

// Something that is writable.
MH_API_TYPE(mh_writable, struct mh_writable {
    // The data that will get written.
    void *data;

    // How is the data supposed to be written.
    mh_writable_type_t type;
});

// Something that can be written to.
MH_API_TYPE(mh_writer, struct mh_writer);

// Write a string into a writer.
MH_API_FUNC(void mh_write_string(const mh_writer_t *writer, const char *str));

// Write a mh_memory_t into a writer.
MH_API_FUNC(void mh_write_memory(const mh_writer_t *writer, mh_memory_t memory));

// Write a signed number into a writer.
MH_API_FUNC(void mh_write_signed_number(const mh_writer_t *writer, mh_signed_number_t num));

// Write an unsigned number into a writer.
MH_API_FUNC(void mh_write_unsigned_number(const mh_writer_t *writer, mh_unsigned_number_t num));

// Write multiple mh_writable_t into a writer, follow a format where {} is the argument.
MH_API_FUNC(void mh_write(const mh_writer_t *writer, const char *format, ...));

// Create an argument for mh_write().
#define MH_FMT(_data, _type) (mh_writable_t) {.data = (void*)_data, .type = _type}

// Create an argument with the value (null).
#define MH_FMT_NULL MH_FMT(NULL, MH_WR_NULL)

// Create an argument that's a null terminated string.
#define MH_FMT_STR(_str) MH_FMT((char*)(_str), MH_WR_STRING)

// Create an argument that's a signed integer.
#define MH_FMT_INT(_int) MH_FMT((mh_signed_number_t)(_int), MH_WR_INT)

// Create an argument that's an unsigned integer.
#define MH_FMT_UINT(_int) MH_FMT((mh_unsigned_number_t)(_int), MH_WR_UINT)

// Create an argument that's a mh_memory_t instance.
#define MH_FMT_MEM(_mem) MH_FMT((mh_memory_t*)(_mem), MH_WR_MEM)

// Create an argument that's a boolean.
#define MH_FMT_BOOL(_bool) MH_FMT((bool)(_bool), MH_WR_BOOL)

// Create an argument that's a mh_code_location_t instance.
#define MH_FMT_LOC(_loc) MH_FMT((mh_code_location_t*)(_loc), MH_WR_LOC)

#endif //MH_MH_WRITER_H
