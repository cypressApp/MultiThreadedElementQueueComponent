cmake_minimum_required(VERSION 3.10)
project(MultiThreadedElementQueueComponent CXX)

set(CMAKE_CXX_STANDARD 20)

set(KCONFIG_HEADER_FILE "${CMAKE_BINARY_DIR}/autoconf.h")

add_custom_command(
  OUTPUT ${KCONFIG_HEADER_FILE}
  COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/genconfig.py ${KCONFIG_HEADER_FILE}
  DEPENDS ${CMAKE_SOURCE_DIR}/Kconfig
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

add_custom_target(generate_kconfig_header DEPENDS ${KCONFIG_HEADER_FILE})
add_executable(my_app main.cpp)

add_dependencies(my_app generate_kconfig_header)
include_directories(${CMAKE_BINARY_DIR})
