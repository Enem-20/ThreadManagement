cmake_minimum_required(VERSION 3.21)

project(ThreadManagement LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS src/*.hpp)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_library(ThreadManagement SHARED)
target_sources(ThreadManagement PRIVATE ${HEADERS} ${SOURCES})

target_compile_definitions(ThreadManagement PRIVATE THREAD_MANAGEMENT_EXPORTS)
target_include_directories(ThreadManagement PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)