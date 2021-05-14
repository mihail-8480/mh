#include "../inc/mh_convert.h"
#include <math.h>

static inline size_t mh_intlen(mh_unsigned_number_t num, mh_base_t base) {
    return (size_t) ceil(log2(num + 1) / log2(base));
}

static inline mh_unsigned_number_t mh_abs(mh_signed_number_t num) {
    return (num >= 0) ? num : -num;
}

static inline bool mh_strint(mh_memory_t *memory, mh_unsigned_number_t num, size_t end, mh_base_t base, char table[]) {
    if (end >= memory->size) {
        return false;
    }
    ((char *) memory->address)[end] = '\0';
    memory->offset = end;
    do {
        ((char *) memory->address)[--end] = table[num % base];
        num /= base;
    } while (num);
    return true;
}

static char mh_conversion_table[] = {
        '0',
        '1', '2', '3',
        '4', '5', '6',
        '7', '8', '9',
        'a', 'b', 'c',
        'd', 'e', 'f'
};

static inline bool mh_verify_base(mh_base_t base) {
    return base <= sizeof mh_conversion_table && base > 1;
}

bool mh_int_to_string(mh_memory_t *memory, mh_signed_number_t num, mh_base_t base) {
    if (!mh_verify_base(base)) {
        return false;
    }
    mh_unsigned_number_t p_num = mh_abs(num);
    size_t end = mh_intlen(p_num, base) + (num < 0);
    bool res = mh_strint(memory, p_num, end, base, mh_conversion_table);
    if (num < 0) {
        ((char *) memory->address)[0] = '-';
    }
    return res;
}

bool mh_uint_to_string(mh_memory_t *memory, mh_unsigned_number_t num, mh_base_t base) {
    if (!mh_verify_base(base)) {
        return false;
    }
    return mh_strint(memory, num, mh_intlen(num, base), base, mh_conversion_table);
}
