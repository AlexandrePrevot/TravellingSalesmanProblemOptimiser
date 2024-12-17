#pragma once

#include "data/population"

class PopulationManager
{
public:
    void generateNewPopulation();

    Population& getPopulation()
    {
        return m_population;
    }

    bool setPopulationSize(const int population_size)
    {
        m_population_size = population_size;
    }

private:

    int m_population_size{0};
    Population m_population;
};