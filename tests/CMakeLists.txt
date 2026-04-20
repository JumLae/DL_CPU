include(FetchContent)
include(GoogleTest)

set(gtest_force_shared_crt OFF CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)

FetchContent_MakeAvailable(googletest)

add_executable(dl_cpu_tests
    ../src/dl_cpu.cpp
    test_version.cpp
)
target_compile_definitions(dl_cpu_tests PRIVATE DLCPU_BUILD)
target_compile_features(dl_cpu_tests PRIVATE cxx_std_17)
target_link_libraries(dl_cpu_tests PRIVATE dl_cpu_internal_objects GTest::gtest_main)
dlcpu_enable_strict_warnings(dl_cpu_tests)
dlcpu_set_default_output_directories(dl_cpu_tests)
dlcpu_copy_runtime_files(dl_cpu_tests ${DLCPU_ONNXRUNTIME_RUNTIME_DLLS})
gtest_discover_tests(dl_cpu_tests WORKING_DIRECTORY $<TARGET_FILE_DIR:dl_cpu_tests>)

add_executable(dl_cpu_dependency_tests
    test_onnx_link.cpp
    test_opencv_link.cpp
)
target_compile_features(dl_cpu_dependency_tests PRIVATE cxx_std_17)
target_link_libraries(dl_cpu_dependency_tests PRIVATE dl_cpu_internal_objects GTest::gtest_main)
dlcpu_enable_strict_warnings(dl_cpu_dependency_tests)
dlcpu_set_default_output_directories(dl_cpu_dependency_tests)
dlcpu_copy_runtime_files(dl_cpu_dependency_tests ${DLCPU_ONNXRUNTIME_RUNTIME_DLLS})
gtest_discover_tests(dl_cpu_dependency_tests WORKING_DIRECTORY $<TARGET_FILE_DIR:dl_cpu_dependency_tests>)

