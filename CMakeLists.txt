cmake_minimum_required(VERSION 3.1)

project(Test LANGUAGES CXX)

set(CMAKE_C_COMPILER "g++")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Source Code
set(PROJECT_SRC
  src/main.cpp
)

# Executable
add_executable(${PROJECT_NAME} ${PROJECT_SRC})

# Include Directory
target_include_directories(${PROJECT_NAME}
  PUBLIC
)

# Link Library
target_link_libraries(${PROJECT_NAME} 
  PUBLIC
)

# Enable CUDA
message(STATUS "GPU Mode ON")
find_package(CUDAToolkit)
find_package(Thrust)

enable_language(CUDA)

set_target_properties(${PROJECT_NAME} 
PROPERTIES CUDA_ARCHITECTURES "75")

#target_sources(${PROJECT_NAME} PRIVATE src/main.cu)

target_link_libraries(${PROJECT_NAME} 
                      PRIVATE 
                      CUDA::cudart
                      CUDA::cublas
                      CUDA::cusparse
                      CUDA::cufft
                      CUDA::cusolver)
