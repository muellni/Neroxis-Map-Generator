cmake_minimum_required(VERSION 3.13)

project(mapgen)

set(CMAKE_CXX_STANDARD 17)

find_package (Eigen3 3.3 REQUIRED NO_MODULE)

add_executable(mapgen
    generator/MapGenerator.cpp
#    map/BinaryMask.cpp
)
target_include_directories(mapgen PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries (mapgen Eigen3::Eigen)

add_executable(testgen
    test/gen.cpp
)
target_include_directories(testgen PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries (testgen Eigen3::Eigen stdc++fs)

add_executable(benchmark
    test/benchmark.cpp
)
target_include_directories(benchmark PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries (benchmark Eigen3::Eigen)
