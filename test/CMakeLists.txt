add_executable(matrix_test MatrixTest.cpp)

set_target_properties(matrix_test PROPERTIES 
    CMAKE_CXX_STANDARD 14 
    CXX_STANDARD_REQUIRED ON
)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)

if (WIN32)
    # Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

FetchContent_MakeAvailable(googletest)

target_include_directories(matrix_test PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(matrix_test GTest::gtest GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(matrix_test)

enable_testing()