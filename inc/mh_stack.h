#ifndef MH_MH_STACK_H
#define MH_MH_STACK_H

#include "mh.h"

MH_API_TYPE(mh_stack_node, struct mh_stack_node {
    struct mh_stack_node *prev;
});

MH_API_TYPE(mh_stack, struct mh_stack {
    size_t depth;
    mh_stack_node_t *last;
});

MH_API_FUNC(void mh_stack_push(mh_stack_t *stack, mh_stack_node_t *node));
MH_API_FUNC(mh_stack_node_t *mh_stack_pop(mh_stack_t *stack));
MH_API_FUNC(mh_stack_node_t *mh_stack_peek(mh_stack_t *stack));

#endif //MH_MH_STACK_H
