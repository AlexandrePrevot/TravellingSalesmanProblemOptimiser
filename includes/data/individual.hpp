#pragma once

#include <iostream>
#include <vector>

#include "coordinate.hpp"

struct Individual
{
    using CoordinateList = std::vector<int>;

public:
    Individual() = default;
    Individual(const CoordinateList &coordinate_list);

    ~Individual();

    Individual(const Individual &individual);

    Individual &operator=(const Individual &individual) = delete;

    Individual(Individual &&individual) noexcept;

    CoordinateList& getCoordinateList();
    double getTotalDistance() const;

    void setCoordinateList(const CoordinateList &coordinate_list);
    void setTotalDistance(double distance);

    friend std::ostream &operator<<(std::ostream &stream, const Individual &individual);

private:
    CoordinateList m_coordinate_list;

    double m_total_distance;
};