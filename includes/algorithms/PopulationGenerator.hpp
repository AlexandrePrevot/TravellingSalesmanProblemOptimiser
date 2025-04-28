#pragma once

#include "data/population.hpp"

namespace PopulationGenerator
{
    void resetIndividual(Individual& individual, const double individual_size);
    void initializeIndividual(Individual& individual, const double individual_size);

    Population generateNewPopulation(const int population_size, const double individual_size);
}