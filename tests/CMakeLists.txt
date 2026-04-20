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
    test_version.cpp
)

target_link_libraries(dl_cpu_tests
    PRIVATE
        dl_cpu
        GTest::gtest_main
)
target_compile_features(dl_cpu_tests PRIVATE cxx_std_17)

dlcpu_enable_strict_warnings(dl_cpu_tests)

foreach(config IN ITEMS Debug Release RelWithDebInfo MinSizeRel)
    string(TOUPPER "${config}" config_upper)
    set_target_properties(dl_cpu_tests PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_${config_upper} "${CMAKE_BINARY_DIR}/${config}"
        LIBRARY_OUTPUT_DIRECTORY_${config_upper} "${CMAKE_BINARY_DIR}/${config}"
        ARCHIVE_OUTPUT_DIRECTORY_${config_upper} "${CMAKE_BINARY_DIR}/${config}"
    )
endforeach()

gtest_discover_tests(dl_cpu_tests WORKING_DIRECTORY $<TARGET_FILE_DIR:dl_cpu_tests>)

