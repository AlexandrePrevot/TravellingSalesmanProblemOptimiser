#pragma once

#include <functional>
#include <vector>

#include "data/coordinate.hpp"
#include "data/individual.hpp"
#include "data/population.hpp"
#include "manager/individual_manager.hpp"

class GeneticAlgorithm {
 public:
  GeneticAlgorithm();

  void SetIndividualSize(const std::size_t individual_size) {
    individual_size_ = individual_size;
  }

  void SetPopulationSize(const std::size_t population_size) {
    population_size_ = population_size;
  }

  void SetSelectionRate(const double selection_rate) {
    selection_rate_ = selection_rate;
  }

  void SetMap(const std::vector<Coordinate>& map) {
    individual_manager_.SetMap(map);
  }

  void SetProgressCallback(const std::function<bool(std::vector<int>&, double,
                                                    int)>& progress_callback) {
    progress_callback_ = progress_callback;
  }

  void SetRealTimeCallBack(bool real_time_callback) {
    real_time_callback_ = real_time_callback;
  }

  Individual BestIndividual() { return best_individual_; }

  bool Process();  // returns wether the process is a success or not

 private:
  Population population_;
  Individual best_individual_;

  IndividualManager individual_manager_;

  std::size_t population_size_;
  std::size_t individual_size_;
  double selection_rate_;
  bool real_time_callback_;

  void SetUpPopulation();
  void SetUpBestIndividual();
  void SetUpIndividualManager();

  bool Cycle();

  std::function<bool(std::vector<int>&, double, int)> progress_callback_;
};
