#ifndef MH_MH_READER_PRIVATE_H
#define MH_MH_READER_PRIVATE_H

#include "../../inc/mh_reader.h"

struct mh_reader {
    void (*read)(mh_ref_t instance, mh_memory_t *mem, size_t count);

    mh_ref_t instance;
};

#endif //MH_MH_READER_PRIVATE_H
