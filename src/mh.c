#include "../inc/mh.h"

mh_version_t mh_get_version(void) {
    return (mh_version_t) {
            .major = MH_VERSION_MAJOR,
            .minor = MH_VERSION_MINOR,
            .patch = MH_VERSION_PATCH
    };
}

mh_const_string_t mh_get_git_hash(void) {
#ifdef MH_GIT_HASH
    return MH_GIT_HASH;
#else
    return "Unknown";
#endif
}