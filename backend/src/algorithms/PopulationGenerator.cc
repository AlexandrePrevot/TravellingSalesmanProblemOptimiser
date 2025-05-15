#include "algorithms/PopulationGenerator.hpp"

#include <algorithm>
#include <memory>
#include <random>
#include <vector>

#include "data/individual.hpp"

static auto rng = std::default_random_engine{std::random_device{}()};

void population_generator::ResetIndividual(Individual& individual,
                                           const double individual_size) {
  std::vector<int>& coordinate_list = individual.GetCoordinateList();
  coordinate_list.resize(individual_size);

  for (int i = 0; i < individual_size; i++) {
    coordinate_list[i] = i;
  }
}

void population_generator::InitializeIndividual(Individual& individual,
                                                const double individual_size) {
  auto& coordinate_list = individual.GetCoordinateList();
  ResetIndividual(individual, individual_size);
  std::shuffle(std::begin(coordinate_list), std::end(coordinate_list), rng);
}

// problems : always generating same population
Population population_generator::GenerateNewPopulation(
    const int population_size, const double individual_size) {
  Population new_population;

  auto& individual_list = new_population.GetPopulation();
  individual_list.resize(population_size);

  for (int i = 0; i < population_size; i++) {
    individual_list[i] = std::make_shared<Individual>();
    InitializeIndividual(*individual_list[i], individual_size);
  }

  return new_population;
}
