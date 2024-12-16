#pragma once

struct Coordinate
{
    double getX() const
    {
        return m_x;
    }

    double getY() const
    {
        return m_y;
    }

    void setX(double coord_x)
    {
        m_x = coord_x;
    }

    void setY(double coord_y)
    {
        m_y = coord_y;
    }

private:
    double m_x{0.};
    double m_y{0.};
};