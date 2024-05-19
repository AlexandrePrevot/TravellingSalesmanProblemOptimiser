#include <iostream>
#include <vector>

#include "data/coordinate.hpp"
#include "data/individual.hpp"
#include "manager/IndividualManager.hpp"

int main()
{
    Coordinate coord_1;
    coord_1.setX(10.);
    coord_1.setY(10.);

    Coordinate coord_2;
    coord_2.setX(20.);
    coord_2.setY(10.);

    Individual ind({coord_1, coord_2});

    std::cout << "distance of ind is " << ind.getTotalDistance() << std::endl;
    IndividualManager::resetDistance(ind);
    std::cout << "distance of ind is " << ind.getTotalDistance() << std::endl;
}