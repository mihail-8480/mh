#include "../../inc/mh_stack.h"

void mh_stack_push(mh_stack_t *stack, mh_stack_node_t *node) {
    stack->depth++;
    node->prev = stack->last;
    stack->last = node;
}

mh_stack_node_t *mh_stack_pop(mh_stack_t *stack) {
    if (stack->depth == 0 || stack->last == NULL) {
        return NULL;
    }
    stack->depth--;
    mh_stack_node_t *node = stack->last;
    stack->last = node->prev;
    return node;
}

mh_stack_node_t *mh_stack_peek(mh_stack_t *stack) {
    return stack->last;
}
