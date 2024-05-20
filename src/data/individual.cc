#include "data/individual.hpp"

#include <iostream>
#include <vector>

#include "data/coordinate.hpp"

Individual::Individual(const std::vector<Coordinate> &coordinate_list)
{
    m_coordinate_list = coordinate_list;
    m_total_distance = 0.;
}

Individual::~Individual()
{
}

Individual::Individual(const Individual &individual)
{
    m_coordinate_list = individual.m_coordinate_list;
    m_total_distance = individual.m_total_distance;
}

Individual::Individual(Individual &&individual) noexcept
{
    m_total_distance = individual.m_total_distance;
    m_coordinate_list = std::move(individual.m_coordinate_list);

    individual.m_coordinate_list.clear();
    individual.m_total_distance = 0.;
}

std::vector<Coordinate> &Individual::getCoordinateList()
{
    return m_coordinate_list;
}

double Individual::getTotalDistance() const
{
    return m_total_distance;
}

void Individual::setCoordinateList(const std::vector<Coordinate> &coordinate_list)
{
    m_coordinate_list = coordinate_list;
}

void Individual::setTotalDistance(double distance)
{
    m_total_distance = distance;
}

std::ostream &operator<<(std::ostream &stream, const Individual &individual)
{
    const std::vector<Coordinate> &coordinate_list = individual.m_coordinate_list;
    stream << "coordinate : ";
    for (const Coordinate &coords : coordinate_list)
    {
        stream << "x : " << coords.getX() << " y : " << coords.getY() << ", ";
    }

    stream << "; distance : " << individual.getTotalDistance();

    return stream;
}