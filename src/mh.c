#include "../inc/mh.h"
#include <stdio.h>

mh_version_t mh_get_version(void) {
    return (mh_version_t) {
        .major = MH_VERSION_MAJOR,
        .minor = MH_VERSION_MINOR,
        .patch = MH_VERSION_PATCH
    };
}

void mh_code_location_to_string(char* str,mh_code_location_t location) {
    if (location.function_address != 0) {
        sprintf(str, "in %s()[%zu] at %s:%d", location.function_name, location.function_address, location.file_name, location.file_line);
    } else {
        sprintf(str, "in %s() at %s:%d", location.function_name, location.file_name, location.file_line);
    }
}
