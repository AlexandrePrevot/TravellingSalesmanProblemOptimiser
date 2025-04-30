#include "manager/IndividualManager.hpp"

#include <algorithm>
#include <cstdlib>
#include <ctime>


bool IndividualManager::mutateIndividual(Individual& individual) const
{
    const int random_value = std::rand() % 100; // should try to get a double value
    if (random_value >= m_mutation_rate*100) {
        return false;
    }

    if (m_mutation_policy == kSwap) {
        randomSwapMutation(individual);
    }

    return true;
}

void IndividualManager::randomSwapMutation(Individual& individual) const
{
    std::vector<int> &coordinate_list = individual.getCoordinateList();
    const std::size_t individual_size = coordinate_list.size();

    const int random_idx_1 = std::rand() % individual_size;
    const int random_idx_2 = std::rand() % individual_size;

    swapCoords(random_idx_1, random_idx_2, individual);
}

void IndividualManager::resetDistance(Individual &individual) const
{
    double res_distance{0.};
    const std::vector<int> &coordinate_list = individual.getCoordinateList();

    if (coordinate_list.size() <= 1)
    {
        individual.setTotalDistance(res_distance);
    }

    double last_x = m_map.at(coordinate_list.at(0)).getX();
    double last_y = m_map.at(coordinate_list.at(0)).getY();

    for (const int &coord_idx : coordinate_list)
    {
        Coordinate coord = m_map.at(coord_idx);
        const double x_dist{coord.getX() - last_x};
        const double y_dist{coord.getY() - last_y};

        res_distance += x_dist * x_dist + y_dist * y_dist;

        last_x = coord.getX();
        last_y = coord.getY();
    }

    individual.setTotalDistance(res_distance);
}

void IndividualManager::swapCoords(size_t idx_1, size_t idx_2, Individual &individual) const
{
    std::vector<int> &coordinate_list{individual.getCoordinateList()};
    const size_t size = coordinate_list.size();

    if (idx_1 < 0)
    {
        return;
    }

    if (idx_1 >= size)
    {
        return;
    }

    if (idx_2 < 0)
    {
        return;
    }

    if (idx_2 >= size)
    {
        return;
    }

    std::swap(coordinate_list.at(idx_1), coordinate_list.at(idx_2));
    resetDistance(individual);
}
