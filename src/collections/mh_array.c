#include "../../inc/mh_collection.h"

typedef struct mh_array {
    mh_collection_t collection;
    void* ptr;
    size_t size;
    size_t element_size;
    mh_context_t *context;
} mh_array_t;


typedef struct mh_array_iterator {
    mh_iterator_t iterator;
    size_t position;
    mh_array_t *array;
} mh_array_iterator_t;


static mh_memory_t mh_array_iterator_current(mh_iterator_t *iterator) {
    MH_THIS(mh_array_iterator_t*, iterator);
    if (this->array->size <= this->array->element_size * this->position) {
        return MH_MEM_NULL;
    }
    return mh_memory_reference((char *) this->array->ptr + (this->array->element_size * this->position), this->array->element_size);
}

static bool mh_array_iterator_start(mh_iterator_t *iterator) {
    MH_THIS(mh_array_iterator_t*, iterator);
    if (this->array->size < this->array->element_size) {
        return false;
    }
    this->position = 0;
    return true;
}

static bool mh_array_iterator_next(mh_iterator_t *iterator) {
    MH_THIS(mh_array_iterator_t*, iterator);
    if (this->array->size <= this->array->element_size * (this->position + 1)) {
        return false;
    }
    this->position++;
    return true;
}

static mh_iterator_t *mh_array_get_iterator(mh_collection_t *collection) {
    MH_THIS(mh_array_t*, collection);
    mh_array_iterator_t* it = mh_context_allocate(this->context, sizeof(mh_array_iterator_t), false).ptr;
    *it = (mh_array_iterator_t) {
            .array = this,
            .position = 0,
            .iterator = {
                    .current = mh_array_iterator_current,
                    .next = mh_array_iterator_next,
                    .start = mh_array_iterator_start
            }
    };
    return &it->iterator;
}


mh_collection_t *mh_array_new(mh_context_t* context, void* array, size_t size, size_t element_size) {
    MH_THIS(mh_array_t*, mh_context_allocate(context, sizeof(mh_array_t), false).ptr);
    *this = (mh_array_t) {
            .element_size = element_size,
            .size = size,
            .ptr = array,
            .context = context,
            .collection = {
                    .get_iterator = mh_array_get_iterator,
                    .destructor = { NULL }
            }};
    return &this->collection;
}
