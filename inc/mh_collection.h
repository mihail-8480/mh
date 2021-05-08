#ifndef MHSERV_MH_COLLECTION_H
#define MHSERV_MH_COLLECTION_H

#include "mh_context.h"
#include "mh_memory.h"

/*
 * <mh_collection.h>
 * The libmh collection manipulation header.
 *
 * Contains functions for iteration.
 */


// The key/value pair structure
MH_API_TYPE(mh_key_value_pair, struct mh_key_value_pair {
    // The key.
    mh_memory_t key;
    // The value.
    mh_memory_t value;
});


// Something to iterate over.
MH_API_TYPE(mh_iterator, struct mh_iterator {
    // For mh_iterator_start().
    bool (*start)(struct mh_iterator *iterator);

    // For mh_iterator_next().
    bool (*next)(struct mh_iterator *iterator);

    // For mh_iterator_current().
    mh_memory_t (*current)(struct mh_iterator *iterator);
});

// A collection of elements.
MH_API_TYPE(mh_collection, struct mh_collection {
    // An optional destructor.
    mh_destructor_t destructor;

    // For mh_collection_get_iterator().
    mh_iterator_t *(*get_iterator)(struct mh_collection *collection);
});

// Set the iterator to the beginning.
MH_API_FUNC(bool mh_iterator_start(mh_iterator_t *iterator));

// Move to the next element.
MH_API_FUNC(bool mh_iterator_next(mh_iterator_t *iterator));

// Get the current element.
MH_API_FUNC(mh_memory_t mh_iterator_current(mh_iterator_t *iterator));

// Create an iterator from a collection.
MH_API_FUNC(mh_iterator_t *mh_collection_get_iterator(mh_collection_t *collection));

// Iterate trough the array and do an action.
MH_API_FUNC(void mh_collection_foreach(mh_collection_t *collection, void (*action)(mh_memory_t memory)));

// Create an array collection.
MH_API_FUNC(mh_collection_t *mh_array_new(mh_context_t* context, void* array, size_t size, size_t element_size));

// Create an iterator and iterate trough the memory.
#define MH_FOREACH_MEM(it, col) mh_iterator_t *it = mh_collection_get_iterator((mh_collection_t *)(col)); for(bool it##_stat = mh_iterator_start(it); it##_stat; it##_stat = mh_iterator_next(it))

// Create an iterator and iterate trough the memory casted as a type.
#define MH_FOREACH(type, el, col) type el; mh_iterator_t *el##_it = mh_collection_get_iterator((mh_collection_t *)(col)); for(bool el##_it_stat = mh_iterator_start(el##_it); el##_it_stat && ((el) = mh_iterator_current(el##_it).address) != (type)-1; el##_it_stat = mh_iterator_next(el##_it))

// Create a fixed-size array collection.
#define MH_ARRAY(ctx, arr) mh_array_new(ctx, arr, sizeof(arr), sizeof(&arr[0]))

#endif //MHSERV_MH_COLLECTION_H
