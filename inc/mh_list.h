#ifndef MH_MH_LIST_H
#define MH_MH_LIST_H

#include "mh_collection.h"

MH_API_TYPE(mh_list_node, struct mh_list_node);

MH_API_TYPE(mh_list, struct mh_list {
    mh_collection_t collection;
});

MH_API_FUNC(mh_list_t *mh_list_new(mh_context_t *context));

MH_API_FUNC(mh_list_node_t *mh_list_last(mh_list_t *list));

MH_API_FUNC(mh_list_node_t *mh_list_first(mh_list_t *list));

MH_API_FUNC(mh_list_node_t *mh_list_at_index(mh_list_t *list, size_t index));

MH_API_FUNC(void mh_list_append(mh_list_t *list, const mh_memory_t ref));

MH_API_FUNC(void mh_list_prepend(mh_list_t *list, const mh_memory_t ref));

MH_API_FUNC(void mh_list_remove(mh_list_t *list, mh_list_node_t *node));

MH_API_FUNC(size_t mh_list_count(mh_list_t *list));

MH_API_FUNC(mh_memory_t *mh_list_node_value(mh_list_node_t *node));

MH_API_FUNC(mh_list_node_t *mh_list_node_previous(mh_list_node_t *node));

MH_API_FUNC(mh_list_node_t *mh_list_node_next(mh_list_node_t *node));

// todo: implement & write tests for the list
MH_API_FUNC(void mh_list_contains_value(mh_list_t *list, const mh_memory_t ref));

MH_API_FUNC(void mh_list_contains_node(mh_list_t *list, mh_list_node_t *ref));

#endif //MH_MH_LIST_H
