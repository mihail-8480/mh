#ifndef MHSERV_MH_COLLECTION_H
#define MHSERV_MH_COLLECTION_H

#include <mh_context.h>
#include <mh_memory.h>

// Something to iterate over
MH_API_TYPE(mh_iterator, struct mh_iterator {
    // For mh_iterator_start()
    void (*start)(struct mh_iterator *iterator);
    // For mh_iterator_next()
    bool (*next)(struct mh_iterator *iterator);

    // For mh_iterator_current()
    mh_memory_t (*current)(struct mh_iterator *iterator);
});

// A collection
MH_API_TYPE(mh_collection, struct mh_collection {
    mh_destructor_t destructor;
    // For mh_collection_get_iterator()
    mh_iterator_t *(*get_iterator)(struct mh_collection *collection);
});

// Set the iterator to the beginning
MH_API_FUNC(void mh_iterator_start(mh_iterator_t *iterator));

// Move to the next element
MH_API_FUNC(bool mh_iterator_next(mh_iterator_t *iterator));

// Get the current element
MH_API_FUNC(mh_memory_t mh_iterator_current(mh_iterator_t *iterator));

// Create an iterator from a collection
MH_API_FUNC(mh_iterator_t *mh_collection_get_iterator(mh_collection_t *collection));


#endif //MHSERV_MH_COLLECTION_H