#include <grpcpp/grpcpp.h>

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "algorithms/genetic_algorithm.hpp"
#include "generated/request.grpc.pb.h"
#include "generated/request.pb.h"

/*
  TODO
  finish the readme
  remove venv from argument in .sh script
*/

class UpdateNotificationClient {
 public:
  explicit UpdateNotificationClient(
      const std::shared_ptr<grpc::Channel>& channel)
      : stub_(TSPO::Optimization::NewStub(channel)) {}

  bool WriteSolution(std::vector<int>& coordinate_list,
                     const std::vector<Coordinate>& map, double distance,
                     int generation) {
    const std::size_t size = map.size();

    TSPO::UpdateNotification notification;
    for (int i = 0; i < size; i++) {
      TSPO::Coordinate* notif_coord = notification.add_coordinates();
      notif_coord->set_coordx(map[coordinate_list[i]].GetX());
      notif_coord->set_coordy(map[coordinate_list[i]].GetY());
    }

    notification.set_score(distance);
    notification.set_generation(generation);

    grpc::ClientContext context;
    google::protobuf::Empty response;
    grpc::Status status = stub_->Update(&context, notification, &response);
    if (!status.ok()) {
      std::cout << "error in the message sending : " << status.error_message()
                << std::endl;
    }
    return true;
  }

 private:
  std::shared_ptr<TSPO::Optimization::Stub> stub_;
};

class OptimizationServiceImpl final : public TSPO::Optimization::Service {
  grpc::Status Optimize([[maybe_unused]] grpc::ServerContext* server_context,
                        const TSPO::OptimizationRequest* optimization_request,
                        TSPO::OptimizationReply* optimization_reply) override {
    std::srand(std::time(nullptr));

    const std::size_t size = optimization_request->coordinates_size();
    std::vector<Coordinate> map;
    map.reserve(size);

    for (int i = 0; i < size; i++) {
      const TSPO::Coordinate& coordinate = optimization_request->coordinates(i);
      Coordinate coord;
      coord.SetX(coordinate.coordx());
      coord.SetY(coordinate.coordy());
      map.push_back(coord);
    }

    GeneticAlgorithm algorithm;
    algorithm.SetIndividualSize(map.size());
    algorithm.SetPopulationSize(optimization_request->individualnumber());
    algorithm.SetSelectionRate(
        optimization_request->mutationrate());  // /!\ to fix
    algorithm.SetRealTimeCallBack(optimization_request->realtimeupdate());
    algorithm.SetMap(map);

    UpdateNotificationClient update_notification_client(
        grpc::CreateChannel("[::]:50052", grpc::InsecureChannelCredentials()));

    auto progress_lambda = [&update_notification_client, &map](
                               std::vector<int>& coordinate_list,
                               double distance, int generation) -> bool {
      return update_notification_client.WriteSolution(coordinate_list, map,
                                                      distance, generation);
    };
    algorithm.SetProgressCallback(progress_lambda);

    const bool success = algorithm.Process();

    std::vector<int> best = algorithm.BestIndividual().GetCoordinateList();

    for (int i = 0; i < size; i++) {
      TSPO::Coordinate* reply_coord = optimization_reply->add_coordinates();
      reply_coord->set_coordx(map[best[i]].GetX());
      reply_coord->set_coordy(map[best[i]].GetY());
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

int main() { RunServer(); }
