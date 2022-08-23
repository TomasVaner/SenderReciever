if(USE_TEST)
    add_executable(test_CircularBuffer main.cpp)
    target_link_libraries(test_CircularBuffer gtest_main)

    gtest_discover_tests(test_CircularBuffer)
endif(USE_TEST)