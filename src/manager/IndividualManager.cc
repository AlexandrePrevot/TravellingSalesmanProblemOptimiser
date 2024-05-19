#include "manager/IndividualManager.hpp"

void IndividualManager::resetDistance(Individual &individual)
{
    double res_distance{0.};
    const std::vector<Coordinate> &coordinate_list = individual.getCoordinateList();

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

    individual.setTotalDistance(res_distance);
}