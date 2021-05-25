cmake_minimum_required(VERSION 3.0)
project(mh
        LANGUAGES C
        VERSION 0.3.0
        DESCRIPTION "A C library that is supposed to make my life easier."
        HOMEPAGE_URL "https://github.com/mihail-8480/mh")

include("cmake/Mh.cmake")

# If git is found, add the commit hash as a macro definition.
find_package(Git)
if (Git_FOUND)
    include("cmake/Git.cmake")
endif ()

# Make the version number be visible to the C code using definitions.
add_compile_definitions(MH_VERSION_MINOR=${CMAKE_PROJECT_VERSION_MINOR})
add_compile_definitions(MH_VERSION_MAJOR=${CMAKE_PROJECT_VERSION_MAJOR})
add_compile_definitions(MH_VERSION_PATCH=${CMAKE_PROJECT_VERSION_PATCH})

# Fix some linker problems.
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Create a library called libmhext.a that contains code that wasn't written by me and has a different copyright.
add_library(mhext STATIC lib/ext/hashmap/hashmap.c lib/ext/hashmap/mh_map.c)

# libmh.so
add_library(mh SHARED lib/src/collections/mh_stack.c lib/src/io/mh_stream.c lib/src/io/mh_memory_stream.c lib/src/mh_memory.c lib/src/network/mh_tcp.c lib/src/io/mh_socket_stream.c lib/src/network/mh_http.c lib/src/mh_thread.c lib/src/io/mh_file_stream.c lib/src/mh_context.c lib/src/collections/mh_collection.c lib/src/mh_handle.c lib/src/mh.c lib/src/test/mh_tests.c lib/src/application/mh_console.c lib/src/collections/mh_list.c lib/src/collections/mh_array.c lib/src/mh_convert.c lib/src/io/mh_writer.c lib/src/io/mh_reader.c lib/src/time/mh_stopwatch.c lib/src/time/mh_time.c)

# Link platform dependent libraries.
if (WIN32)
    target_link_libraries(mh PRIVATE wsock32 ws2_32 dl m)
endif (WIN32)

if (HAIKU)
    target_link_libraries(mh PRIVATE bsd network m)
endif (HAIKU)

if (UNIX_LIKE)
    target_link_libraries(mh PRIVATE pthread dl m)
endif (UNIX_LIKE)

# Link libmhext.a to libmh.so.
target_link_libraries(mh PRIVATE mhext)

# libmh-default-tests.so
add_library(mh-default-tests MODULE tests/thread_test.c tests/memory_test.c tests/handle_test.c tests/lib/default_test_provider.c tests/context_test.c tests/stream_test.c tests/collection_test.c tests/conversion_test.c)
target_link_libraries(mh-default-tests PRIVATE mh)

# mhtest
add_executable(mhtest lib/src/test/mh_test_runner.c)
target_link_libraries(mhtest PRIVATE mh)

# example
add_executable(example examples/lib/main.c examples/version.c lib/src/time/mh_stopwatch.c)
target_link_libraries(example PRIVATE mh)

install(TARGETS mh mhtest
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

install(DIRECTORY inc/ DESTINATION include)