#pragma once

#include "data/individual.hpp"

class IndividualManager
{
public:
    static void resetDistance(Individual &individual);
    static void swapCoords(size_t idx_1, size_t idx_2, Individual &individual);
    static void copyTo(Individual& to_copy, Individual& target);
};