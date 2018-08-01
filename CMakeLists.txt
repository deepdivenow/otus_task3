cmake_minimum_required(VERSION 3.9)
project(pr_allocator VERSION 0.0.0$ENV{TRAVIS_BUILD_NUMBER})
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING
            "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel."
            FORCE)
endif(NOT CMAKE_BUILD_TYPE)
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
set(CMAKE_CXX_FLAGS "-Wpedantic -Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(CMAKE_CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED ON)

add_executable(allocator main.cpp)
add_executable(test_allocator test.cpp)

install(TARGETS allocator RUNTIME DESTINATION bin)

find_package(GTest REQUIRED)

set_target_properties(test_allocator PROPERTIES
        INCLUDE_DIRECTORIES ${GTEST_INCLUDE_DIRS}
        )

target_link_libraries(test_allocator GTest::GTest GTest::Main)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT deepdivenow@gmail.com)
set(CPACK_PACKAGE_NAME allocator)

include (CPack)

enable_testing()
add_test(test_allocator_valid test_allocator)