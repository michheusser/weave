cmake_minimum_required(VERSION 3.31)
project(weave)

set(CMAKE_CXX_STANDARD 20)

# Default: logging enabled in debug, disabled in release
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    option(ENABLE_LOGGING "Enable logging" OFF)
    option(ENABLE_MONITORING "Enable monitoring" OFF)
else()
    option(ENABLE_LOGGING "Enable logging" ON)
    option(ENABLE_MONITORING "Enable monitoring" ON)
endif()

message(STATUS "Logging Enabled: ${ENABLE_LOGGING}")
message(STATUS "Monitoring Enabled: ${ENABLE_MONITORING}")


if(ENABLE_LOGGING)
    find_package(spdlog REQUIRED)
endif()

add_subdirectory(source)
add_subdirectory(test)
