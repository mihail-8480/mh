#ifndef MH_MH_LIST_H
#define MH_MH_LIST_H

#include "mh_collection.h"

/*
 * <mh_list.h>
 * The libmh list header.
 *
 * Contains a linked list type.
 */

// An element of a list.
MH_API_TYPE(mh_list_node, struct mh_list_node);

// A list type.
MH_API_TYPE(mh_list, struct mh_list {
    // For iteration.
    mh_collection_t collection;
});

// Create a new list.
MH_API_FUNC(mh_list_t *mh_list_new(mh_context_t *context));

// Get the last element of a list.
MH_API_FUNC(mh_list_node_t *mh_list_last(mh_list_t *list));

// Get the first element of a list.
MH_API_FUNC(mh_list_node_t *mh_list_first(mh_list_t *list));

// Iterate trough the list until a certain index is reached and return the element found there.
MH_API_FUNC(mh_list_node_t *mh_list_at_index(mh_list_t *list, size_t index));

// Add something to the end of a list.
MH_API_FUNC(void mh_list_append(mh_list_t *list, const mh_memory_t ref));

// Add something to the beginning of a list.
MH_API_FUNC(void mh_list_prepend(mh_list_t *list, const mh_memory_t ref));

// Remove a node in the list.
MH_API_FUNC(void mh_list_remove(mh_list_t *list, mh_list_node_t *node));

// Return the number of elements in the list.
MH_API_FUNC(size_t mh_list_count(mh_list_t *list));

// Get a pointer to a value of a list node.
MH_API_FUNC(mh_memory_t *mh_list_node_value(mh_list_node_t *node));

// Get the previous sibling of a list node.
MH_API_FUNC(mh_list_node_t *mh_list_node_previous(mh_list_node_t *node));

// Get the next sibling of a list node.
MH_API_FUNC(mh_list_node_t *mh_list_node_next(mh_list_node_t *node));

#endif //MH_MH_LIST_H
