#ifndef MHSERV_MH_HANDLE_H
#define MHSERV_MH_HANDLE_H

#include "mh_context.h"

/*
 * <mh_handle.h>
 * The libmh dynamic library header.
 *
 * Contains methods for opening dynamic libraries and finding symbols inside them.
 */

// A handle to a dynamic library.
MH_API_TYPE(mh_handle, struct mh_handle {
    // A destructor used for closing the handle.
    mh_destructor_t destructor;
});

// Load a library and create a handle.
MH_API_FUNC(mh_handle_t *mh_handle_new(mh_context_t *context, mh_const_string_t path));

// Find a symbol inside of a library.
MH_API_FUNC(mh_ref_t mh_handle_find_symbol(mh_handle_t *handle, mh_const_string_t name));

#endif //MHSERV_MH_HANDLE_H
