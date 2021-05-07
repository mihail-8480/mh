#include "../../inc/mh_collection.h"

mh_iterator_t *mh_collection_get_iterator(mh_collection_t *collection) {
    return collection->get_iterator(collection);
}

bool mh_iterator_start(mh_iterator_t *iterator) {
    return iterator->start(iterator);
}

bool mh_iterator_next(mh_iterator_t *iterator) {
    return iterator->next(iterator);
}

mh_memory_t mh_iterator_current(mh_iterator_t *iterator) {
    return iterator->current(iterator);
}

void mh_collection_foreach(mh_collection_t *collection, void (*action)(mh_memory_t)) {
    MH_FOREACH_MEM(iterator, collection) {
        mh_memory_t mem = mh_iterator_current(iterator);
        action(mem);
    }
}

