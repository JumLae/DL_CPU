include(FetchContent)

function(dlcpu_setup_onnxruntime)
    set(options)
    set(oneValueArgs TARGET_NAME INCLUDE_DIR_VAR RUNTIME_DLLS_VAR)
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "" ${ARGN})

    if(NOT ARG_TARGET_NAME OR NOT ARG_INCLUDE_DIR_VAR OR NOT ARG_RUNTIME_DLLS_VAR)
        message(FATAL_ERROR "dlcpu_setup_onnxruntime requires TARGET_NAME, INCLUDE_DIR_VAR, and RUNTIME_DLLS_VAR.")
    endif()

    FetchContent_Declare(
        dlcpu_onnxruntime_nuget
        URL https://www.nuget.org/api/v2/package/Microsoft.ML.OnnxRuntime/1.22.0
        URL_HASH SHA256=D571E63A2329BAACB713F441E65AD75284DE354DB6E1AC435FE4BEBBB417986A
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )

    FetchContent_MakeAvailable(dlcpu_onnxruntime_nuget)

    set(include_dir "${dlcpu_onnxruntime_nuget_SOURCE_DIR}/build/native/include")
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(runtime_subdir "win-x64")
    else()
        set(runtime_subdir "win-x86")
    endif()

    set(native_dir "${dlcpu_onnxruntime_nuget_SOURCE_DIR}/runtimes/${runtime_subdir}/native")
    set(import_lib "${native_dir}/onnxruntime.lib")
    set(runtime_dll "${native_dir}/onnxruntime.dll")
    set(provider_dll "${native_dir}/onnxruntime_providers_shared.dll")

    foreach(required_path IN ITEMS "${include_dir}" "${import_lib}" "${runtime_dll}" "${provider_dll}")
        if(NOT EXISTS "${required_path}")
            message(FATAL_ERROR "ONNX Runtime 1.22.0 package is missing expected path: ${required_path}")
        endif()
    endforeach()

    add_library(${ARG_TARGET_NAME} SHARED IMPORTED GLOBAL)
    set_target_properties(${ARG_TARGET_NAME} PROPERTIES
        IMPORTED_IMPLIB "${import_lib}"
        IMPORTED_LOCATION "${runtime_dll}"
        INTERFACE_INCLUDE_DIRECTORIES "${include_dir}"
    )

    set(${ARG_INCLUDE_DIR_VAR} "${include_dir}" PARENT_SCOPE)
    set(${ARG_RUNTIME_DLLS_VAR} "${runtime_dll};${provider_dll}" PARENT_SCOPE)
endfunction()
