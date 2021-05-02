#ifndef MH_MH_H
#define MH_MH_H

#include <stddef.h>
#include <stdbool.h>

// Get the number of elements in a fixed size array.
#define MH_FIXED_COUNT(array) (sizeof array / sizeof array[0])

// This may not be used.
#define MH_UNUSED __attribute__((unused))

// This function does not return.
#define MH_NORETURN __attribute__((noreturn))

// Define a MH API function.
#define MH_API_FUNC(x) MH_UNUSED extern x

// Turn a function name into an error location.
#define MH_LOCATION(x) ((mh_code_location_t){.file_name = __FILE__, .file_line = __LINE__, .function_name = #x, .function_address = (size_t)x})
#define MH_LOCATION_ANY() ((mh_code_location_t){.file_name = __FILE__, .file_line = __LINE__, .function_name = __func__, .function_address = (size_t)NULL})

// Define an MH API type.
#define MH_API_TYPE(name, code) typedef code name##_t

// Define a variable named this.
#define MH_THIS(type, expression) type this = (type)expression

// Version information.
MH_API_TYPE(mh_version, struct mh_version {
    const unsigned short major;
    const unsigned short minor;
    const unsigned short patch;
});

// A location in the code.
MH_API_TYPE(mh_code_location, struct mh_code_location {
    const char *file_name;
    unsigned int file_line;
    const char *function_name;
    size_t function_address;
});

// Get the current version.
MH_API_FUNC(mh_version_t mh_get_version(void));

// Turn a code location to a string.
MH_API_FUNC(void mh_code_location_to_string(char* str,mh_code_location_t location));

#endif //MH_MH_H
