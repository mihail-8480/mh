#include "../../inc/mh_list.h"

struct mh_list_node {
    mh_memory_t value;
    struct mh_list_node *next;
    struct mh_list_node *previous;
    size_t allocation_index;
};

typedef struct mh_list_private {
    mh_list_t base;
    mh_context_t *context;
    mh_list_node_t *first;
    mh_list_node_t *last;
    size_t count;
} mh_list_private_t;

typedef struct mh_list_iterator {
    mh_iterator_t iterator;
    mh_list_node_t *current;
    mh_list_private_t *list;
} mh_list_iterator_t;

static mh_memory_t mh_list_iterator_current(mh_iterator_t *iterator) {
    MH_THIS(mh_list_iterator_t*, iterator);
    if (this->current == NULL) {
        return MH_MEM_NULL;
    }
    return this->current->value;
}

static bool mh_list_iterator_next(mh_iterator_t* iterator) {
    MH_THIS(mh_list_iterator_t*, iterator);
    if (this->current == NULL || this->current->next == NULL) {
        return false;
    }
    this->current = this->current->next;
    return true;
}

static void mh_list_iterator_start(mh_iterator_t *iterator) {
    MH_THIS(mh_list_iterator_t*, iterator);
    this->current = this->list->first;
}

static mh_iterator_t* mh_list_get_iterator(mh_collection_t *collection) {
    MH_THIS(mh_list_private_t*, collection);
    mh_list_iterator_t* iterator = mh_context_allocate(this->context, sizeof(mh_list_iterator_t), false).ptr;
    *iterator = (mh_list_iterator_t){
        .list = this,
        .current = this->first,
        .iterator = {
                .current = mh_list_iterator_current,
                .next = mh_list_iterator_next,
                .start = mh_list_iterator_start
        }
    };
    return &iterator->iterator;
}

mh_list_t *mh_list_new(mh_context_t* context){
    MH_THIS(mh_list_private_t*,mh_context_allocate(context, sizeof(mh_list_private_t), false).ptr);
    MH_NULL_REFERENCE(context, this);

    *this = (mh_list_private_t){
        .first = NULL,
        .last = NULL,
        .count = 0,
        .context = context,
        .base.collection = {
                .destructor.free = NULL,
                .get_iterator = mh_list_get_iterator
        }
    };
    return &this->base;
}

mh_memory_t mh_list_node_value(mh_list_node_t *node) {
    return node->value;
}


mh_list_node_t *mh_list_node_new(mh_context_t* context, const mh_memory_t ref) {
    mh_context_allocation_reference_t alloc = mh_context_allocate(context, sizeof(mh_list_node_t), false);

    MH_THIS(mh_list_node_t*, alloc.ptr);
    MH_NULL_REFERENCE(context, this);

    this->allocation_index = alloc.index;
    this->value = ref;
    this->next = NULL;
    this->previous = NULL;

    return this;
}

size_t mh_list_count(mh_list_t *list) {
    MH_THIS(mh_list_private_t*, list);
    return this->count;
}
void mh_list_append(mh_list_t *list, const mh_memory_t ref) {
    MH_THIS(mh_list_private_t*, list);
    mh_list_node_t* node = mh_list_node_new(this->context, ref);
    if (this->last != NULL) {
        this->last->next = node;
    }
    node->previous = this->last;
    this->last = node;
    this->count++;
}


mh_list_node_t *mh_list_node_next(mh_list_node_t *node) {
    return node->next;
}

mh_list_node_t *mh_list_node_previous(mh_list_node_t *node) {
    return node->previous;
}

mh_list_node_t *mh_list_last(mh_list_t* list) {
    MH_THIS(mh_list_private_t*, list);
    return this->last;
}

mh_list_node_t *mh_list_first(mh_list_t* list) {
    MH_THIS(mh_list_private_t*, list);
    return this->first;
}

mh_list_node_t *mh_list_at_index(mh_list_t* list, size_t index) {
    MH_THIS(mh_list_private_t*, list);
    if (index >= this->count) {
        mh_context_error(this->context, "The index is out of range.", MH_LOCATION(mh_list_at_index));
    }
    mh_list_node_t* node = this->first;
    for (size_t i = 0; i < index; i++) {
        node = node->next;
    }
    return node;
}

void mh_list_prepend(mh_list_t *list, const mh_memory_t ref) {
    MH_THIS(mh_list_private_t*, list);
    mh_list_node_t* node = mh_list_node_new(this->context, ref);
    if (this->first != NULL) {
        this->first->previous = node;
    }
    node->next = this->first;
    this->first = node;
    this->count++;
}


void mh_list_remove(mh_list_t *list, mh_list_node_t *node) {
    MH_THIS(mh_list_private_t*, list);
    MH_NULL_REFERENCE(this->context, node);

    if (node == this->first) {
        this->first = node->next;
    } else {
        node->previous->next = node->next;
    }

    if (node == this->last) {
        this->last = node->previous;
    } else {
        node->next->previous = node->previous;
    }

    this->count--;
    mh_context_free(this->context, (mh_context_allocation_reference_t){.ptr = node, .index = node->allocation_index});
}
