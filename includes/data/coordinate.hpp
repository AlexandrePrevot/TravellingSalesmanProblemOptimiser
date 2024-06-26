#pragma once

struct Coordinate
{
    double getX() const;
    double getY() const;

    void setX(double coord_x);
    void setY(double coord_y);

private:
    double m_x{0.};
    double m_y{0.};
};