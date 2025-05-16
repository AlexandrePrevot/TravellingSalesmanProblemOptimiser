#pragma once

#include "data/population.hpp"

namespace population_generator {
void ResetIndividual(Individual& individual, double individual_size);
void InitializeIndividual(Individual& individual, double individual_size);

Population GenerateNewPopulation(int population_size,
                                 double individual_size);
}  // namespace population_generator
