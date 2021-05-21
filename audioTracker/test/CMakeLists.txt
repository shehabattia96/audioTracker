message("Building AudioTracker_Tests")
cmake_minimum_required(VERSION 3.11)
set( CMAKE_VERBOSE_MAKEFILE ON )

project( AudioTracker_Tests C CXX)

get_filename_component( APP_PATH "${CMAKE_CURRENT_SOURCE_DIR}/" ABSOLUTE )


#### Catch2
find_package(Catch2 CONFIG REQUIRED) # uses vcpkg toolchain
set(VCPKG_LIBRARIES ${VCPKG_LIBRARIES} Catch2::Catch2)

#### Test sources
set(
    sources
    ${APP_PATH}/test_main.cpp
    # ${APP_PATH}/test_file.cpp
)


add_executable (AudioTracker_Tests ${sources})
include_directories(${VCPKG_INCLUDES})
target_link_libraries(AudioTracker_Tests LINK_PUBLIC ${VCPKG_LIBRARIES})

#### copy resources directory to build dir:
add_custom_command(TARGET AudioTracker_Tests POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory "${APP_PATH}/resources/" "$<TARGET_FILE_DIR:AudioTracker_Tests>/resources")


message("Done building AudioTracker_Tests")   