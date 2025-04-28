#include <algorithm>

#include "manager/IndividualManager.hpp"

void IndividualManager::resetDistance(Individual &individual)
{
    double res_distance{0.};
    const std::vector<int> &coordinate_list = individual.getCoordinateList();

    /*
    if (coordinate_list.size() <= 1)
    {
        individual.setTotalDistance(res_distance);
    }

    double last_x = coordinate_list.at(0).getX();
    double last_y = coordinate_list.at(0).getY();

    for (const Coordinate &coord : coordinate_list)
    {
        const double x_dist{coord.getX() - last_x};
        const double y_dist{coord.getY() - last_y};

        res_distance += x_dist * x_dist + y_dist * y_dist;

        last_x = coord.getX();
        last_y = coord.getY();
    }
    */

    individual.setTotalDistance(res_distance);
}

void IndividualManager::swapCoords(size_t idx_1, size_t idx_2, Individual &individual)
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

void IndividualManager::copyTo(Individual& to_copy, Individual& target) {
    target.setCoordinateList(to_copy.getCoordinateList());
    target.setTotalDistance(to_copy.getTotalDistance());
}