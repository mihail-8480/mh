#include "../../inc/mh_reader_private.h"

void mh_read_memory(const mh_reader_t *reader, mh_memory_t *memory) {
    reader->read(reader->instance, memory, memory->size);
}

void mh_read_string(const mh_reader_t *reader, char buffer[], size_t buffer_size, char end_at) {
    char chr;
    mh_memory_t mem = mh_memory_reference(&chr, sizeof chr);
    for (size_t position = 0; position < buffer_size; position++) {
        reader->read(reader->instance, &mem, mem.size);
        if (mem.offset != 1) {
            break;
        }
        if (chr == end_at) {
            break;
        }
        buffer[position] = chr;
    }
}

mh_unsigned_number_t mh_read_uint(const mh_reader_t *reader) {
    char num_str[21];
    mh_memory_t mem = mh_memory_reference(num_str, sizeof num_str);
    for (size_t position = 0; position < mem.size; position++) {
        reader->read(reader->instance, &mem, 1);
        if (mem.offset != 1) {
            break;
        } else {
            mem.address = (char *) mem.address + 1;
        }
        if (!mh_is_number(num_str[position], MH_BASE_DEC)) {
            num_str[position] = 0;
            break;
        }
    }
    mh_unsigned_number_t num = 0;
    mh_uint_parse(num_str, MH_BASE_DEC, &num);
    return num;
}

mh_signed_number_t mh_read_int(const mh_reader_t *reader) {
    char num_str[21];
    mh_memory_t mem = mh_memory_reference(num_str, sizeof num_str);
    for (size_t position = 0; position < mem.size; position++) {
        reader->read(reader->instance, &mem, 1);
        if (mem.offset != 1) {
            break;
        } else {
            mem.address = (char *) mem.address + 1;
        }
        if (num_str[position] != '+' && num_str[position] != '-' && !mh_is_number(num_str[position], MH_BASE_DEC)) {
            num_str[position] = 0;
            break;
        }
    }
    mh_signed_number_t num = 0;
    mh_int_parse(num_str, MH_BASE_DEC, &num);
    return num;
}

