cmake_minimum_required(VERSION 3.5)

project(ThreadManagement)

add_subdirectory(external/hopscotch-map)

add_subdirectory(internal)
if(${BUILD_TESTS})
	add_subdirectory(test)
endif()
