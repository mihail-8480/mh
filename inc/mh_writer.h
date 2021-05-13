#ifndef MH_MH_WRITER_H
#define MH_MH_WRITER_H
#include "mh_memory.h"
#include "mh_convert.h"

MH_API_TYPE(mh_writer, struct mh_writer);

MH_API_FUNC(void mh_write_string(const mh_writer_t *writer, const char* str));
MH_API_FUNC(void mh_write_memory(const mh_writer_t *writer, mh_memory_t memory));
MH_API_FUNC(void mh_write_signed_number(const mh_writer_t *writer, mh_signed_number_t num));
MH_API_FUNC(void mh_write_unsigned_number(const mh_writer_t *writer, mh_unsigned_number_t num));

#endif //MH_MH_WRITER_H
