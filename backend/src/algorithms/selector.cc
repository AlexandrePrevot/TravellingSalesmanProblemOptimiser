#include <iostream>
#include <vector>

#include "algorithms/selector.hpp"
#include "data/individual.hpp"

void selector::cutPopulation(double rate, std::vector<Individual> &population)
{
    // useless because we will overwrite data next time
    const size_t new_size = rate * population.size();
    population.resize(new_size);
}
