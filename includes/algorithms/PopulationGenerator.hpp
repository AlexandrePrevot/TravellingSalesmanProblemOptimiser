#pragma once

#include "data/population.hpp"

namespace PopulationGenerator
{
    Population generateNewPopulation(const int population_size, double max_y, double max_x, double min_y, double min_x);
}