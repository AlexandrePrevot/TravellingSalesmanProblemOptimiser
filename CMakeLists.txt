cmake_minimum_required(VERSION 3.28.3)
project(TravellingSalesmanProblemOptimiser)

set(algorithm_list
    selector.cc
)

list(TRANSFORM algorithm_list PREPEND "src/algorithms/")


message("here is the list ${algorithm_list}")


add_library(lib_algo STATIC ${algorithm_list})
target_include_directories(lib_algo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/includes)

add_executable(${PROJECT_NAME} main.cc)
target_link_libraries(${PROJECT_NAME} PUBLIC lib_algo)
