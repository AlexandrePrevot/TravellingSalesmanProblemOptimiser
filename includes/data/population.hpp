#pragma once

#include "data/individual.hpp"

class Population 
{
public:
    std::vector<Individual>& getPopulation()
    {
        return m_population;
    }

    void setPopulation(const std::vector<Individual>& population)
    {
        m_population = population;
    }

private:
    std::vector<Individual> m_population;
};