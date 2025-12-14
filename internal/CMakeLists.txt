cmake_minimum_required(VERSION 3.5)

project(ThreadManagement)

file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE HEADERS src/*.hpp)

add_library(ThreadManagement)

set_target_properties(ThreadManagement PROPERTIES
	CXX_STANDARD 23
	CXX_STANDARD_REQUIRED On
	CXX_EXTENSIONS Off
)

target_sources(ThreadManagement PRIVATE ${SOURCES})

file(INSTALL ${HEADERS} DESTINATION ../../include)

target_link_libraries(ThreadManagement INTERFACE hopscotch_map)

target_include_directories(ThreadManagement PUBLIC ../include)

target_include_directories(ThreadManagement PRIVATE ../external/hopscotch-map/include)