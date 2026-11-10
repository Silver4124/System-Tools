# 3.16 es el minimo real de este fichero: las dos banderas -S/-B que recomienda el
# README llegaron en 3.13 y CONFIGURE_DEPENDS en 3.12.
cmake_minimum_required(VERSION 3.16)
# La version vive aqui tambien: 1.5, la misma que anuncian la cabecera y el README.
project(System_Tools VERSION 1.5 LANGUAGES CXX)

# System Tools is header/impl-based: there is nothing to compile for the
# library itself, only the user's translation units. Expose it as INTERFACE.
add_library(System_Tools INTERFACE)
target_include_directories(System_Tools INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(System_Tools INTERFACE cxx_std_11)

# The library resolves system symbols at runtime (LoadLibrary/dlopen), so nothing
# is linked for it. glibc 2.34 moved dlopen and pthread_create into libc.so.6, and
# that is exactly where get_system_function looks, so there the flags are not needed
# at all. With an older glibc the two live in libdl.so.2 and libpthread.so.0, and
# since the only loader inside libc is dlopen itself the library cannot map them by
# itself: those are the cases that still need the link.
if(UNIX AND NOT APPLE)
    include(CheckCXXSourceCompiles)
    set(CMAKE_REQUIRED_QUIET ON)
    check_cxx_source_compiles("
        extern \"C\" void* dlopen(const char*, int);
        extern \"C\" int pthread_create(void*, const void*, void*(*)(void*), void*);
        void* (*st_taken_dlopen)(const char*, int) = &dlopen;
        int (*st_taken_pthread_create)(void*, const void*, void*(*)(void*), void*) = &pthread_create;
        int main(){ return 0; }
    " ST_LIBC_CARRIES_DL_AND_PTHREAD)
    if(NOT ST_LIBC_CARRIES_DL_AND_PTHREAD)
        target_link_libraries(System_Tools INTERFACE dl pthread)
    endif()
endif()

option(ST_BUILD_EXAMPLES "Build the examples under examples/" ON)
if(ST_BUILD_EXAMPLES)
    file(GLOB ST_EXAMPLE_SRCS CONFIGURE_DEPENDS examples/*/*.cpp)
    # examples/counterexamples is written on purpose to fail: kept out of the sweep.
    list(FILTER ST_EXAMPLE_SRCS EXCLUDE REGEX "examples/counterexamples/")
    foreach(src ${ST_EXAMPLE_SRCS})
        get_filename_component(st_name ${src} NAME_WE)
        add_executable(${st_name} ${src})
        target_link_libraries(${st_name} PRIVATE System_Tools)
        set_target_properties(${st_name} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    endforeach()
endif()
