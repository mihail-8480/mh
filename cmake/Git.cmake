cmake_minimum_required(VERSION 3.0)
execute_process(COMMAND
        "${GIT_EXECUTABLE}" rev-parse HEAD
        RESULT_VARIABLE
        GIT_HASH_RESULT
        OUTPUT_VARIABLE
        GIT_HASH)
string(REGEX REPLACE "\n$" "" GIT_HASH "${GIT_HASH}")
add_compile_definitions(MH_GIT_HASH="${GIT_HASH}")