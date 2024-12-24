#pragma once

#include "data/population.hpp"

namespace PopulationGenerator
{
    void resetIndividual(Individual& individual, const double individual_size, const double max_y, const double max_x, const double min_y, const double min_x);

    Population generateNewPopulation(const int population_size, const double individual_size, const double max_y, const double max_x, const double min_y, const double min_x);
}