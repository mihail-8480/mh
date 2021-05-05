#ifndef MHSERV_MH_MAP_H
#define MHSERV_MH_MAP_H

#include "mh_collection.h"
#include "mh_memory.h"

// The map structure
MH_API_TYPE(mh_map, struct mh_map {
    mh_collection_t collection;
});

// The key/value pair structure (the iterator returns mh_memory_t that points to this)
MH_API_TYPE(mh_key_value_pair, struct mh_key_value_pair {
    mh_memory_t key;
    mh_memory_t value;
});

// Add an element to the map
MH_API_FUNC(void mh_map_add(mh_map_t *map, mh_memory_t key, mh_memory_t value));

// Remove an element from the map (WARNING: currently not implemented)
MH_API_FUNC(void mh_map_remove(mh_map_t *map, mh_memory_t key));

// Get an element from the map
MH_API_FUNC(mh_memory_t mh_map_get(mh_map_t *map, mh_memory_t key));

// Check if the map contains a key
MH_API_FUNC(bool mh_map_contains(mh_map_t *map, mh_memory_t key));

// Create a new map inside a context
MH_API_FUNC(mh_map_t *mh_map_new(mh_context_t *context));

#endif //MHSERV_MH_MAP_H
