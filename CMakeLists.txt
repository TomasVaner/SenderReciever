cmake_minimum_required(VERSION 3.0.0)
project(SendReciever VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)

INCLUDE_DIRECTORIES(/usr/local/src/openssl-1.1.1q/include)
include(CTest)

add_executable(sender sender.cpp)
add_executable(reciever reciever.cpp)

add_subdirectory(tests)
add_subdirectory(libs)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)


enable_testing()
