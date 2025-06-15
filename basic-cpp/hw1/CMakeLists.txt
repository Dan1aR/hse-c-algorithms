cmake_minimum_required(VERSION 3.16)
project(KeyValueStorage)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")
add_executable(kv_storage ${SOURCES})

file(GLOB TEST_FILES "tests/*.cpp")
if(TEST_FILES)
    add_executable(run_tests 
        ${TEST_FILES}
        src/string_pool.cpp
        src/storage.cpp
    )
endif()
