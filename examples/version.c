#include "lib/example.h"

COMMAND(version) {
    mh_version_t version = mh_get_version();
    mh_const_string_t commit = mh_get_git_hash();
    MH_WRITE("libmh version {}.{}.{} (commit {})\n",
             MH_FMT_UINT(version.major),
             MH_FMT_UINT(version.minor),
             MH_FMT_UINT(version.patch),
             MH_FMT_STR(commit));
}
