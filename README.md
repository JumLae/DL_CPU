# DL_CPU

DL_CPU is a CPU-only Windows DLL scaffold for legacy industrial inspection
applications. The repository now wires in ONNX Runtime 1.22.0 and OpenCV as
internal dependencies while keeping the public ABI limited to a small C
surface with static CRT linkage in `dl_cpu.dll`.

## Repository layout

- `include/dl_cpu/` public C headers
- `src/` internal C++ implementation
- `src/internal/` internal-only headers and utilities
- `tests/` GoogleTest-based unit tests
- `third_party/` placeholder for future vendored dependencies
- `cmake/` CMake helper modules and header compatibility checks
- `scripts/` helper scripts placeholder
- `docs/` dependency and architecture notes
- `.github/workflows/` GitHub Actions CI

## Requirements

- Windows
- Visual Studio 2019 with the v142 toolset
- CMake 3.20 or newer
- A local vcpkg checkout pinned to the repository baseline commit

## Local build

Set `VCPKG_ROOT` to your local vcpkg checkout before configuring:

```powershell
$env:VCPKG_ROOT = 'D:\path\to\vcpkg'
```

### x64

```powershell
cmake -S . -B build/x64 -G "Visual Studio 16 2019" -A x64 `
  -DCMAKE_TOOLCHAIN_FILE="$env:VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake" `
  -DVCPKG_TARGET_TRIPLET=x64-windows
cmake --build build/x64 --config Debug
cmake --build build/x64 --config Release
ctest --test-dir build/x64 -C Debug --output-on-failure
ctest --test-dir build/x64 -C Release --output-on-failure
```

### x86

```powershell
cmake -S . -B build/x86 -G "Visual Studio 16 2019" -A Win32 `
  -DCMAKE_TOOLCHAIN_FILE="$env:VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake" `
  -DVCPKG_TARGET_TRIPLET=x86-windows
cmake --build build/x86 --config Debug
cmake --build build/x86 --config Release
ctest --test-dir build/x86 -C Debug --output-on-failure
ctest --test-dir build/x86 -C Release --output-on-failure
```

If the Visual Studio 2019 generator is unavailable, use the Visual Studio 2022
generator with `-T v142`.

## Runtime deployment bundle

The runtime directory must contain the following bundle next to the client
application executable:

- `dl_cpu.dll`
- `onnxruntime.dll`
- `onnxruntime_providers_shared.dll`
- `opencv_world4.dll`
- `zlib1.dll`

The current OpenCV world build also imports `MSVCP140.dll`, `CONCRT140.dll`,
`VCRUNTIME140.dll`, and `VCRUNTIME140_1.dll` from the Microsoft runtime.
Those CRT DLLs are not copied into the local build tree automatically, so
deployment on machines without the corresponding Microsoft runtime still
needs either the Redistributable installed or an approved app-local CRT
packaging step.

## Exported functions

The current DLL exports exactly two C symbols:

- `DLCPU_GetVersion`
- `DLCPU_GetLastErrorMessage`

## VS2012 MFC client integration

The client output directory must contain the runtime deployment bundle above
before the application starts. That requirement applies both to static import
library usage and to explicit `LoadLibraryW` loading.

### Option 1: link against the import library

```cpp
extern "C" {
#include "dl_cpu/dl_cpu.h"
}

void QueryVersion()
{
    int32_t major = 0;
    int32_t minor = 0;
    int32_t patch = 0;
    const int32_t status = DLCPU_GetVersion(&major, &minor, &patch);
    (void)status;
}
```

### Option 2: load the DLL dynamically

```cpp
typedef int32_t (__stdcall* DLCPU_GetVersionFn)(
    int32_t* major,
    int32_t* minor,
    int32_t* patch);

typedef int32_t (__stdcall* DLCPU_GetLastErrorMessageFn)(
    char* buffer,
    int32_t buffer_size);

void QueryVersionDynamically()
{
    HMODULE module = ::LoadLibraryW(L"dl_cpu.dll");
    if (module == NULL) {
        return;
    }

    DLCPU_GetVersionFn get_version =
        reinterpret_cast<DLCPU_GetVersionFn>(::GetProcAddress(module, "DLCPU_GetVersion"));
    DLCPU_GetLastErrorMessageFn get_last_error =
        reinterpret_cast<DLCPU_GetLastErrorMessageFn>(
            ::GetProcAddress(module, "DLCPU_GetLastErrorMessage"));

    if (get_version != NULL && get_last_error != NULL) {
        int32_t major = 0;
        int32_t minor = 0;
        int32_t patch = 0;
        char message[256] = {0};

        const int32_t status = get_version(&major, &minor, &patch);
        if (status != 0) {
            get_last_error(message, static_cast<int32_t>(sizeof(message)));
        }
    }

    ::FreeLibrary(module);
}
```

## Current scope

This repository now verifies that ONNX Runtime and OpenCV can be fetched,
linked, copied into the runtime bundle, and exercised through internal smoke
tests. It still does not implement model loading, preprocessing,
classification, segmentation, or any public inference API.
