#include "../../inc/mh_map.h"
#include "hashmap.h"
#include <string.h>

typedef struct mh_hashmap {
    mh_map_t base;
    struct hashmap *map;
    mh_context_t *context;
} mh_hashmap_t;

typedef struct mh_map_iterator {
    mh_iterator_t base;
    mh_hashmap_t *map;
    size_t i;
} mh_map_iterator_t;

static int mh_kv_compare(const void *a, const void *b, MH_UNUSED void *udata) {
    const mh_key_value_pair_t *ua = a;
    const mh_key_value_pair_t *ub = b;
    if (ua->key.size != ub->key.size) {
        return -1;
    }
    return memcmp(ua->key.address, ub->key.address, ua->key.size);
}

static uint64_t mh_kv_hash(const void *item, uint64_t seed0, uint64_t seed1) {
    const mh_key_value_pair_t *kv = item;
    uint64_t hash = hashmap_sip(kv->key.address, kv->key.size, seed0, seed1);
    return hash;
}

void mh_map_set(mh_map_t *map, mh_memory_t key, mh_memory_t value) {
    MH_THIS(mh_hashmap_t*, map);
    mh_key_value_pair_t *kv = mh_context_allocate(this->context, sizeof(mh_key_value_pair_t), false).ptr;
    *kv = (mh_key_value_pair_t) {
            .key = key,
            .value = value
    };
    void *result = hashmap_set(this->map, kv);
    if (result == NULL && this->map->oom) {
        MH_THROW(this->context, "Out of memory.");
    }
}

void mh_map_remove(mh_map_t *map, MH_UNUSED mh_memory_t key) {
    MH_THIS(mh_hashmap_t*, map);
    if (hashmap_delete(this->map, &(mh_key_value_pair_t) {.key = key}) == NULL) {
        MH_THROW(this->context, "The item was not found.");
    }
}


mh_memory_t mh_map_get(mh_map_t *map, mh_memory_t key) {
    MH_THIS(mh_hashmap_t*, map);
    void *result = hashmap_get(this->map, &(mh_key_value_pair_t) {.key = key});
    if (result == NULL) {
        return MH_MEM_NULL;
    }
    return ((mh_key_value_pair_t *) result)->value;
}

bool mh_map_contains(mh_map_t *map, mh_memory_t key) {
    return mh_map_get(map, key).address != NULL;
}

static bool mh_map_iterator_start(mh_iterator_t *iterator) {
    MH_THIS(mh_map_iterator_t*, iterator);
    this->i = 0;
    while (this->i < this->map->map->nbuckets) {
        struct bucket *bucket = bucket_at(this->map->map, this->i);
        if (bucket->dib) {
            return true;
        }
        this->i++;
    }
    return false;
}

static bool mh_map_iterator_next(mh_iterator_t *iterator) {
    MH_THIS(mh_map_iterator_t*, iterator);
    while (++this->i < this->map->map->nbuckets) {
        struct bucket *bucket = bucket_at(this->map->map, this->i);
        if (bucket->dib) {
            return true;
        }
    }
    return false;
}

static mh_memory_t mh_map_iterator_current(mh_iterator_t *iterator) {
    MH_THIS(mh_map_iterator_t*, iterator);
    struct bucket *bucket = bucket_at(this->map->map, this->i);
    return (mh_memory_t) {
            .address = bucket_item(bucket),
            .size = sizeof(mh_key_value_pair_t),
            .offset = 0
    };
}

static mh_iterator_t *mh_map_get_iterator(mh_collection_t *collection) {
    MH_THIS(mh_hashmap_t*, collection);
    mh_map_iterator_t *it = mh_context_allocate(this->context, sizeof(mh_map_iterator_t), false).ptr;
    *it = (mh_map_iterator_t) {
            .map = this,
            .i = 0,
            .base = {
                    .next = mh_map_iterator_next,
                    .current = mh_map_iterator_current,
                    .start = mh_map_iterator_start
            }
    };
    return &it->base;
}

static void mh_map_destructor(void *map) {
    MH_THIS(mh_hashmap_t*, map);
    hashmap_free(this->map);
}

mh_map_t *mh_map_new(mh_context_t *context) {
    MH_THIS(mh_hashmap_t*, mh_context_allocate(context, sizeof(mh_hashmap_t), false).ptr);
    *this = (mh_hashmap_t) {
            .context = context,
            .base.collection = {
                    .destructor = {mh_map_destructor},
                    .get_iterator = mh_map_get_iterator
            },
            .map = hashmap_new(sizeof(mh_key_value_pair_t), 0, 0, 0,
                               mh_kv_hash, mh_kv_compare, NULL)
    };
    mh_context_add_destructor(context, &this->base.collection.destructor);
    return &this->base;
}
