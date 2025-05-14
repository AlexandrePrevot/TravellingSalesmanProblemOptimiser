#include <iostream>
#include <memory>
#include <cstdlib>
#include <ctime>
#include <vector>

#include <grpcpp/grpcpp.h>

#include "algorithms/selector.hpp"
#include "algorithms/PopulationGenerator.hpp"
#include "algorithms/cross_over_operator.hpp"

#include "data/coordinate.hpp"
#include "data/individual.hpp"

#include "manager/IndividualManager.hpp"
#include "manager/MutationManager.hpp"

#include "algorithms/GeneticAlgorithm.hpp"

#include "generated/request.grpc.pb.h"
#include "generated/request.pb.h"

/*
    TODO
    Use pointers to individual in Population
    In individual manager : don't recalculate the distance entirely when swaping
    in cross over ERO : take a random neighbour if they have same number of neihbours
    in GeneticAlgorithm::cycle fix new individual generation (currently maybe not enough are created)
    handle edge cases of submission : for example selection rate must select at least 1 individual
    maybe using long instead of double would accelerate the computing ?

    make sure everything is const when needed (sometimes I had to do weird trickes, you'll see)

    /!\ generate proto message with script /!\
    use c++ google style guide
    make a docker file ?
*/

class UpdateNotificationClient {
public:
    UpdateNotificationClient(const std::shared_ptr<grpc::Channel>& channel) : stub_(TSPO::Optimization::NewStub(channel)) {}

    bool WriteSolution(std::vector<int>& coordinate_list, const std::vector<Coordinate>& map, double distance, int generation) {
        const std::size_t size = map.size();

        TSPO::UpdateNotification notification;
        for (int i = 0; i < size; i++) {
            TSPO::Coordinate* notif_coord = notification.add_coordinates();
            notif_coord->set_coordx(map[coordinate_list[i]].getX());
            notif_coord->set_coordy(map[coordinate_list[i]].getY());
        }

        notification.set_score(distance);
        notification.set_generation(generation);

        std::cout << "sending update message" << std::endl;

        grpc::ClientContext context;
        google::protobuf::Empty response;
        grpc::Status status = stub_->Update(&context, notification, &response);
        if (status.ok()) {
            std::cout << "message update done" << std::endl;
        } else {
            std::cout << "error in the message sending : " << status.error_message() << std::endl;
        }
        return true;
    }

private:
    std::shared_ptr<TSPO::Optimization::Stub> stub_;
};


class OptimizationServiceImpl final : public TSPO::Optimization::Service {

    grpc::Status Optimize(grpc::ServerContext* server_context, const TSPO::OptimizationRequest* optimization_request, TSPO::OptimizationReply* optimization_reply) {
        std::cout << "received message" << std::endl;
        
        const std::size_t size = optimization_request->coordinates_size();
        std::vector<Coordinate> map;
        map.reserve(size);
        
        for (int i = 0; i < size; i++) {
            const TSPO::Coordinate& coordinate = optimization_request->coordinates(i);
            std::cout << "coordX : " << coordinate.coordx() << " coordY : " << coordinate.coordy() << std::endl;
            Coordinate coord;
            coord.setX(coordinate.coordx());
            coord.setY(coordinate.coordy());
            map.push_back(coord);
        }

        GeneticAlgorithm algorithm;
        algorithm.setIndividualSize(map.size());
        algorithm.setPopulationSize(optimization_request->individualnumber());
        algorithm.setSelectionRate(optimization_request->mutationrate()); // /!\ to fix
        algorithm.setMap(map);

        UpdateNotificationClient update_notification_client(grpc::CreateChannel("[::]:50052", grpc::InsecureChannelCredentials()));

        auto progress_lambda = [&update_notification_client, &map] (std::vector<int>& coordinate_list, double distance, int generation) -> bool
        {
            return update_notification_client.WriteSolution(coordinate_list, map, distance, generation);
        };
        algorithm.setProgressCallback(progress_lambda);

        const bool success = algorithm.process();

        std::vector<int> best = algorithm.best_individual().getCoordinateList();

        for (int i = 0; i < size; i++) {
            TSPO::Coordinate* reply_coord = optimization_reply->add_coordinates();
            reply_coord->set_coordx(map[best[i]].getX());
            reply_coord->set_coordy(map[best[i]].getY());
        }

        return grpc::Status::OK;
    }
};



void RunServer() {
  const std::string server_address("0.0.0.0:50051");
  OptimizationServiceImpl optimization_service;

  grpc::ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&optimization_service);

  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
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

    //UpdateNotificationClient client(grpc::CreateChannel("localhost:50052", grpc::InsecureChannelCredentials()));

    std::srand(std::time(0));// wasn't initializing random before ?

    RunServer();

}