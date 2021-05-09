#ifndef MH_MH_H
#define MH_MH_H

#include <stddef.h>
#include <stdbool.h>

/*
 * <mh.h>
 * The libmh base header.
 *
 * Contains macros that this library uses and some functions.
 */

// Get the number of elements in a fixed size array.
#define MH_FIXED_COUNT(array) (sizeof array / sizeof array[0])

// This may not be used.
#define MH_UNUSED __attribute__((unused))

// Intended fallthrough.
#define MH_FALLTHROUGH __attribute__ ((fallthrough));

// This is a constructor.
#define MH_CONSTRUCTOR __attribute__((constructor))

// This is a destructor.
#define MH_DESTRUCTOR __attribute__((destructor))

// One per thread.
#define MH_PER_THREAD __thread

// This function does not modify the state of the program.
#define MH_PURE __attribute__ ((pure))

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
    // Major version number.
    const unsigned short major;
    // Minor version number.
    const unsigned short minor;
    // Patch version number.
    const unsigned short patch;
});

// A location in the code.
MH_API_TYPE(mh_code_location, struct mh_code_location {
    // The name of the file where the code is located at.
    const char *file_name;
    // The line of the file where the code is located at.
    unsigned int file_line;
    // The function name where this came from.
    const char *function_name;
    // The address (or 0) of the function pointer to the function where this came from.
    size_t function_address;
});

// Get the current version.
MH_PURE MH_API_FUNC(mh_version_t mh_get_version(void));

// Turn a code location to a string.
MH_API_FUNC(void mh_code_location_to_string(char *str, mh_code_location_t location));


#ifdef MH_DEBUG
#include <stdio.h>
// Print something to the screen.
#define MH_INFO(...) printf(__VA_ARGS__)
#else

// Doesn't do anything.
static inline void mh_nothing() {

}
// Doesn't do anything.
#define MH_INFO(...) mh_nothing()
#endif


#endif //MH_MH_H
