#include "default_tests.h"
#include "../inc/mh_thread.h"
#include "../inc/mh_tcp.h"

static bool listener_result = false;

static void *listener_thread(void *listener) {
    MH_THIS(mh_tcp_listener_t*, listener);
    mh_tcp_start(this);
    return NULL;
}


static void on_connect(mh_tcp_listener_t *listener, mh_context_t *context, mh_socket_t socket,
                       MH_UNUSED mh_socket_address_t address) {
    mh_stream_t *stream = mh_socket_stream_new(context, socket);
    mh_memory_t ref = MH_REF_CONST("Hello!");
    char buf[7];
    mh_memory_t buf_ref = MH_REF_CONST(buf);
    mh_stream_write(stream, &ref, ref.size);
    mh_stream_read(stream, &buf_ref, buf_ref.size);
    listener_result = mh_memory_is_equal(ref, buf_ref);
    listener->running = false;
}

MH_TEST_NEW(tcp_test) {
    mh_socket_address_t address = mh_tcp_string_to_address("127.0.0.1", 21337);
    mh_tcp_listener_t listener = {
            .context = MH_GLOBAL,
            .address = address,
            .max_clients = 1,
            .running = false,
            .on_connect = on_connect
    };
    mh_thread_create(listener_thread, &listener);

    usleep(100);

    mh_tcp_client_t client = {
            .address = address,
            .context = MH_GLOBAL
    };

    mh_stream_t *stream = mh_tcp_connect(&client);

    mh_memory_t ref = MH_REF_CONST("Hello!");

    char buf[7];
    mh_memory_t buf_ref = MH_REF_CONST(buf);

    mh_stream_read(stream, &buf_ref, buf_ref.size);
    mh_stream_write(stream, &ref, ref.size);


    while (listener.running) {
        usleep(100);
    }

    MH_TEST_EXPECT(mh_memory_is_equal(buf_ref, ref));
    MH_TEST_EXPECT(listener_result);
    MH_TEST_PASSED();
}
