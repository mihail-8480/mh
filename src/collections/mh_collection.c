#include "../../inc/mh_collection.h"
#include "../../inc/mh_map.h"
#include <stdarg.h>

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

void mh_collection_find_values(mh_collection_t *collection, size_t key_count, const mh_memory_t keys[], ...) {
    va_list args;
    va_start(args, keys);

    mh_context_t *context = mh_start();
    mh_map_t *arg_map = mh_map_new(context);

    for(size_t i = 0; i < key_count; i++) {
        mh_memory_t *arg = va_arg(args,mh_memory_t*);
        mh_map_set(arg_map, keys[i], mh_memory_reference(arg, sizeof(mh_memory_t*)));
    }

    va_end(args);

    MH_FOREACH(mh_key_value_pair_t*, kv, collection) {
        mh_memory_t mem = mh_map_get(arg_map, kv->key);
        if (mem.address == NULL) {
            continue;
        }
        mh_memory_t* arg = mem.address;
        *arg = kv->value;
    }

    mh_end(context);
}
