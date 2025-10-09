cmake_minimum_required(VERSION 3.31)
project(weave)

set(CMAKE_CXX_STANDARD 20)


find_package(OpenCV REQUIRED)

if(ENABLE_LOGGING)
    find_package(spdlog REQUIRED)
endif()

add_subdirectory(source)
