#include "../inc/mh_convert.h"
#include <math.h>

static mh_const_string_t zero = "0";
static const char mh_conversion_table[] = {
        '0',
        '1', '2', '3',
        '4', '5', '6',
        '7', '8', '9',
        'a', 'b', 'c',
        'd', 'e', 'f'
};

static inline size_t mh_intlen(mh_unsigned_number_t num, mh_base_t base) {
    return (size_t) ceil(log2(num + 1) / log2(base));
}

static inline mh_unsigned_number_t mh_abs(mh_signed_number_t num) {
    return (num >= 0) ? num : -num;
}

static inline bool mh_strint(mh_memory_t *memory, mh_unsigned_number_t num, size_t end, mh_base_t base) {
    if (end >= memory->size) {
        return false;
    }
    ((char *) memory->address)[end] = '\0';
    memory->offset = end;
    do {
        ((char *) memory->address)[--end] = mh_conversion_table[num % base];
        num /= base;
    } while (num);
    return true;
}

static inline bool mh_verify_base(mh_base_t base) {
    return base <= sizeof mh_conversion_table && base > 1;
}

static inline bool mh_zero(mh_memory_t *memory) {
    strcpy(memory->address, zero);
    memory->offset = 1;
    return true;
}

bool mh_int_to_string(mh_memory_t *memory, mh_signed_number_t num, mh_base_t base) {
    if (!mh_verify_base(base)) {
        return false;
    }

    if (num == 0 && memory->size > 1) {
        return mh_zero(memory);
    }

    mh_unsigned_number_t p_num = mh_abs(num);
    size_t end = mh_intlen(p_num, base) + (num < 0);
    bool res = mh_strint(memory, p_num, end, base);
    if (num < 0) {
        ((char *) memory->address)[0] = '-';
    }
    return res;
}

bool mh_uint_to_string(mh_memory_t *memory, mh_unsigned_number_t num, mh_base_t base) {
    if (!mh_verify_base(base)) {
        return false;
    }

    if (num == 0 && memory->size > 1) {
        return mh_zero(memory);
    }

    return mh_strint(memory, num, mh_intlen(num, base), base);
}

static inline signed char mh_chr_to_int(const char chr) {
    for (size_t i = 0; i < sizeof(mh_conversion_table); i++) {
        if (mh_conversion_table[i] == chr) {
            return (char) i;
        }
    }
    return -1;
}

static inline bool mh_intstr(mh_const_string_t str, mh_base_t base, mh_unsigned_number_t *out) {
    mh_unsigned_number_t number = 0;
    for (; *str; str++) {
        signed char num = mh_chr_to_int(*str);
        if (num == -1) {
            return false;
        }
        number = (number * base) + num;
    }
    *out = number;
    return true;
}

bool mh_int_parse(mh_const_string_t str, mh_base_t base, mh_signed_number_t *out) {
    mh_signed_number_t sign = 1;
    if (*str != 0 && str[0] == '-') {
        sign = -1;
        str++;
    }
    if (*str != 0 && str[0] == '+') {
        sign = 1;
        str++;
    }
    mh_unsigned_number_t u_num = 0;
    bool res = mh_intstr(str, base, &u_num);
    if (!res) {
        return false;
    }
    *out = u_num * sign;
    return true;
}

bool mh_uint_parse(mh_const_string_t str, mh_base_t base, mh_unsigned_number_t *out) {
    return mh_intstr(str, base, out);
}
