#pragma once

#include <vector>

#include "data/individual.hpp"

namespace selector {
void cutPopulation(double rate, std::vector<Individual> &population);
}