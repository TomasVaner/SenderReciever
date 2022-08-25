cmake_minimum_required(VERSION 3.0.0)
project(SendReciever VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

find_package(OpenSSL REQUIRED)

include(CTest)

add_executable(sender sender.cpp)
target_link_libraries(sender OpenSSL::SSL)
add_executable(reciever reciever.cpp)
target_link_libraries(reciever OpenSSL::SSL)

option(USE_TEST "Use CTest" ON)

if(USE_TEST)
    enable_testing()
    add_subdirectory(tests)
endif(USE_TEST)
add_subdirectory(libs)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)