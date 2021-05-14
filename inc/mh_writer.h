#ifndef MH_MH_WRITER_H
#define MH_MH_WRITER_H
#include "mh_memory.h"
#include "mh_convert.h"

typedef enum mh_writable_type {
    MH_WR_NULL = 0,
    MH_WR_STRING = 1,
    MH_WR_INT = 2,
    MH_WR_UINT = 3,
    MH_WR_MEM = 4,
    MH_WR_BOOL = 5,
    MH_WR_LOC = 6
} mh_writable_type_t;

MH_API_TYPE(mh_writer, struct mh_writer);
MH_API_TYPE(mh_writable, struct mh_writable {
    void *data;
    mh_writable_type_t type;
});

MH_API_FUNC(void mh_write_string(const mh_writer_t *writer, const char* str));
MH_API_FUNC(void mh_write_memory(const mh_writer_t *writer, mh_memory_t memory));
MH_API_FUNC(void mh_write_signed_number(const mh_writer_t *writer, mh_signed_number_t num));
MH_API_FUNC(void mh_write_unsigned_number(const mh_writer_t *writer, mh_unsigned_number_t num));
MH_API_FUNC(void mh_write(const mh_writer_t *writer, const char* format, ...));

#define MH_FMT(_data, _type) (mh_writable_t) {.data = (void*)_data, .type = _type}
#define MH_FMT_NULL MH_FMT(NULL, MH_WR_NULL)
#define MH_FMT_STR(_str) MH_FMT((char*)(_str), MH_WR_STRING)
#define MH_FMT_INT(_int) MH_FMT((mh_signed_number_t)(_int), MH_WR_INT)
#define MH_FMT_UINT(_int) MH_FMT((mh_unsigned_number_t)(_int), MH_WR_UINT)
#define MH_FMT_MEM(_mem) MH_FMT((mh_memory_t*)(_mem), MH_WR_MEM)
#define MH_FMT_BOOL(_bool) MH_FMT((bool)(_bool), MH_WR_BOOL)
#define MH_FMT_LOC(_loc) MH_FMT((mh_code_location_t*)(_loc), MH_WR_LOC)

#define MH_WRITE(...) mh_write(MH_OUTPUT, __VA_ARGS__)
#define MH_WRITE_ERR(...) mh_write(MH_ERROR, __VA_ARGS__)

#endif //MH_MH_WRITER_H
