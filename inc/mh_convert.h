#ifndef MH_MH_CONVERT_H
#define MH_MH_CONVERT_H

#include "mh_memory.h"

// A signed number.
MH_API_TYPE(mh_signed_number, int64_t);

// An unsigned number.
MH_API_TYPE(mh_unsigned_number, u_int64_t);

// Convert a signed number to a string and move the offset to the amount of characters written.
MH_API_FUNC(bool mh_int_to_string(mh_memory_t *memory, mh_signed_number_t num));

// Convert an unsigned number to a string and move the offset to the amount of characters written.
MH_API_FUNC(bool mh_uint_to_string(mh_memory_t *memory, mh_unsigned_number_t num));

#endif //MH_MH_CONVERT_H
