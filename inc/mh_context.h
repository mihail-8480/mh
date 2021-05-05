#ifndef MHSERV_MH_CONTEXT_H
#define MHSERV_MH_CONTEXT_H

#include "mh.h"
#include <stdbool.h>
#include <stddef.h>
#include <setjmp.h>

// A function pointer that points to the method that is supposed to free memory
typedef void (*mh_destructor_free_t)(void *ptr);

// A pointer to something allocated
MH_API_TYPE(mh_context_allocation, void*);

// A context reference to allocated memory
MH_API_TYPE(mh_context_allocation_reference,struct mh_context_allocation_reference {
    mh_context_allocation_t ptr;
    size_t index;
});

// A destructor structure, should be a part of every other structure that needs freeing
MH_API_TYPE(mh_destructor, struct mh_destructor {
    // The destructor function
    mh_destructor_free_t free;
});

// The context structure, used for error reporting and memory management
MH_API_TYPE(mh_context, struct mh_context {
    short nothing;
});

// A context error handler
typedef bool (*mh_error_handler_t)(mh_context_t *context, const char *message, mh_code_location_t from);

// Destroy a structure that has a mh_destructor as it's first field
MH_API_FUNC(void mh_destroy(mh_destructor_t *object));

// Create a context
MH_API_FUNC(mh_context_t *mh_start(void));

// End a context
MH_API_FUNC(void mh_end(mh_context_t *context));

// Report an error to a context
MH_NORETURN MH_API_FUNC(void mh_context_error(mh_context_t *context, const char *message, mh_code_location_t from));

// Set an error handler to a context
MH_API_FUNC(void mh_context_set_error_handler(mh_context_t *context, mh_error_handler_t handler));

// Catch an error on this thread, returns false on a caught error
MH_API_FUNC(jmp_buf *mh_context_get_jump_buffer(mh_context_t *context));

// Resize memory in the context
MH_API_FUNC(void *mh_context_reallocate(mh_context_t *context, mh_context_allocation_reference_t ref, size_t size));

// Free previously allocated memory
MH_API_FUNC(void mh_context_free(mh_context_t *context, mh_context_allocation_reference_t ref));

// Allocate memory that will get destroyed when the context ends
MH_API_FUNC(mh_context_allocation_reference_t mh_context_allocate(mh_context_t *context, size_t size, bool clear));

// Add a destructor that will be called when the context ends
MH_API_FUNC(void *mh_context_add_destructor(mh_context_t *context, mh_destructor_t *destructor));

// Bind a context to the current thread
MH_API_FUNC(void mh_context_bind_to_thread(mh_context_t *context));

// Get a context that was bound to the current thread
MH_API_FUNC(mh_context_t *mh_context_get_from_thread(void));

#define MH_CONTEXT(name, code) { mh_context_t* name = mh_start(); code mh_end(name);}
#define MH_NULL_REFERENCE(ctx, arg) if (arg == NULL) mh_context_error(ctx, "The reference `" #arg "` is NULL.", MH_LOCATION_ANY())

#define MH_TRY(ctx) if (!setjmp(*mh_context_get_jump_buffer(ctx)))
#define MH_CATCH else

#endif //MHSERV_MH_CONTEXT_H
