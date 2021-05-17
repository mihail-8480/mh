#ifndef MH_MH_READER_PRIVATE_H
#define MH_MH_READER_PRIVATE_H

#include "../../inc/mh_reader.h"

struct mh_reader {
    void (*read)(void *instance, mh_memory_t *mem, size_t count);

    void *instance;
};

#endif //MH_MH_READER_PRIVATE_H
