cmake_minimum_required(VERSION 3.0.0)
project(cmd-chat VERSION 0.1.0)

include(CTest)
enable_testing()

# fix linux pthread error
find_package(Threads)

add_executable(server main.cpp src/server.h src/user.h src/message.h src/server.cpp)
add_executable(client src/client.cpp)

target_link_libraries (server ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries (client ${CMAKE_THREAD_LIBS_INIT})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
