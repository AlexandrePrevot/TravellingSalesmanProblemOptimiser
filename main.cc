#include <iostream>
#include <vector>

#include <cstdlib>
#include <ctime>

#include <grpcpp/grpcpp.h>

#include "algorithms/selector.hpp"
#include "algorithms/PopulationGenerator.hpp"
#include "algorithms/cross_over_operator.hpp"

#include "data/coordinate.hpp"
#include "data/individual.hpp"

#include "manager/IndividualManager.hpp"
#include "manager/MutationManager.hpp"

#include "algorithms/GeneticAlgorithm.hpp"

#include "messages/generated/request.grpc.pb.h"
#include "messages/generated/request.pb.h"

/*
    TODO
    Use pointers to individual in Population
    In individual manager : don't recalculate the distance entirely when swaping
    maj to every file
    in cross over ERO : take a random neighbour if they have same number of neihbours

    use a specific formatted style
    make sure everything is const when needed (sometimes I had to do weird trickes, you'll see)


*/

class GreeterServiceImpl final : public TSPO::Greeter::Service {
    grpc::Status SayHello(grpc::ServerContext* server_context, const TSPO::HelloRequest* hello_request, TSPO::HelloReply* hello_reply) override {
        const std::string prefix("Hello ");
        std::cout << "received a message with name " << hello_request->name() << std::endl;
        hello_reply->set_message(prefix + hello_request->name());
        return grpc::Status::OK;
    }
};

void RunServer() {
  std::string server_address("0.0.0.0:50051");
  GreeterServiceImpl service;

  grpc::ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);

  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;
  server->Wait();
}


int main()
{
    /*
    Coordinate coord_1;
    coord_1.setX(10.);
    coord_1.setY(10.);

    Coordinate coord_2;
    coord_2.setX(20.);
    coord_2.setY(10.);

    Individual ind({0, 1});

    Individual ind_2(ind);

    Individual ind_3(ind);

    std::vector<Individual> vec_of_ind({ind, ind_2, ind_3});

    std::cout << " individual : " << ind << std::endl;

    std::cout << "number of individual " << vec_of_ind.size() << std::endl;
    selector::cutPopulation(0.99, vec_of_ind);
    std::cout << "number of individual " << vec_of_ind.size() << std::endl;

    std::cout << "distance of ind is " << ind.getTotalDistance() << std::endl;
    IndividualManager::resetDistance(ind);
    std::cout << "distance of ind is " << ind.getTotalDistance() << std::endl;


    // move mutation manager into /algorithms
    std::cout << "mutation " << std::endl;
    MutationManager mutation_manager;
    mutation_manager.setMutationRate(0.75);
    mutation_manager.setMutationPolicy(MutationManager::kSwap);

    mutation_manager.mutateIndividual(ind);
    std::cout << " individual : " << ind << std::endl;


    std::cout << "population generator" << std::endl;

    Population population = PopulationGenerator::generateNewPopulation(10, 15);

    std::cout << population << std::endl;

    std::cout << "cross over result between 1 and 2" << std::endl;

    auto& result_population = population.getPopulation();

    const auto child = cross_over::crossOver(result_population[1], result_population[2]);

    std::cout << child << std::endl;
    */

    RunServer();

    std::srand(std::time(0));

    std::vector<Coordinate> map;
#define Create(x,y) \
{\
    Coordinate coord;\
    coord.setX(x);\
    coord.setY(y); \
    map.push_back(coord);\
}

    Create(-17, 35);
    Create(-11, 1);
    Create(23, 11);
    Create(10, 5);
    Create(3, 0);
    Create(0, 0);
    Create(21, 21);
    Create(1, 34);
    Create(3, 9);
    Create(9, 13);

#undef Create

    GeneticAlgorithm algorithm;
    algorithm.setIndividualSize(map.size());
    algorithm.setPopulationSize(20);
    algorithm.setSelectionRate(0.1);
    algorithm.setMap(map);

    const bool success = algorithm.process();

    std::cout << "is it a success : " << success << std::endl;

}