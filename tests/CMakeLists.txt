add_executable(Google_Tests_run
        UtilsTests.cpp
)

target_link_libraries(Google_Tests_run
        PRIVATE
        gtest_main
        euler_common
)

include(GoogleTest)
gtest_discover_tests(Google_Tests_run)