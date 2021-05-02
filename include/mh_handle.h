#ifndef MHSERV_MH_HANDLE_H
#define MHSERV_MH_HANDLE_H

#include <mh_context.h>

// A handle to a dynamic library
MH_API_TYPE(mh_handle, struct mh_handle {
    mh_destructor_t destructor;
});

// Load a library and create a handle
MH_API_FUNC(mh_handle_t *mh_handle_new(mh_context_t *context, const char *path));

// Find a symbol inside of a library
MH_API_FUNC(void *mh_handle_find_symbol(mh_handle_t *handle, const char *name));

#endif //MHSERV_MH_HANDLE_H
