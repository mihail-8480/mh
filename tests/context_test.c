#include "lib/default_tests.h"
#include "../inc/mh_context.h"

MH_TEST_NEW(context_create_test) {
    mh_context_t *context = mh_start();
    MH_TEST_EXPECT(context != NULL);
    mh_end(context);
    MH_TEST_PASSED();
}

MH_TEST_NEW(context_error_test) {

    MH_TRY(error2, MH_GLOBAL) {
        MH_TRY(error3, MH_GLOBAL) {
            MH_THROW(MH_GLOBAL, "Test2");
        } MH_CATCH (error3, MH_GLOBAL, {
            MH_THROW(MH_GLOBAL, "Test3");
        });
    } MH_CATCH (error2, MH_GLOBAL, {
        MH_TEST_PASSED();
    });

    MH_TEST_FAIL();
}

MH_TEST_NEW(context_allocate_test) {
    bool failed = false;
    mh_const_string_t reason = "Success.";
    mh_context_t *context = mh_start();
    mh_context_allocation_reference_t allocation = mh_context_allocate(context, 10, true);
    if (allocation.ptr == NULL) {
        failed = true;
        reason = "Allocation failed.";
    } else {
        for (int i = 0; i < 10; i++) {
            if (((char *) allocation.ptr)[i] != 0) {
                failed = true;
                reason = "Allocation is not zeroed.";
                break;
            }
        }
    }
    mh_end(context);
    MH_TEST_RESULT(!failed, reason);
}

MH_TEST_NEW(context_reallocate_test) {
    mh_context_t *context = mh_start();
    mh_context_allocation_reference_t allocation = mh_context_allocate(context, 10, true);
    mh_ref_t new_address = mh_context_reallocate(context, allocation, 20);
    mh_end(context);
    MH_TEST_EXPECT(new_address != NULL);
    MH_TEST_PASSED();
}

typedef struct my_destructor {
    mh_destructor_t destructor;
    bool *changed;
} my_destructor_t;

void my_destructor_free(mh_ref_t args) {
    *(((my_destructor_t *) args)->changed) = true;
}

MH_TEST_NEW(context_destructor_test) {
    bool changed = false;
    my_destructor_t destructor = {
            .changed = &changed,
            .destructor.free = my_destructor_free
    };
    mh_context_t *context = mh_start();
    mh_context_add_destructor(context, &destructor.destructor);
    mh_end(context);
    MH_TEST_EXPECT(changed);
    MH_TEST_PASSED();
}

MH_TEST_NEW(context_thread_bind_test) {
    mh_context_t *context = mh_start();
    mh_context_bind_to_thread(context);
    mh_context_t *second = mh_context_get_from_thread();
    mh_context_bind_to_thread(NULL);
    MH_TEST_EXPECT(context == second);
    MH_TEST_PASSED();
}
