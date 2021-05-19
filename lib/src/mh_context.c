#include "../../inc/mh_context.h"
#include <stdlib.h>
#include <setjmp.h>

// The container for the context's variables
struct mh_context {
    size_t allocation_count;
    size_t allocation_size;
    mh_context_allocation_t *allocations;
    size_t destructor_count;
    size_t destructor_size;
    mh_destructor_t **destructors;
    mh_error_handler_t error_handler;
    mh_stack_t jump_stack;
    mh_stack_t recycled_allocations;
};
mh_context_t *mh_global_context;

typedef struct mh_recycled_node {
    mh_stack_node_t node;
    size_t index;
} mh_recycled_node_t;

MH_CONSTRUCTOR(101) void mh_context_create_global(void) {
    MH_INFO("mh_context_create_global():\n");
    mh_global_context = mh_start();
}


MH_DESTRUCTOR void mh_context_destroy_global(void) {
    MH_INFO("mh_context_destroy_global():\n");
    mh_end(mh_global_context);
}

void mh_destroy(mh_destructor_t *object) {
    MH_INFO("mh_destroy(%zu):\n", (size_t) object);
    // If the object isn't null or it's destructor function pointer isn't null
    if (object != NULL && object->free != NULL) {
        // Call the destructor function pointer
        object->free(object);
        MH_INFO("(mh_destroy)-- success\n");
    } else {
        MH_INFO("(mh_destroy)-- did nothing\n");
    }
}

mh_context_t *mh_start(void) {
    MH_INFO("mh_start():\n");

    // Create the context structure and set some default values
    MH_THIS(mh_context_t*, malloc(sizeof(mh_context_t)));
    *this = (mh_context_t) {
            .allocation_count = 0,
            .allocation_size = 32,
            .allocations = malloc(sizeof(mh_context_allocation_t) * 32),
            .destructor_count = 0,
            .destructor_size = 32,
            .destructors = malloc(sizeof(mh_destructor_t *) * 32),
            .error_handler = NULL,
            .jump_stack = {
                    .depth = 0,
                    .last = NULL
            },
            .recycled_allocations = {
                    .depth = 0,
                    .last = NULL
            }
    };
    MH_INFO("(mh_start)-- returned %zu\n", (size_t) this);
    return this;
}

void mh_end(mh_context_t *context) {
    MH_INFO("mh_end(%zu):\n", (size_t) context);
    MH_THIS(mh_context_t*, context);

    // Call every destructor and free the destructor array
    for (size_t i = 0; i < this->destructor_count; i++) {
        mh_destroy(this->destructors[i]);
    }
    free(this->destructors);

    // Free every allocation and the allocation array
    for (size_t i = 0; i < this->allocation_count; i++) {
        MH_INFO("(mh_end [%zu])-- freeing [%zu] (%zu)\n", (size_t) context, i, (size_t) this->allocations[i]);
        if (this->allocations[i] != NULL) {
            free(this->allocations[i]);
        }
    }
    free(this->allocations);

    while (this->recycled_allocations.depth) {
        mh_ref_t node = mh_stack_pop(&this->recycled_allocations);
        MH_INFO("(mh_end [%zu])-- removing recycled allocation index node [%zu] (%zu)\n", (size_t) context,
                this->recycled_allocations.depth, (size_t) node);
        free(node);
    }

    free(this);
    MH_INFO("(mh_end)-- success\n");
}

mh_context_allocation_reference_t mh_context_allocate(mh_context_t *context, size_t size, bool clear) {
    MH_THIS(mh_context_t*, context);
    MH_INFO("mh_context_allocate(%zu, %zu, %d):\n", (size_t) context, size, clear);

    size_t index;

    // If you can recycle a previous index, do it.
    if (this->recycled_allocations.depth > 0) {

        mh_recycled_node_t *node = (mh_recycled_node_t *) mh_stack_pop(&this->recycled_allocations);
        index = node->index;
        MH_INFO("(mh_context_allocate)-- allocating memory with a recycled index: %zu\n", node->index);
        free(node);
    } else {
        // If you can't get a new index and resize the array if there isn't enough space.
        index = this->allocation_count++;
        MH_INFO("(mh_context_allocate)-- allocating memory with a new index: %zu\n", index);
        if (index >= this->allocation_size) {
            this->allocation_size *= 2;
            MH_INFO("(mh_context_allocate)-- resizing allocations array to %zu\n", this->allocation_size);
            this->allocations = realloc(this->allocations, sizeof(mh_context_allocation_t) * this->allocation_size);
        }
    }

    // If clear is true use calloc, if not use malloc
    mh_ref_t ptr;
    if (!clear) {
        ptr = malloc(size);
    } else {
        ptr = calloc(1, size);
    }

    MH_NULL_REFERENCE(context, ptr);

    this->allocations[index] = ptr;
    MH_INFO("(mh_context_allocate)-- allocated [%zu] (%zu)\n", index, (size_t) ptr);
    return (mh_context_allocation_reference_t) {.index = index, .ptr = ptr};
}

mh_ref_t mh_context_reallocate(mh_context_t *context, mh_context_allocation_reference_t ref, size_t size) {
    MH_THIS(mh_context_t*, context);
    MH_INFO("mh_context_reallocate(%zu, {%zu,%zu}, %zu):\n", (size_t) context, (size_t) ref.ptr, ref.index, size);

    if (ref.index >= this->allocation_count || this->allocations[ref.index] != ref.ptr) {
        MH_THROW(context, "Invalid allocation reference.");
    }

    mh_ref_t ptr = realloc(ref.ptr, size);

    MH_NULL_REFERENCE(context, ptr);

    this->allocations[ref.index] = ptr;
    MH_INFO("(mh_context_reallocate)-- resized %zu to %zu - resulted in %zu\n", (size_t) ref.ptr, size,
            (size_t) this->allocations[ref.index]);
    return this->allocations[ref.index];
}

void mh_context_free(mh_context_t *context, mh_context_allocation_reference_t ref) {
    MH_THIS(mh_context_t*, context);
    MH_INFO("mh_context_free(%zu, {%zu,%zu}):\n", (size_t) context, (size_t) ref.ptr, ref.index);

    if (ref.index >= this->allocation_count || this->allocations[ref.index] != ref.ptr) {
        MH_THROW(context, "Invalid allocation reference.");
    }

    free(ref.ptr);
    this->allocations[ref.index] = NULL;

    mh_recycled_node_t *node = malloc(sizeof(mh_recycled_node_t));
    node->index = ref.index;
    mh_stack_push(&this->recycled_allocations, &node->node);
}

mh_ref_t mh_context_add_destructor(mh_context_t *context, mh_destructor_t *destructor) {
    MH_THIS(mh_context_t*, context);
    MH_INFO("mh_context_add_destructor(%zu, %zu):\n", (size_t) context, (size_t) destructor);

    // Double the destructor array if there isn't enough space
    if (this->destructor_count + 1 >= this->destructor_size) {
        this->destructor_size *= 2;
        MH_INFO("(mh_context_add_destructor)-- resizing destructors array to %zu\n", this->destructor_size);
        this->destructors = realloc(this->destructors, sizeof(mh_destructor_t *) * this->destructor_size);
    }

    // Add the destructor to the destructors array
    this->destructors[this->destructor_count++] = destructor;
    MH_INFO("(mh_context_add_destructor)-- added destructor [%zu] (%zu)\n", this->destructor_count - 1,
            (size_t) destructor);
    return destructor;
}

void mh_context_error(mh_context_t *context, mh_const_string_t message, mh_code_location_t from) {
    MH_THIS(mh_context_t*, context);
    MH_INFO("mh_context_error(%zu, %s, %s at %s:%u)\n", (size_t) context, message, from.function_name, from.file_name,
            from.file_line);

    if (this->jump_stack.depth) {
        mh_context_jump_stack_node_t *node = ((mh_context_jump_stack_node_t *) mh_stack_pop(&this->jump_stack));
        if (node != NULL) {
            longjmp(node->jmp, true);
        }
    } else if (this->error_handler != NULL) {
        this->error_handler(context, message, from);
    }

    // Destroy the context and crash the program
    mh_end(context);
    abort();
}


void mh_context_push_jump(mh_context_t *context, mh_context_jump_stack_node_t *jump) {
    MH_THIS(mh_context_t*, context);
    mh_stack_push(&this->jump_stack, &jump->node);
}

void mh_context_remove_jump(mh_context_t *context, mh_context_jump_stack_node_t *jump) {
    MH_THIS(mh_context_t*, context);
    if ((mh_context_jump_stack_node_t *) mh_stack_peek(&this->jump_stack) == jump) {
        mh_stack_pop(&this->jump_stack);
    }
}

void mh_context_set_error_handler(mh_context_t *context, mh_error_handler_t handler) {
    MH_THIS(mh_context_t*, context);
    this->error_handler = handler;
}

MH_PER_THREAD mh_context_t *mh_context_current_thread = NULL;

void mh_context_bind_to_thread(mh_context_t *context) {
    if (mh_context_current_thread != NULL) {
        mh_end(mh_context_current_thread);
    }
    mh_context_current_thread = context;
}

mh_context_t *mh_context_get_from_thread(void) {
    return mh_context_current_thread;
}
