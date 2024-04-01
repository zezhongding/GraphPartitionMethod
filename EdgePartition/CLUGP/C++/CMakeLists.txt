cmake_minimum_required(VERSION 3.10)

project(CLUGP)
set(CMAKE_CXX_STANDARD 11)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
find_package(Threads REQUIRED)
FIND_PACKAGE(OpenMP REQUIRED)
if(OPENMP_FOUND)
message("OPENMP FOUND")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

set(EXECUTABLE_OUTPUT_PATH "./bin/")


add_executable(main main.cpp 
                Clugp.cpp
                CluSP.cpp
                ClusterGameTask.cpp
                ClusterPackGame.cpp
                ClusterTask.cpp
                Edge.cpp
                EdgeGameTask.cpp
                EdgePartitionGame.cpp
                FileConvert.cpp
                FileProcess.cpp
                globalConfig.cpp
                StreamCluster.cpp
                readGraph.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

target_link_libraries(main PRIVATE Threads::Threads)