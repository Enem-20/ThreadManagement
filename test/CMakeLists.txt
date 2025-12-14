project(ThreadManagement-test)

add_executable(ThreadManagement-test)

target_sources(ThreadManagement-test PRIVATE src/main.cpp)

target_link_libraries(ThreadManagement-test PRIVATE ThreadManagement)