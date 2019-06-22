
cmake_minimum_required (VERSION 2.6)
project (sourcesdk)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-m32)

file(GLOB_RECURSE SOURCESDK_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
add_library(sourcesdk SHARED ${SOURCESDK_SOURCES})
target_include_directories(sourcesdk PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/sdk")
target_include_directories(sourcesdk PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")
target_link_options(sourcesdk PUBLIC -m32)
