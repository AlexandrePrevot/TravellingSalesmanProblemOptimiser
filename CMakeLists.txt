cmake_minimum_required(VERSION 3.28.3)
project(TravellingSalesmanProblemOptimiser)

set(algorithm_list
    selector.cc
)

list(TRANSFORM algorithm_list PREPEND "src/algorithms/")

set(data_list
    coordinate.cc
)

list(TRANSFORM data_list PREPEND "src/data/")

add_library(lib_algo STATIC ${algorithm_list})
target_include_directories(lib_algo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/includes)

add_library(lib_data STATIC ${data_list})
target_include_directories(lib_data PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/includes)

add_executable(${PROJECT_NAME} main.cc)
target_link_libraries(${PROJECT_NAME} PUBLIC lib_algo)
target_link_libraries(${PROJECT_NAME} PUBLIC lib_data)
