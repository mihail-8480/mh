// Copyright 2020 Joshua J Baker. All rights reserved.
// Use of this source code is governed by an MIT-style
// license that can be found in ./LICENSE file.

#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct hashmap;

struct bucket {
    uint64_t hash: 48;
    uint64_t dib: 16;
};

struct hashmap {
    void *(*malloc)(size_t);

    void *(*realloc)(void *, size_t);

    void (*free)(void *);

    bool oom;
    size_t elsize;
    size_t cap;
    uint64_t seed0;
    uint64_t seed1;

    uint64_t (*hash)(const void *item, uint64_t seed0, uint64_t seed1);

    int (*compare)(const void *a, const void *b, void *udata);

    void *udata;
    size_t bucketsz;
    size_t nbuckets;
    size_t count;
    size_t mask;
    size_t growat;
    size_t shrinkat;
    void *buckets;
    void *spare;
    void *edata;
};


struct hashmap *hashmap_new(size_t elsize, size_t cap,
                            uint64_t seed0, uint64_t seed1,
                            uint64_t (*hash)(const void *item,
                                             uint64_t seed0, uint64_t seed1),
                            int (*compare)(const void *a, const void *b,
                                           void *udata),
                            void *udata);

void hashmap_free(struct hashmap *map);

void hashmap_clear(struct hashmap *map, bool update_cap);

void *hashmap_get(struct hashmap *map, void *item);

void *hashmap_set(struct hashmap *map, void *item);

void *hashmap_delete(struct hashmap *map, void *item);

void *hashmap_probe(struct hashmap *map, uint64_t position);

uint64_t hashmap_sip(const void *data, size_t len,
                     uint64_t seed0, uint64_t seed1);

struct bucket *bucket_at(struct hashmap *map, size_t index);

void *bucket_item(struct bucket *entry);

#endif
