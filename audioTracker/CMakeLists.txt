message("Building AudioTracker") 
cmake_minimum_required(VERSION 3.11)
set( CMAKE_VERBOSE_MAKEFILE ON )
project( AudioTracker C CXX )

get_filename_component( APP_PATH "${CMAKE_CURRENT_SOURCE_DIR}/" ABSOLUTE )

#### include rtAudio via VCPKG
find_package(RtAudio CONFIG REQUIRED)


#### VCPKG include and link list
set(VCPKG_INCLUDES ${VCPKG_INCLUDES})
set(VCPKG_LIBRARIES 
	${VCPKG_LIBRARIES}
	# rtaudio libraries
	RtAudio::rtaudio
	)


add_executable (AudioTracker main.cpp)
include_directories(${VCPKG_INCLUDES})
target_link_libraries(AudioTracker LINK_PUBLIC ${VCPKG_LIBRARIES})

#### copy resources directory to build dir:
add_custom_command(TARGET AudioTracker POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory "${APP_PATH}/resources/" "$<TARGET_FILE_DIR:AudioTracker>/resources")

#### tests	
include(CTest)
if(BUILD_TESTING)
  add_subdirectory(test)
endif()


message("Done building AudioTracker")  
