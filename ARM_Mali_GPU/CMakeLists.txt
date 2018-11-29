cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenCL REQUIRED)

message(${OpenCL_LIBRARIES})
message(${OpenCL_FOUND})
message(${OpenCL_INCLUDE_DIRS})

add_executable(gpuInfo GPUInfo.cpp)
target_link_libraries(gpuInfo ${OpenCL_LIBRARIES})
