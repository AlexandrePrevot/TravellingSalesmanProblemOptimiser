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

    if (m_mutation_policy == kReverse) {

    }

    return true;
}

void IndividualManager::reverseMutation(Individual& individual) const
{
    std::vector<int> &coordinate_list = individual.getCoordinateList();
    const std::size_t individual_size = coordinate_list.size();

    const int random_idx_1 = std::rand() % individual_size;
    const int random_idx_2 = std::rand() % individual_size;

    if (random_idx_1 > random_idx_2) {
        std::reverse(coordinate_list.begin() + random_idx_2, coordinate_list.begin() + random_idx_1);
    } else {
        std::reverse(coordinate_list.begin() + random_idx_1, coordinate_list.begin() + random_idx_2);
    }

    resetDistance(individual);
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
    const std::size_t size = coordinate_list.size();

    if (size <= 1) {
        individual.setTotalDistance(res_distance);
    }


    for (int i = 0; i < size - 1; i++) {
        Coordinate coord = m_map[coordinate_list[i]];
        Coordinate next_coord = m_map[coordinate_list[i+1]];
        const double x_dist{coord.getX() - next_coord.getX()};
        const double y_dist{coord.getY() - next_coord.getY()};

        res_distance += x_dist * x_dist + y_dist * y_dist;
    }

    Coordinate start_coord = m_map[coordinate_list[0]];
    Coordinate end_coord = m_map[coordinate_list[size-1]];

    const double x_dist{end_coord.getX() - start_coord.getX()};
    const double y_dist{end_coord.getY() - start_coord.getY()};

    res_distance += x_dist * x_dist + y_dist * y_dist;

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
