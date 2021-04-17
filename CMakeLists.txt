cmake_minimum_required(VERSION 3.0)
project(mh C)
include("Mh.cmake")

add_library(mh SHARED streams/mh_stream.c streams/mh_memory_stream.c mh_memory.c network/mh_tcp.c streams/mh_socket_stream.c network/mh_http.c mh_thread.c streams/mh_file_stream.c mh_context.c collections/mh_shitmap.c collections/mh_collection.c mh_handle.c)

if (WIN32)
    target_link_libraries(mh PRIVATE wsock32 ws2_32 dl)
endif (WIN32)

if(HAIKU)
    target_link_libraries(mh PRIVATE bsd network)
endif(HAIKU)

if(UNIX_LIKE)
    target_link_libraries(mh PRIVATE pthread dl)
endif(UNIX_LIKE)