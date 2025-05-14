cmake_minimum_required(VERSION 3.28.3)
project(TravellingSalesmanProblemOptimiser)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC REQUIRED)

set(algorithm_list
    cross_over_operator.cc
    PopulationGenerator.cc
    GeneticAlgorithm.cc
)

list(TRANSFORM algorithm_list PREPEND "src/algorithms/")

set(data_list
    individual.cc
    population.cc
)

list(TRANSFORM data_list PREPEND "src/data/")

set(manager_list
    IndividualManager.cc
)

list(TRANSFORM manager_list PREPEND "src/manager/")

set(grpc_list
    request.pb.cc
    request.grpc.pb.cc
)

list(TRANSFORM grpc_list PREPEND "generated/")

add_library(lib_algo STATIC ${algorithm_list})
target_include_directories(lib_algo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/includes)

add_library(lib_data STATIC ${data_list})
target_include_directories(lib_data PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/includes)

add_library(lib_manager STATIC ${manager_list})
target_include_directories(lib_manager PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/includes)

add_library(lib_grpc STATIC ${grpc_list})
target_link_libraries(lib_grpc PUBLIC gRPC::grpc++ protobuf::libprotobuf)

add_executable(${PROJECT_NAME} main.cc)
target_link_libraries(${PROJECT_NAME} PUBLIC lib_grpc)
target_link_libraries(${PROJECT_NAME} PUBLIC lib_algo)
target_link_libraries(${PROJECT_NAME} PUBLIC lib_data)
target_link_libraries(${PROJECT_NAME} PUBLIC lib_manager)