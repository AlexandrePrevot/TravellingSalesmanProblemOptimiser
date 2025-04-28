#include "algorithms/PopulationGenerator.hpp"

#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <random>

#include "data/individual.hpp"
#include "data/coordinate.hpp"

static auto rng = std::default_random_engine {};

void PopulationGenerator::resetIndividual(Individual& individual, const double individual_size)
{
    std::vector<int>& coordinate_list = individual.getCoordinateList();
    coordinate_list.resize(individual_size);

    for (int i = 0; i < individual_size; i++)
    {
        coordinate_list[i] = i;
    }
}

void PopulationGenerator::initializeIndividual(Individual& individual, const double individual_size)
{
    std::srand(std::time(nullptr));
    auto& coordinate_list = individual.getCoordinateList();
    resetIndividual(individual, individual_size);
    std::shuffle(std::begin(coordinate_list), std::end(coordinate_list), rng);
}


// problems : always generating same population
Population PopulationGenerator::generateNewPopulation(const int population_size, const double individual_size)
{
    Population new_population;

    auto& individual_list = new_population.getPopulation();
    individual_list.resize(population_size);

    for (int i = 0; i < population_size; i++)
    {
        initializeIndividual(individual_list[i], individual_size);
    }

    return new_population;
}