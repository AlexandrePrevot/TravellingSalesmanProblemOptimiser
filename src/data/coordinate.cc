#include "data/coordinate.hpp"

double Coordinate::getX() const
{
    return m_x;
}

double Coordinate::getY() const
{
    return m_y;
}

void Coordinate::setX(double coord_x)
{
    m_x = coord_x;
}

void Coordinate::setY(double coord_y)
{
    m_y = coord_y;
}
