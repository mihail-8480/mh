#include "mh_writer_private.h"

void mh_write_string(const mh_writer_t *writer, const char* str) {
    mh_memory_t memory = MH_REF_STRING((void*)str);
    writer->write(writer->instance, &memory, memory.size);
}

void mh_write_memory(const mh_writer_t *writer, mh_memory_t memory) {
    writer->write(writer->instance, &memory, memory.size);
}

void mh_write_signed_number(const mh_writer_t *writer, mh_signed_number_t num) {
    char data[20];
    mh_memory_t mem = MH_REF_CONST(data);
    mh_int_to_string(&mem, num, 10);
    writer->write(writer->instance, &mem, mem.offset);
}

void mh_write_unsigned_number(const mh_writer_t *writer, mh_unsigned_number_t num) {
    char data[20];
    mh_memory_t mem = MH_REF_CONST(data);
    mh_uint_to_string(&mem, num, 10);
    writer->write(writer->instance, &mem, mem.offset);
}
