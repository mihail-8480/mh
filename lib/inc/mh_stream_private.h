#ifndef MHSERV_MH_STREAM_PRIVATE_H
#define MHSERV_MH_STREAM_PRIVATE_H

#include "../../inc/mh_memory.h"
#include <stdlib.h>
#include <string.h>

// The private stream structure, it contains all the methods required for a stream implementation
typedef struct mh_stream_private {
    mh_stream_t base;

    void (*write)(mh_ref_t stream, mh_memory_t *buffer, size_t count);

    void (*read)(mh_ref_t stream, mh_memory_t *buffer, size_t count);

    void (*seek)(mh_ref_t stream, size_t position);

    void (*flush)(mh_ref_t stream);

    size_t (*get_position)(mh_ref_t stream);

    size_t (*get_size)(mh_ref_t stream);

    bool can_read;
    bool can_write;
    bool can_seek;
    mh_context_t *context;
} mh_stream_private_t;

#endif //MHSERV_MH_STREAM_PRIVATE_H
