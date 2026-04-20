function(dlcpu_set_msvc_static_runtime)
    if(MSVC)
        set(CMAKE_MSVC_RUNTIME_LIBRARY
            "MultiThreaded$<$<CONFIG:Debug>:Debug>"
            CACHE STRING
            "MSVC runtime library"
            FORCE
        )
    endif()
endfunction()

function(dlcpu_enable_strict_warnings target_name)
    if(NOT TARGET ${target_name})
        message(FATAL_ERROR "Target '${target_name}' does not exist.")
    endif()

    if(MSVC)
        set_property(
            TARGET ${target_name}
            PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
        )
        target_compile_options(${target_name} PRIVATE /W4 /WX)
    endif()
endfunction()

