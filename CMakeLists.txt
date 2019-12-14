
cmake_minimum_required (VERSION 2.6)
project (nekohook-tf2)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-m32)
add_link_options(-m32)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    if(CMAKE_BUILD_TYPE STREQUAL "RELEASE")
        add_compile_options(-mtune=generic -Ofast -DNDEBUG -fno-rtti -fno-plt -fvisibility=hidden)
    elseif(CMAKE_BUILD_TYPE STREQUAL "DEBUG")
        add_compile_options(-g3 -ggdb -Og)
    endif()
endif()


add_subdirectory(external/nekohook)
add_subdirectory(external/nekohack)
file(GLOB_RECURSE NEKOHOOKTF_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
add_library(nekohook-tf2 SHARED ${NEKOHOOKTF_SOURCES})
target_include_directories(nekohook-tf2 PRIVATE nekohook nekohack)
target_link_libraries(nekohook-tf2 PRIVATE nekohook nekohack)

# Class and netvar dumper
file(GLOB_RECURSE DUMPERTF_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/dumper/*.cpp")
add_library(nekohook-dumper SHARED ${DUMPERTF_SOURCES})
target_include_directories(nekohook-dumper PRIVATE nekohack "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_link_libraries(nekohook-dumper PRIVATE nekohack)
