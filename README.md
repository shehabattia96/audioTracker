# audioTracker - Triangulates sound position from three microphones

## Getting Started

Run `install.sh -o windows|linux|mac`. To build run `install.sh -b -o windows|linux|mac`.

### VSCode support for toolchain

You'll need the `cmaketools` extension. Scan for kits then search the Pallete for "CMake: Edit Local-User CMake Kits and add `"toolchainFile": "${workspaceFolder}/externals/vcpkg/scripts/buildsystems/vcpkg.cmake"` to your kit. To build the toolchain, you must run `install.sh` at least once, you do not need to run it before each build.

## High Level Process

1. Time difference between microphones are capturing the same frequency that should be 10cm/330meters/sec = 0.303 milliseconds
2. Capturing at 48 Khz corresponds to 1/48000seconds = 0.021 milliseconds
3. We collect 256 samples @ 48 Khz in 5.4 ms