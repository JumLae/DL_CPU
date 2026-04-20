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
        target_compile_options(${target_name} PRIVATE /utf-8 /W4 /WX)
    endif()
endfunction()

function(dlcpu_set_default_output_directories target_name)
    foreach(config IN ITEMS Debug Release RelWithDebInfo MinSizeRel)
        string(TOUPPER "${config}" config_upper)
        set_target_properties(${target_name} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY_${config_upper} "${CMAKE_BINARY_DIR}/${config}"
            LIBRARY_OUTPUT_DIRECTORY_${config_upper} "${CMAKE_BINARY_DIR}/${config}"
            ARCHIVE_OUTPUT_DIRECTORY_${config_upper} "${CMAKE_BINARY_DIR}/${config}"
        )
    endforeach()
endfunction()

function(dlcpu_copy_runtime_files target_name)
    foreach(runtime_file IN LISTS ARGN)
        if(runtime_file)
            add_custom_command(
                TARGET ${target_name}
                POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${runtime_file}"
                    "$<TARGET_FILE_DIR:${target_name}>"
                VERBATIM
            )
        endif()
    endforeach()
endfunction()
