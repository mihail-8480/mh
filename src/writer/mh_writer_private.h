#ifndef MH_MH_WRITER_PRIVATE_H
#define MH_MH_WRITER_PRIVATE_H

#include "../../inc/mh_writer.h"

struct mh_writer {
    void (*write)(void *instance, mh_memory_t *mem, size_t count);

    void *instance;
};

#endif //MH_MH_WRITER_PRIVATE_H
