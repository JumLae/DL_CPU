cmake_minimum_required(VERSION 3.20)

project(dl_cpu VERSION 0.1.0 LANGUAGES C CXX)

if(NOT MSVC)
    message(FATAL_ERROR "DL_CPU currently supports only MSVC toolchains.")
endif()

include(CTest)
include(cmake/MSVCRuntime.cmake)

dlcpu_set_msvc_static_runtime()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_library(dl_cpu SHARED
    src/dl_cpu.cpp
    src/internal/error_state.cpp
)

target_compile_definitions(dl_cpu PRIVATE DLCPU_BUILD)
target_compile_features(dl_cpu PRIVATE cxx_std_17)
target_include_directories(dl_cpu
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/src/internal
)

dlcpu_enable_strict_warnings(dl_cpu)

if(CMAKE_SIZEOF_VOID_P EQUAL 4)
    target_sources(dl_cpu PRIVATE src/dl_cpu_x86.def)
endif()

foreach(config IN ITEMS Debug Release RelWithDebInfo MinSizeRel)
    string(TOUPPER "${config}" config_upper)
    set_target_properties(dl_cpu PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_${config_upper} "${CMAKE_BINARY_DIR}/${config}"
        LIBRARY_OUTPUT_DIRECTORY_${config_upper} "${CMAKE_BINARY_DIR}/${config}"
        ARCHIVE_OUTPUT_DIRECTORY_${config_upper} "${CMAKE_BINARY_DIR}/${config}"
    )
endforeach()

add_library(dl_cpu_public_header_c OBJECT cmake/public_header_check.c)
target_include_directories(dl_cpu_public_header_c PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
set_property(TARGET dl_cpu_public_header_c PROPERTY C_STANDARD 99)
set_property(TARGET dl_cpu_public_header_c PROPERTY C_STANDARD_REQUIRED YES)
dlcpu_enable_strict_warnings(dl_cpu_public_header_c)

add_library(dl_cpu_public_header_cpp11 OBJECT cmake/public_header_check.cpp)
target_include_directories(dl_cpu_public_header_cpp11 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
set_property(TARGET dl_cpu_public_header_cpp11 PROPERTY CXX_STANDARD 11)
set_property(TARGET dl_cpu_public_header_cpp11 PROPERTY CXX_STANDARD_REQUIRED YES)
set_property(TARGET dl_cpu_public_header_cpp11 PROPERTY CXX_EXTENSIONS NO)
dlcpu_enable_strict_warnings(dl_cpu_public_header_cpp11)

if(BUILD_TESTING AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/CMakeLists.txt")
    add_subdirectory(tests)
endif()

