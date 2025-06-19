cmake_minimum_required(VERSION 3.10)
project(MultiThreadedElementQueueComponent CXX)

set(CMAKE_CXX_STANDARD 20)

# Generate autoconf.h from Kconfig
set(KCONFIG_HEADER_FILE "${CMAKE_BINARY_DIR}/autoconf.h")

add_custom_command(
  OUTPUT ${KCONFIG_HEADER_FILE}
  COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/genconfig.py ${KCONFIG_HEADER_FILE}
  DEPENDS ${CMAKE_SOURCE_DIR}/Kconfig
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

add_custom_target(generate_kconfig_header DEPENDS ${KCONFIG_HEADER_FILE})

# Include the build directory for autoconf.h
include_directories(${CMAKE_BINARY_DIR})

# Main application
add_executable(my_app
    main.cpp
)
add_dependencies(my_app generate_kconfig_header)

# Fetch GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)
# Disable GoogleTest's install step
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

# Enable testing
enable_testing()

# Add your test executable
add_executable(tests
    tests/test_custom_queue.cpp
)

# Add your queue implementation to the test build
target_sources(tests PRIVATE
    CustomClass.hpp
    CustomClass.tpp
)

# Link test binary with GoogleTest and pthread
target_link_libraries(tests
    gtest_main
    pthread
)

# Discover and register tests
include(GoogleTest)
gtest_discover_tests(tests)
