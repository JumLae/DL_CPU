# Dependencies

## OpenCV world feature exception

OpenCV uses the `opencv4[world]` feature as an approved exception to the
general "no feature flags" rule. The reason is deployment simplicity: the
runtime bundle and CI dependency audit are intentionally designed around a
single `opencv_world<VER>.dll` instead of many module DLLs, which reduces
field maintenance complexity for industrial installation environments.

