#include "../inc/mh.h"
#include <stdio.h>

mh_version_t mh_get_version(void) {
    return (mh_version_t) {
            .major = MH_VERSION_MAJOR,
            .minor = MH_VERSION_MINOR,
            .patch = MH_VERSION_PATCH
    };
}
