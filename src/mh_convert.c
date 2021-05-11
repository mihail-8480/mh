#include "../inc/mh_convert.h"

static inline size_t mh_intlen(mh_unsigned_number_t num) {
    size_t i;
    for (i = 0; num; num/=10, i++);
    return i;
}

static inline mh_unsigned_number_t mh_abs(mh_signed_number_t num) {
    return (num >= 0) ? num : -num;
}

static inline bool mh_strint(mh_memory_t *memory, mh_unsigned_number_t num, size_t end) {
    if (end >= memory->size) {
        return false;
    }
    ((char*)memory->address)[end] = '\0';
    memory->offset = end;
    do {
        ((char*)memory->address)[--end] = '0' + num % 10;
        num /= 10;
    } while(num);
    return true;
}

bool mh_int_to_string(mh_memory_t *memory, mh_signed_number_t num) {
    mh_unsigned_number_t p_num = mh_abs(num);
    size_t end = mh_intlen(p_num) + (num < 0);
    bool res = mh_strint(memory, p_num, end);
    if (num < 0) {
        ((char*)memory->address)[0] = '-';
    }
    return res;
}

bool mh_uint_to_string(mh_memory_t *memory, mh_unsigned_number_t num) {
    return mh_strint(memory, num, mh_intlen(num));
}
