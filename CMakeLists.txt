cmake_minimum_required(VERSION 3.22)
project(HFT_LOW_LATENCY)

#set(CMAKE_CXX_COMPILER "/usr/bin/aarch64-linux-gnu-g++-13")
set(CMAKE_CXX_STANDARD 20 CACHE STRING "The C++ standard to use")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}")
message(STATUS "CMAKE_CXX_STANDARD: ${CMAKE_CXX_STANDARD}")
message(STATUS "CXXFLAGS: ${CMAKE_CXX_FLAGS}")

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message(STATUS ">>> Linux OS detected")
    
    elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    message(STATUS ">>> MacOS detected")
endif()   
cmake_host_system_information(RESULT total_mem QUERY TOTAL_PHYSICAL_MEMORY)
message(STATUS ">>> Total physical memory: ${total_mem}")
cmake_host_system_information(RESULT num_cores QUERY NUMBER_OF_PHYSICAL_CORES)
message(STATUS ">>> Number of physical cores: ${num_cores}")

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.tar.gz
)

FetchContent_MakeAvailable(googletest)

enable_testing()
add_subdirectory(tests)