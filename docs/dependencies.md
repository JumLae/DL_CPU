# Dependencies

## ONNX Runtime 1.22.0

DL_CPU pins ONNX Runtime to the official NuGet package
`Microsoft.ML.OnnxRuntime/1.22.0`. The reason is Windows x86 coverage:
version `1.22.0` is the newest official package that still ships both
`runtimes/win-x86/native/*` and `runtimes/win-x64/native/*`. Newer
`1.23.x` packages dropped the `win-x86` prebuilt, which would break the
Win32 compatibility requirement for legacy MFC clients. The package is
downloaded at configure time via CMake `FetchContent` and verified with a
hard-coded SHA-256.

## OpenCV via vcpkg manifest mode

OpenCV is consumed through `vcpkg.json` in manifest mode so local builds
and CI use the exact same dependency graph and the same pinned
`builtin-baseline` commit. This keeps `find_package(OpenCV REQUIRED
COMPONENTS core imgproc imgcodecs)` reproducible across Win32 and x64
without vendoring OpenCV sources into the repository. We intentionally use
`default-features: false` plus `opencv4[world]` so the runtime bundle stays
small enough to document and audit.

OpenCV는 [world] feature를 예외적으로 허용함. 근거: 번들 단순화.

With the current `opencv4[world]` build from vcpkg, the build tree
co-locates `opencv_world4.dll` and `zlib1.dll`. `opencv_world4.dll` also
imports the Microsoft CRT DLLs `MSVCP140.dll`, `CONCRT140.dll`,
`VCRUNTIME140.dll`, and `VCRUNTIME140_1.dll`, plus the corresponding
`api-ms-win-crt-*` loader shims. Those CRT DLLs are not part of our public
ABI, but deployment must still account for them on machines that do not
already provide the required Microsoft runtime.

## /MT and /MD coexistence

`dl_cpu.dll` and every target we build ourselves use `/MT` so our DLL does
not require the Visual C++ Redistributable. OpenCV and ONNX Runtime remain
their official prebuilt `/MD` DLLs. This coexistence is safe because each
DLL keeps its own CRT state and our public ABI never transfers ownership of
memory allocated by one CRT to another. The rule is strict: no allocation in
`dl_cpu.dll` may be freed by third-party DLLs, and no allocation from
OpenCV or ONNX Runtime may be freed through our ABI boundary.
