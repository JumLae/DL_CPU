cmake_minimum_required(VERSION 3.20)

project(dl_cpu VERSION 0.1.0 LANGUAGES C CXX)

if(NOT MSVC)
    message(FATAL_ERROR "DL_CPU currently supports only MSVC toolchains.")
endif()

include(CTest)
if(CMAKE_VS_PLATFORM_NAME)
    set(dlcpu_fetchcontent_suffix "${CMAKE_VS_PLATFORM_NAME}")
else()
    math(EXPR dlcpu_pointer_bits "${CMAKE_SIZEOF_VOID_P} * 8")
    set(dlcpu_fetchcontent_suffix "${CMAKE_SYSTEM_NAME}-${dlcpu_pointer_bits}")
endif()

set(
    FETCHCONTENT_BASE_DIR
    "${CMAKE_SOURCE_DIR}/.cache/fetchcontent/${dlcpu_fetchcontent_suffix}"
    CACHE PATH
    "FetchContent cache root"
    FORCE
)
include(cmake/MSVCRuntime.cmake)
include(cmake/OnnxRuntimeNuget.cmake)

dlcpu_set_msvc_static_runtime()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

find_package(OpenCV REQUIRED COMPONENTS core imgproc imgcodecs)

dlcpu_setup_onnxruntime(
    TARGET_NAME dlcpu_onnxruntime
    INCLUDE_DIR_VAR DLCPU_ONNXRUNTIME_INCLUDE_DIR
    RUNTIME_DLLS_VAR DLCPU_ONNXRUNTIME_RUNTIME_DLLS
)

add_library(dl_cpu_internal_objects OBJECT
    src/internal/error_state.cpp
    src/internal/onnx_bootstrap.cpp
    src/internal/opencv_bootstrap.cpp
)

target_compile_features(dl_cpu_internal_objects PRIVATE cxx_std_17)
target_include_directories(dl_cpu_internal_objects
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/src/internal
)
target_include_directories(dl_cpu_internal_objects PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(dl_cpu_internal_objects
    PUBLIC
        dlcpu_onnxruntime
        ${OpenCV_LIBS}
)
dlcpu_enable_strict_warnings(dl_cpu_internal_objects)

add_library(dl_cpu SHARED
    src/dl_cpu.cpp
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
target_link_libraries(dl_cpu PRIVATE dl_cpu_internal_objects)
dlcpu_enable_strict_warnings(dl_cpu)
dlcpu_set_default_output_directories(dl_cpu)
dlcpu_copy_runtime_files(dl_cpu ${DLCPU_ONNXRUNTIME_RUNTIME_DLLS})

if(CMAKE_SIZEOF_VOID_P EQUAL 4)
    target_sources(dl_cpu PRIVATE src/dl_cpu_x86.def)
endif()

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
