#include "mh_writer_private.h"
#include <stdarg.h>

void mh_write_string(const mh_writer_t *writer, const char* str) {
    mh_memory_t memory = MH_REF_STRING((void*)str);
    writer->write(writer->instance, &memory, memory.size);
}

void mh_write_memory(const mh_writer_t *writer, mh_memory_t memory) {
    writer->write(writer->instance, &memory, memory.size);
}

void mh_write_signed_number(const mh_writer_t *writer, mh_signed_number_t num) {
    char data[21];
    mh_memory_t mem = MH_REF_CONST(data);
    mh_int_to_string(&mem, num, 10);
    writer->write(writer->instance, &mem, mem.offset);
}

void mh_write_unsigned_number(const mh_writer_t *writer, mh_unsigned_number_t num) {
    char data[21];
    mh_memory_t mem = MH_REF_CONST(data);
    mh_uint_to_string(&mem, num, 10);
    writer->write(writer->instance, &mem, mem.offset);
}

inline static void mh_buffered_flush(const mh_writer_t *writer, mh_memory_t *buf) {
    mh_write_memory(writer, (mh_memory_t){.offset = 0, .size = buf->offset, .address = buf->address});
    buf->offset = 0;
}

inline static void mh_buffered_write(const mh_writer_t *writer, mh_memory_t *buf, char c) {
    retry:
    if (buf->offset < buf->size) {
        ((char*)buf->address)[buf->offset] = c;
        buf->offset++;
    } else {
        mh_buffered_flush(writer, buf);
        goto retry;
    }
}

static void mh_buffered_write_string(const mh_writer_t *writer, mh_memory_t *buf, const char *c) {
    while(*c) {
        mh_buffered_write(writer, buf, *c);
        c++;
    }
}

inline static void mh_buffered_write_memory(const mh_writer_t *writer, mh_memory_t *buf, mh_memory_t mem) {
    for(size_t i = 0; i < mem.size; i++) {
        mh_buffered_write(writer, buf, ((char*)mem.address)[i]);
    }
}

inline static void mh_buffered_write_writable(const mh_writer_t *writer, mh_memory_t *buf, mh_writable_t writable) {
    char data[21];
    mh_memory_t mem;
    mh_code_location_t *location;
    switch(writable.type) {
        case MH_WR_NULL:
            if (writable.data != NULL) {
                mh_buffered_write_string(writer, buf, "(invalid)");
            } else {
                mh_buffered_write_string(writer, buf, "(null)");
            }
            break;
        case MH_WR_STRING:
            if (writable.data != NULL) {
                mh_buffered_write_string(writer, buf, writable.data);
            } else {
                mh_buffered_write_string(writer, buf, "(null)");
            }
            break;
        case MH_WR_INT:
            mem = MH_REF_CONST(data);
            if (mh_int_to_string(&mem, (mh_signed_number_t)writable.data, 10)) {
                mh_buffered_write_string(writer, buf, data);
            } else {
                mh_buffered_write_string(writer, buf, "(error)");
            }
            break;
        case MH_WR_UINT:
            mem = MH_REF_CONST(data);
            if (mh_uint_to_string(&mem, (mh_unsigned_number_t)writable.data, 10)) {
                mh_buffered_write_string(writer, buf, data);
            } else {
                mh_buffered_write_string(writer, buf, "(error)");
            }
            break;
        case MH_WR_MEM:
            if (writable.data != NULL) {
                mem = *(mh_memory_t *) (writable.data);
                mh_buffered_write_memory(writer, buf, mem);
            } else {
                mh_buffered_write_string(writer, buf, "(null)");
            }
            break;
        case MH_WR_BOOL:
            if (writable.data) {
                mh_buffered_write_string(writer, buf, "True");
            } else {
                mh_buffered_write_string(writer, buf, "False");
            }
            break;
        case MH_WR_LOC:
            if (writable.data != NULL) {
                location = (mh_code_location_t *)writable.data;
                mh_buffered_write_string(writer, buf, "in ");
                mh_buffered_write_string(writer, buf, location->function_name);
                mh_buffered_write_string(writer, buf, "() at ");
                mh_buffered_write_string(writer, buf, location->file_name);
                mem = MH_REF_CONST(data);
                if (mh_uint_to_string(&mem, (mh_unsigned_number_t)location->file_line, 10)) {
                    mh_buffered_write(writer, buf, ':');
                    mh_buffered_write_string(writer, buf, mem.address);
                }
            } else {
                mh_buffered_write_string(writer, buf, "(null)");
            }
            break;
        default:
            mh_buffered_write_string(writer, buf, "(unknown)");
            break;
    }
}

void mh_write(const mh_writer_t *writer, const char* format, ...) {
    va_list list;
    va_start(list, format);
    char s_buf[32];
    mh_memory_t buf = MH_REF_CONST(s_buf);
    char prev = 0;
    while (*format) {
        if (*format == '{' && format[1] == '}') {
            if (prev != '{') {
                mh_buffered_write_writable(writer, &buf, va_arg(list, mh_writable_t));
            }
            format++;
        } else {
            mh_buffered_write(writer, &buf, *format);
        }
        prev = *format;
        format++;
    }
    mh_buffered_flush(writer, &buf);
    va_end(list);
}
