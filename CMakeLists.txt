## Please run install.sh before trying to build this to obtain all the dependencies and toolchain.
cmake_minimum_required(VERSION 3.11)
project( audioTracker VERSION 0.0.1 DESCRIPTION "Triangulates audio from three microphones" LANGUAGES C CXX)

set(VCPKG_EXTRN_PATH "${CMAKE_CURRENT_SOURCE_DIR}/externals/vcpkg" CACHE STRING "Path to vcpkg")

add_subdirectory(audioTracker)