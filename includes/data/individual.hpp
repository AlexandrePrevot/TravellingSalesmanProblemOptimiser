#pragma once

#include <vector>

#include "coordinate.hpp"

class Individual
{
    using CoordinateList = std::vector<Coordinate>;

public:
    Individual(const CoordinateList &coordinate_list);

    ~Individual();

    Individual(const Individual &individual);

    Individual &operator=(const Individual &individual) = delete;

    Individual(Individual &&individual) noexcept;

    CoordinateList getCoordinateList() const;
    double getTotalDistance() const;

    void setCoordinateList(const CoordinateList &coordinate_list);
    void setTotalDistance(double distance);

private:
    CoordinateList m_coordinate_list;

    double m_total_distance;
};