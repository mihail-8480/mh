#include "default_tests.h"
#include "../inc/mh_map.h"
#include "../inc/mh_list.h"

static int c = 0;

static void count(MH_UNUSED mh_memory_t mem) {
    c++;
}

MH_TEST_NEW(map_test) {
    mh_map_t *map = mh_map_new(MH_GLOBAL);
    char key[] = "TestKey";
    char key2[] = "TestKey1";
    char invalid_key[] = "TestKeyInvalid";
    char value[] = "TestValue";
    char value2[] = "TestValue2";
    mh_map_set(map, MH_REF_CONST(key), MH_REF_CONST(value));
    mh_map_set(map, MH_REF_CONST(key2), MH_REF_CONST(value2));
    MH_TEST_EXPECT(!mh_map_contains(map, MH_REF_CONST(invalid_key)));
    MH_TEST_EXPECT(mh_map_contains(map, MH_REF_CONST(key)));
    MH_TEST_EXPECT(mh_memory_is_equal(mh_map_get(map, MH_REF_CONST(key)), MH_REF_CONST(value)));
    mh_collection_foreach(&map->collection, count);
    MH_TEST_EXPECT(c == 2);
    MH_TEST_PASSED();
}

MH_TEST_NEW(list_test) {
    mh_list_t *list = mh_list_new(MH_GLOBAL);
    mh_list_prepend(list, MH_REF_CONST("Node0"));
    mh_list_append(list, MH_REF_CONST("Node1"));
    mh_list_append(list, MH_REF_CONST("Node2"));
    mh_list_append(list, MH_REF_CONST("Node3"));
    mh_list_append(list, MH_REF_CONST("Node4"));
    mh_list_prepend(list, MH_REF_CONST("Node-1"));
    mh_list_prepend(list, MH_REF_CONST("Node-2"));
    MH_TEST_EXPECT(mh_list_count(list) == 7);

    mh_iterator_t *iterator = mh_collection_get_iterator(&list->collection);
    MH_TEST_EXPECT(mh_iterator_start(iterator));
    MH_TEST_EXPECT(strcmp(mh_iterator_current(iterator).address, "Node-2") == 0);
    MH_TEST_EXPECT(mh_iterator_next(iterator));
    MH_TEST_EXPECT(strcmp(mh_iterator_current(iterator).address, "Node-1") == 0);
    MH_TEST_EXPECT(mh_iterator_next(iterator));
    MH_TEST_EXPECT(strcmp(mh_iterator_current(iterator).address, "Node0") == 0);
    MH_TEST_EXPECT(mh_iterator_next(iterator));
    MH_TEST_EXPECT(strcmp(mh_iterator_current(iterator).address, "Node1") == 0);
    MH_TEST_EXPECT(mh_iterator_next(iterator));
    MH_TEST_EXPECT(strcmp(mh_iterator_current(iterator).address, "Node2") == 0);
    MH_TEST_EXPECT(mh_iterator_next(iterator));
    MH_TEST_EXPECT(strcmp(mh_iterator_current(iterator).address, "Node3") == 0);
    MH_TEST_EXPECT(mh_iterator_next(iterator));
    MH_TEST_EXPECT(strcmp(mh_iterator_current(iterator).address, "Node4") == 0);

    mh_memory_t *mem = mh_list_node_value(mh_list_node_next(mh_list_at_index(list, 4)));
    MH_TEST_EXPECT(strcmp(mem->address, "Node3") == 0);

    mh_list_remove(list, mh_list_at_index(list, 0));
    mh_list_remove(list, mh_list_at_index(list, 1));

    MH_TEST_EXPECT(mh_list_count(list) == 5);
    MH_TEST_EXPECT(strcmp(mh_list_node_value(mh_list_last(list))->address, "Node4") == 0);

    MH_TEST_PASSED();
}

MH_TEST_NEW(iterator_test) {
    char array[] = "TEST";
    int i = 0;
    MH_FOREACH(char*, chr, MH_ARRAY(MH_GLOBAL, array)) {
        MH_TEST_EXPECT(*chr == array[i++]);
    }
    MH_TEST_PASSED();
}

MH_TEST_NEW(stack_test) {
    typedef struct {
        mh_stack_node_t node;
        int a;
    } my_node_t;

    my_node_t n1, n2, n3, n4;
    n1.a = 1;
    n2.a = 2;
    n3.a = 3;
    n4.a = 4;

    mh_stack_t stack = {
            .depth = 0,
            .last = NULL
    };

    mh_stack_push(&stack, &n1.node);
    mh_stack_push(&stack, &n2.node);
    mh_stack_push(&stack, &n3.node);
    mh_stack_push(&stack, &n4.node);

    int i = 4;
    while (stack.depth) {
        my_node_t *node = (my_node_t *) mh_stack_pop(&stack);
        MH_TEST_EXPECT(node->a == i--);
    }

    MH_TEST_PASSED();
}
