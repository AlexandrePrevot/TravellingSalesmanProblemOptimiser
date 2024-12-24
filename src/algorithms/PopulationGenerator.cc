#include "algorithms/PopulationGenerator.hpp"

#include <cassert>
#include <cstdlib>
#include <ctime>

#include "data/individual.hpp"
#include "data/coordinate.hpp"


void PopulationGenerator::resetIndividual(Individual& individual, const double individual_size, const double max_y, const double max_x, const double min_y, const double min_x)
{
    std::vector<Coordinate>& coordinate_list = individual.getCoordinateList();
    coordinate_list.resize(individual_size);

    for (int i = 0; i < individual_size; i++)
    {
        const int x_diff = max_x - min_x;
        const int y_diff = max_y - min_y;

        const int pos_x = (std::rand() % x_diff) + min_x;
        const int pos_y = (std::rand() % y_diff) + min_y;

        coordinate_list[i].setX(pos_x);
        coordinate_list[i].setY(pos_y);
    }
}

Population PopulationGenerator::generateNewPopulation(const int population_size, const double individual_size, double max_y, double max_x, double min_y, double min_x)
{
    assert((max_y > min_y) && (max_x > min_x));

    std::srand(std::time(nullptr));

    Population new_population;

    auto& individual_list = new_population.getPopulation();
    individual_list.resize(population_size);

    for (int i = 0; i < population_size; i++)
    {
        resetIndividual(individual_list[i], population_size, max_y, max_x, min_y, min_x);
    }

    return new_population;
}