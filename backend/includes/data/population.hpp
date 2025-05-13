#pragma once

#include "data/individual.hpp"

struct Population 
{
public:
    std::vector<Individual>& getPopulation()
    {
        return m_population;
    }

    // 0 indexed
    friend std::ostream &operator<<(std::ostream &stream, Population& population);

private:
    std::vector<Individual> m_population;
};