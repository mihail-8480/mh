cmake_minimum_required(VERSION 3.0)
project(mh
        LANGUAGES C
        VERSION 0.1.2
        DESCRIPTION "A C library that is supposed to make my life easier."
        HOMEPAGE_URL "https://github.com/mihail-8480/mh")

include("Mh.cmake")

set(BUILD_WITH_INSTALL_RPATH TRUE)

add_compile_definitions(MH_VERSION_MINOR=${CMAKE_PROJECT_VERSION_MINOR})
add_compile_definitions(MH_VERSION_MAJOR=${CMAKE_PROJECT_VERSION_MAJOR})
add_compile_definitions(MH_VERSION_PATCH=${CMAKE_PROJECT_VERSION_PATCH})

add_library(mh SHARED src/collections/mh_stack.c src/streams/mh_stream.c src/streams/mh_memory_stream.c src/mh_memory.c src/network/mh_tcp.c src/streams/mh_socket_stream.c src/network/mh_http.c src/mh_thread.c src/streams/mh_file_stream.c src/mh_context.c src/collections/mh_shitmap.c src/collections/mh_collection.c src/mh_handle.c src/mh.c src/test/mh_tests.c src/application/mh_args.c src/collections/mh_list.c src/collections/mh_array.c)

if (WIN32)
    target_link_libraries(mh PRIVATE wsock32 ws2_32 dl)
endif (WIN32)

if (HAIKU)
    target_link_libraries(mh PRIVATE bsd src/network)
endif (HAIKU)

if (UNIX_LIKE)
    target_link_libraries(mh PRIVATE pthread dl)
endif (UNIX_LIKE)


add_library(mh-default-tests SHARED tests/thread_test.c tests/memory_test.c tests/handle_test.c tests/default_test_provider.c tests/context_test.c tests/stream_test.c tests/collection_test.c)
target_link_libraries(mh-default-tests PRIVATE mh)

add_executable(mhtest src/test/mh_test_runner.c)
target_link_libraries(mhtest PRIVATE mh)

install(TARGETS mh mhtest
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

install(DIRECTORY inc/ DESTINATION include)