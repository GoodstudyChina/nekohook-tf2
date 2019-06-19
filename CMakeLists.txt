
cmake_minimum_required (VERSION 2.6)
project (nekohook-tf2)

set(CMAKE_CXX_STANDARD 17)
add_definitions(-m32)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    if(CMAKE_BUILD_TYPE STREQUAL "RELEASE")
        add_definitions(-mtune=generic -Ofast -DNDEBUG -fno-rtti -fno-plt -fvisibility=hidden)
    elseif(CMAKE_BUILD_TYPE STREQUAL "DEBUG")
        add_definitions(-g3 -ggdb -Og)
    endif()
endif()

file(GLOB_RECURSE NEKOHOOKTF_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
add_library(nekohook-tf2 SHARED NEKOHOOKTF_SOURCES)
target_include_directories(nekohook-tf2 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_include_directories(nekohook-tf2 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/sdk/include")
