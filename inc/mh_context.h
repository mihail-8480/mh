#ifndef MHSERV_MH_CONTEXT_H
#define MHSERV_MH_CONTEXT_H

#include "mh.h"
#include "mh_stack.h"
#include <stdbool.h>
#include <stddef.h>
#include <setjmp.h>

/*
 * <mh_context.h>
 * The libmh context library.
 *
 * Contains error handling and memory management functions.
 */

// A function pointer that points to the method that is supposed to free memory.
typedef void (*mh_destructor_free_t)(void *ptr);

// A pointer to something allocated.
MH_API_TYPE(mh_context_allocation, void*);

// A context reference to allocated memory.
MH_API_TYPE(mh_context_allocation_reference, struct mh_context_allocation_reference {
    // The pointer of the allocation.
    mh_context_allocation_t ptr;
    // The index of the allocation in memory so we don't have to iterate trough the allocation array.
    size_t index;
});

// A stack node that contains a jump buffer.
MH_API_TYPE(mh_context_jump_stack_node, struct mh_context_jump_stack_node {
    // The base type.
    mh_stack_node_t node;
    // The jump buffer.
    jmp_buf jmp;
});

// A destructor structure, should be a part of every other structure that needs freeing.
MH_API_TYPE(mh_destructor, struct mh_destructor {
    // The destructor function.
    mh_destructor_free_t free;
});

// The context structure, used for error reporting and memory management.
MH_API_TYPE(mh_context, struct mh_context);

// A context error handler.
typedef bool (*mh_error_handler_t)(mh_context_t *context, const char *message, mh_code_location_t from);

// Destroy a structure that has a mh_destructor as it's first field.
MH_API_FUNC(void mh_destroy(mh_destructor_t *object));

// Create a context.
MH_API_FUNC(mh_context_t *mh_start(void));

// End a context.
MH_API_FUNC(void mh_end(mh_context_t *context));

// Report an error to a context.
MH_NORETURN MH_API_FUNC(void mh_context_error(mh_context_t *context, const char *message, mh_code_location_t from));

// Set an error handler to a context.
MH_API_FUNC(void mh_context_set_error_handler(mh_context_t *context, mh_error_handler_t handler));

// Push a jump buffer for error handling.
MH_API_FUNC(void mh_context_push_jump(mh_context_t *context, mh_context_jump_stack_node_t *jump));

// Remove a jump buffer if it's the latest one.
MH_API_FUNC(void mh_context_remove_jump(mh_context_t *context, mh_context_jump_stack_node_t *jump));

// Resize memory in the context.
MH_API_FUNC(void *mh_context_reallocate(mh_context_t *context, mh_context_allocation_reference_t ref, size_t size));

// Free previously allocated memory.
MH_API_FUNC(void mh_context_free(mh_context_t *context, mh_context_allocation_reference_t ref));

// Allocate memory that will get destroyed when the context ends.
MH_API_FUNC(mh_context_allocation_reference_t mh_context_allocate(mh_context_t *context, size_t size, bool clear));

// Add a destructor that will be called when the context ends.
MH_API_FUNC(void *mh_context_add_destructor(mh_context_t *context, mh_destructor_t *destructor));

// Bind a context to the current thread.
MH_API_FUNC(void mh_context_bind_to_thread(mh_context_t *context));

// Get a context that was bound to the current thread.
MH_API_FUNC(mh_context_t *mh_context_get_from_thread(void));

// The global context.
extern mh_context_t *mh_global_context;

// The global context.
#define MH_GLOBAL mh_global_context

// Check for a null reference and report the error.
#define MH_NULL_REFERENCE(ctx, arg) if (arg == NULL) mh_context_error(ctx, "The reference `" #arg "` is NULL.", MH_LOCATION_ANY())

// Try executing some code.
#define MH_TRY(name, ctx) mh_context_jump_stack_node_t name; mh_context_push_jump(ctx, &name); if (!setjmp(name.jmp))

// If a MH_TRY fails, it'll fallback to this.
#define MH_CATCH(name, ctx, code) else code mh_context_remove_jump(ctx, &name)

// Throw an error.
#define MH_THROW(ctx, message) mh_context_error(ctx, message, MH_LOCATION_ANY())

#endif //MHSERV_MH_CONTEXT_H
