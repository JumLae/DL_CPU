# DL_CPU

DL_CPU is a CPU-only Windows DLL scaffold for legacy industrial inspection
applications. This repository currently provides a minimal C ABI shell with
static CRT linkage, x86/x64 Visual Studio builds, GoogleTest coverage, and
GitHub Actions validation. ONNX Runtime and OpenCV integration are intentionally
deferred to later tasks.

## Repository layout

- `include/dl_cpu/` public C headers
- `src/` internal C++ implementation
- `src/internal/` internal-only headers and utilities
- `tests/` GoogleTest-based unit tests
- `third_party/` placeholder for future vendored dependencies
- `cmake/` CMake helper modules and header compatibility checks
- `scripts/` helper scripts placeholder
- `docs/` architecture notes placeholder
- `.github/workflows/` GitHub Actions CI

## Requirements

- Windows
- Visual Studio 2019 with the v142 toolset
- CMake 3.20 or newer

## Local build

### x64

```powershell
cmake -S . -B build/x64 -G "Visual Studio 16 2019" -A x64
cmake --build build/x64 --config Debug
cmake --build build/x64 --config Release
ctest --test-dir build/x64 -C Debug --output-on-failure
ctest --test-dir build/x64 -C Release --output-on-failure
```

### x86

```powershell
cmake -S . -B build/x86 -G "Visual Studio 16 2019" -A Win32
cmake --build build/x86 --config Debug
cmake --build build/x86 --config Release
ctest --test-dir build/x86 -C Debug --output-on-failure
ctest --test-dir build/x86 -C Release --output-on-failure
```

If the Visual Studio 2019 generator is unavailable, use the Visual Studio 2022
generator with `-T v142`.

## Exported functions

The current DLL exports exactly two C symbols:

- `DLCPU_GetVersion`
- `DLCPU_GetLastErrorMessage`

## VS2012 MFC client integration

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

This scaffold does not yet perform inference, image I/O, model loading, or
session management. Those APIs will be introduced in later tasks after the ABI,
build, and CI foundation is in place.

