#ifndef MHSERV_MH_HTTP_H
#define MHSERV_MH_HTTP_H

#include "mh_tcp.h"
#include "mh_memory.h"
#include "mh_stream.h"
#include "mh_list.h"

/*
 * <mh_http.h>
 * The libmh HTTP header.
 *
 * Contains a HTTP listener implementation.
 */

// A HTTP request.
MH_API_TYPE(mh_http_request, struct mh_http_request {
    // The HTTP method.
    mh_memory_t method;
    // The HTTP URL.
    mh_memory_t url;
    // The HTTP version.
    mh_memory_t version;
    // The HTTP headers.
    mh_list_t *headers;
    // The HTTP address.
    mh_socket_address_t address;
    // The HTTP POST content.
    mh_memory_t content;
    // The listener from where this was requested.
    mh_tcp_listener_t *listener;
    // The current context.
    mh_context_t *context;
    // The socket stream.
    mh_stream_t *stream;
});

// Create a HTTP listener.
MH_API_FUNC(mh_tcp_listener_t *mh_http_listener_new(mh_tcp_listener_t base));

// A HTTP request handler.
typedef void (*http_request_handler_t)(mh_http_request_t *request);

// Set a request handler.
MH_API_FUNC(void mh_http_set_request_handler(mh_tcp_listener_t *listener, http_request_handler_t request_handler));

// Set an error handler.
MH_API_FUNC(void mh_http_set_error_handler(mh_tcp_listener_t *listener, mh_error_handler_t handler));

// Finish reading the entire post content into memory.
MH_API_FUNC(void mh_http_request_read_content(mh_http_request_t *request));

#endif //MHSERV_MH_HTTP_H
