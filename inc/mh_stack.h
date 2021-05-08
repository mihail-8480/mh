#ifndef MH_MH_STACK_H
#define MH_MH_STACK_H

#include "mh.h"

/*
 * <mh_stack.h>
 * The libmh stack header.
 *
 * Contains a stack-like linked-list type.
 */

// A node inside a stack.
MH_API_TYPE(mh_stack_node, struct mh_stack_node {
    // The previous element in the parent stack.
    struct mh_stack_node *prev;
});

// A stack.
MH_API_TYPE(mh_stack, struct mh_stack {
    // The depth of the stack.
    size_t depth;
    // The last element of the stack.
    mh_stack_node_t *last;
});

// Push something to the top of a stack.
MH_API_FUNC(void mh_stack_push(mh_stack_t *stack, mh_stack_node_t *node));

// Return and remove the element at the top of a stack.
MH_API_FUNC(mh_stack_node_t *mh_stack_pop(mh_stack_t *stack));

// Return the element at the top of a stack.
MH_API_FUNC(mh_stack_node_t *mh_stack_peek(mh_stack_t *stack));

#endif //MH_MH_STACK_H
