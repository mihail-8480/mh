#include "../inc/mh_convert.h"
#include <math.h>

static inline size_t mh_intlen10(mh_unsigned_number_t num) {
    return (size_t)ceil(log10(num+1));
}

static inline mh_unsigned_number_t mh_abs(mh_signed_number_t num) {
    return (num >= 0) ? num : -num;
}

static inline bool mh_strint10(mh_memory_t *memory, mh_unsigned_number_t num, size_t end) {
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
    size_t end = mh_intlen10(p_num) + (num < 0);
    bool res = mh_strint10(memory, p_num, end);
    if (num < 0) {
        ((char*)memory->address)[0] = '-';
    }
    return res;
}

bool mh_uint_to_string(mh_memory_t *memory, mh_unsigned_number_t num) {
    return mh_strint10(memory, num, mh_intlen10(num));
}
