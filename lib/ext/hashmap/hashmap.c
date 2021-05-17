// Copyright 2020 Joshua J Baker. All rights reserved.
// Use of this source code is governed by an MIT-style
// license that can be found in the ./LICENSE file.

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "hashmap.h"
#include "../../../inc/mh_context.h"

#define panic(_msg_) MH_THROW(MH_GLOBAL, _msg_)

struct bucket *bucket_at(struct hashmap *map, size_t index) {
    return (struct bucket *) (((char *) map->buckets) + (map->bucketsz * index));
}

void *bucket_item(struct bucket *entry) {
    return ((char *) entry) + sizeof(struct bucket);
}

static uint64_t get_hash(struct hashmap *map, void *key) {
    return map->hash(key, map->seed0, map->seed1) << 16 >> 16;
}

struct hashmap *hashmap_new(
        size_t elsize, size_t cap,
        uint64_t seed0, uint64_t seed1,
        uint64_t (*hash)(const void *,
                         uint64_t, uint64_t),
        int (*compare)(const void *, const void *,
                       void *),
        void *udata) {
    int ncap = 16;
    if (cap < (size_t) ncap) {
        cap = ncap;
    } else {
        while ((size_t) ncap < cap) {
            ncap *= 2;
        }
        cap = ncap;
    }
    size_t bucketsz = sizeof(struct bucket) + elsize;
    while (bucketsz & (sizeof(uintptr_t) - 1)) {
        bucketsz++;
    }
    size_t size = sizeof(struct hashmap) + bucketsz * 2;
    struct hashmap *map = malloc(size);
    if (!map) {
        return NULL;
    }
    memset(map, 0, sizeof(struct hashmap));
    map->elsize = elsize;
    map->bucketsz = bucketsz;
    map->seed0 = seed0;
    map->seed1 = seed1;
    map->hash = hash;
    map->compare = compare;
    map->udata = udata;
    map->spare = ((char *) map) + sizeof(struct hashmap);
    map->edata = (char *) map->spare + bucketsz;
    map->cap = cap;
    map->nbuckets = cap;
    map->mask = map->nbuckets - 1;
    map->buckets = malloc(map->bucketsz * map->nbuckets);
    if (!map->buckets) {
        free(map);
        return NULL;
    }
    memset(map->buckets, 0, map->bucketsz * map->nbuckets);
    map->growat = map->nbuckets * 0.75;
    map->shrinkat = map->nbuckets * 0.10;
    map->malloc = malloc;
    map->realloc = realloc;
    map->free = free;
    return map;
}


void hashmap_clear(struct hashmap *map, bool update_cap) {
    map->count = 0;
    if (update_cap) {
        map->cap = map->nbuckets;
    } else if (map->nbuckets != map->cap) {
        void *new_buckets = map->malloc(map->bucketsz * map->cap);
        if (new_buckets) {
            map->free(map->buckets);
            map->buckets = new_buckets;
        }
        map->nbuckets = map->cap;
    }
    memset(map->buckets, 0, map->bucketsz * map->nbuckets);
    map->mask = map->nbuckets - 1;
    map->growat = map->nbuckets * 0.75;
    map->shrinkat = map->nbuckets * 0.10;
}


static bool resize(struct hashmap *map, size_t new_cap) {
    struct hashmap *map2 = hashmap_new(map->elsize, new_cap, map->seed1,
                                       map->seed1, map->hash, map->compare,
                                       map->udata);
    if (!map2) {
        return false;
    }
    for (size_t i = 0; i < map->nbuckets; i++) {
        struct bucket *entry = bucket_at(map, i);
        if (!entry->dib) {
            continue;
        }
        entry->dib = 1;
        size_t j = entry->hash & map2->mask;
        for (;;) {
            struct bucket *bucket = bucket_at(map2, j);
            if (bucket->dib == 0) {
                memcpy(bucket, entry, map->bucketsz);
                break;
            }
            if (bucket->dib < entry->dib) {
                memcpy(map2->spare, bucket, map->bucketsz);
                memcpy(bucket, entry, map->bucketsz);
                memcpy(entry, map2->spare, map->bucketsz);
            }
            j = (j + 1) & map2->mask;
            entry->dib += 1;
        }
    }
    map->free(map->buckets);
    map->buckets = map2->buckets;
    map->nbuckets = map2->nbuckets;
    map->mask = map2->mask;
    map->growat = map2->growat;
    map->shrinkat = map2->shrinkat;
    map->free(map2);
    return true;
}

void *hashmap_set(struct hashmap *map, void *item) {
    if (!item) {
        panic("Item is null.");
    }
    map->oom = false;
    if (map->count == map->growat) {
        if (!resize(map, map->nbuckets * 2)) {
            map->oom = true;
            return NULL;
        }
    }


    struct bucket *entry = map->edata;
    entry->hash = get_hash(map, item);
    entry->dib = 1;
    memcpy(bucket_item(entry), item, map->elsize);

    size_t i = entry->hash & map->mask;
    for (;;) {
        struct bucket *bucket = bucket_at(map, i);
        if (bucket->dib == 0) {
            memcpy(bucket, entry, map->bucketsz);
            map->count++;
            return NULL;
        }
        if (entry->hash == bucket->hash &&
            map->compare(bucket_item(entry), bucket_item(bucket),
                         map->udata) == 0) {
            memcpy(map->spare, bucket_item(bucket), map->elsize);
            memcpy(bucket_item(bucket), bucket_item(entry), map->elsize);
            return map->spare;
        }
        if (bucket->dib < entry->dib) {
            memcpy(map->spare, bucket, map->bucketsz);
            memcpy(bucket, entry, map->bucketsz);
            memcpy(entry, map->spare, map->bucketsz);
        }
        i = (i + 1) & map->mask;
        entry->dib += 1;
    }
}

void *hashmap_get(struct hashmap *map, void *key) {
    if (!key) {
        panic("Key is null.");
    }
    uint64_t hash = get_hash(map, key);
    size_t i = hash & map->mask;
    for (;;) {
        struct bucket *bucket = bucket_at(map, i);
        if (!bucket->dib) {
            return NULL;
        }
        if (bucket->hash == hash &&
            map->compare(key, bucket_item(bucket), map->udata) == 0) {
            return bucket_item(bucket);
        }
        i = (i + 1) & map->mask;
    }
}

void *hashmap_probe(struct hashmap *map, uint64_t position) {
    size_t i = position & map->mask;
    struct bucket *bucket = bucket_at(map, i);
    if (!bucket->dib) {
        return NULL;
    }
    return bucket_item(bucket);
}

void *hashmap_delete(struct hashmap *map, void *key) {
    if (!key) {
        panic("Key is null.");
    }
    map->oom = false;
    uint64_t hash = get_hash(map, key);
    size_t i = hash & map->mask;
    for (;;) {
        struct bucket *bucket = bucket_at(map, i);
        if (!bucket->dib) {
            return NULL;
        }
        if (bucket->hash == hash &&
            map->compare(key, bucket_item(bucket), map->udata) == 0) {
            memcpy(map->spare, bucket_item(bucket), map->elsize);
            bucket->dib = 0;
            for (;;) {
                struct bucket *prev = bucket;
                i = (i + 1) & map->mask;
                bucket = bucket_at(map, i);
                if (bucket->dib <= 1) {
                    prev->dib = 0;
                    break;
                }
                memcpy(prev, bucket, map->bucketsz);
                prev->dib--;
            }
            map->count--;
            if (map->nbuckets > map->cap && map->count <= map->shrinkat) {
                // Ignore the return value. It's ok for the resize operation to
                // fail to allocate enough memory because a shrink operation
                // does not change the integrity of the data.
                resize(map, map->nbuckets / 2);
            }
            return map->spare;
        }
        i = (i + 1) & map->mask;
    }
}

void hashmap_free(struct hashmap *map) {
    if (!map) return;
    map->free(map->buckets);
    map->free(map);
}

bool hashmap_scan(struct hashmap *map,
                  bool (*iter)(const void *, void *), void *udata) {
    for (size_t i = 0; i < map->nbuckets; i++) {
        struct bucket *bucket = bucket_at(map, i);
        if (bucket->dib) {
            if (!iter(bucket_item(bucket), udata)) {
                return false;
            }
        }
    }
    return true;
}

//-----------------------------------------------------------------------------
// SipHash reference C implementation
//
// Copyright (c) 2012-2016 Jean-Philippe Aumasson
// <jeanphilippe.aumasson@gmail.com>
// Copyright (c) 2012-2014 Daniel J. Bernstein <djb@cr.yp.to>
//
// To the extent possible under law, the author(s) have dedicated all copyright
// and related and neighboring rights to this software to the public domain
// worldwide. This software is distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication along
// with this software. If not, see
// <http://creativecommons.org/publicdomain/zero/1.0/>.
//
// default: SipHash-2-4
//-----------------------------------------------------------------------------


static uint64_t SIP64(const uint8_t *in, const size_t inlen,
                      uint64_t seed0, uint64_t seed1) {
#define U8TO64_LE(p) \
    {  (((uint64_t)((p)[0])) | ((uint64_t)((p)[1]) << 8) | \
        ((uint64_t)((p)[2]) << 16) | ((uint64_t)((p)[3]) << 24) | \
        ((uint64_t)((p)[4]) << 32) | ((uint64_t)((p)[5]) << 40) | \
        ((uint64_t)((p)[6]) << 48) | ((uint64_t)((p)[7]) << 56)) }
#define U64TO8_LE(p, v) \
    { U32TO8_LE((p), (uint32_t)((v))); \
      U32TO8_LE((p) + 4, (uint32_t)((v) >> 32)); }
#define U32TO8_LE(p, v) \
    { (p)[0] = (uint8_t)((v)); \
      (p)[1] = (uint8_t)((v) >> 8); \
      (p)[2] = (uint8_t)((v) >> 16); \
      (p)[3] = (uint8_t)((v) >> 24); }
#define ROTL(x, b) (uint64_t)(((x) << (b)) | ((x) >> (64 - (b))))
#define SIPROUND \
    { v0 += v1; v1 = ROTL(v1, 13); \
      v1 ^= v0; v0 = ROTL(v0, 32); \
      v2 += v3; v3 = ROTL(v3, 16); \
      v3 ^= v2; \
      v0 += v3; v3 = ROTL(v3, 21); \
      v3 ^= v0; \
      v2 += v1; v1 = ROTL(v1, 17); \
      v1 ^= v2; v2 = ROTL(v2, 32); }
    uint64_t k0 = U8TO64_LE((uint8_t *) &seed0);
    uint64_t k1 = U8TO64_LE((uint8_t *) &seed1);
    uint64_t v3 = UINT64_C(0x7465646279746573) ^k1;
    uint64_t v2 = UINT64_C(0x6c7967656e657261) ^k0;
    uint64_t v1 = UINT64_C(0x646f72616e646f6d) ^k1;
    uint64_t v0 = UINT64_C(0x736f6d6570736575) ^k0;
    const uint8_t *end = in + inlen - (inlen % sizeof(uint64_t));
    for (; in != end; in += 8) {
        uint64_t m = U8TO64_LE(in);
        v3 ^= m;
        SIPROUND
        SIPROUND
        v0 ^= m;
    }
    const int left = inlen & 7;
    uint64_t b = ((uint64_t) inlen) << 56;
    switch (left) {
        case 7:
            b |= ((uint64_t) in[6]) << 48;
            MH_FALLTHROUGH
        case 6:
            b |= ((uint64_t) in[5]) << 40;
            MH_FALLTHROUGH
        case 5:
            b |= ((uint64_t) in[4]) << 32;
            MH_FALLTHROUGH
        case 4:
            b |= ((uint64_t) in[3]) << 24;
            MH_FALLTHROUGH
        case 3:
            b |= ((uint64_t) in[2]) << 16;
            MH_FALLTHROUGH
        case 2:
            b |= ((uint64_t) in[1]) << 8;
            MH_FALLTHROUGH
        case 1:
            b |= ((uint64_t) in[0]);
            break;
            MH_FALLTHROUGH
        case 0:
            break;
    }
    v3 ^= b;
    SIPROUND
    SIPROUND
    v0 ^= b;
    v2 ^= 0xff;
    SIPROUND
    SIPROUND
    SIPROUND
    SIPROUND
    b = v0 ^ v1 ^ v2 ^ v3;
    uint64_t out = 0;
    U64TO8_LE((uint8_t *) &out, b)
    return out;
}

uint64_t hashmap_sip(const void *data, size_t len,
                     uint64_t seed0, uint64_t seed1) {
    return SIP64((uint8_t *) data, len, seed0, seed1);
}
