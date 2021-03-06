#include "../../inc/mh_handle.h"
#include <dlfcn.h>

typedef struct mh_handle_private {
    mh_handle_t base;
    mh_ref_t handle;
    mh_context_t *context;
} mh_handle_private_t;

void mh_handle_destroy(mh_ref_t ptr) {
    // Wrapper for dlclose()
    MH_THIS(mh_handle_private_t*, ptr);
    dlclose(this->handle);
}

mh_handle_t *mh_handle_new(mh_context_t *context, mh_const_string_t path) {
    // Wrapper for dlopen()
    MH_THIS(mh_handle_private_t*,
            (mh_handle_private_t *) mh_context_allocate(context, sizeof(mh_handle_private_t), false).ptr);
    mh_ref_t handle = dlopen(path, RTLD_LAZY);
    if (!handle) {
        MH_THROW(context, dlerror());
    }
    *this = (mh_handle_private_t) {
            .context = context,
            .base.destructor = {mh_handle_destroy},
            .handle = handle
    };
    mh_context_add_destructor(context, &this->base.destructor);
    return (mh_handle_t *) this;
}

mh_ref_t mh_handle_find_symbol(mh_handle_t *handle, mh_const_string_t name) {
    // Wrapper for dlsym()
    MH_THIS(mh_handle_private_t*, handle);
    mh_ref_t sym = dlsym(this->handle, name);
    if (!sym) {
        MH_THROW(this->context, dlerror());
    }
    return sym;
}
