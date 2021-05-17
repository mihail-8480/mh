#ifndef MH_MH_READER_H
#define MH_MH_READER_H

#include "mh_memory.h"
#include "mh_convert.h"

/*
 * <mh_reader.h>
 * The libmh reader header.
 */

// The reader type.
MH_API_TYPE(mh_reader, struct mh_reader);

// Read a string from a reader into a character buffer until a character is reached or the buffer is filled.
MH_API_FUNC(void mh_read_string(const mh_reader_t *reader, char buffer[], size_t buffer_size, char end_at));

// Fill a mh_memory_t instance with bytes from the reader.
MH_API_FUNC(void mh_read_memory(const mh_reader_t *reader, mh_memory_t *memory));

// Read a single base 10 unsigned number.
MH_API_FUNC(mh_unsigned_number_t mh_read_uint(const mh_reader_t *reader));

// Read a single base 10 signed number.
MH_API_FUNC(mh_signed_number_t mh_read_int(const mh_reader_t *reader));

#endif //MH_MH_READER_H
