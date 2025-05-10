#pragma once

#include "manager/IndividualManager.hpp"

#include "data/coordinate.hpp"
#include "data/individual.hpp"
#include "data/population.hpp"
#include "data/coordinate.hpp"

class GeneticAlgorithm {
public:
    GeneticAlgorithm();

    void setIndividualSize(const std::size_t individual_size) {
        m_individual_size = individual_size;
    }

    void setPopulationSize(const std::size_t population_size) {
        m_population_size = population_size;
    }

    void setSelectionRate(const double selection_rate) {
        m_selection_rate = selection_rate;
    }

    void setMap(const std::vector<Coordinate>& map) {
        m_individual_manager.setMap(map);
    }

    Individual best_individual() {
        return m_best_individual;
    }

    bool process(); // returns wether the process is a success or not

private:
    Population m_population;
    Individual m_best_individual;

    IndividualManager m_individual_manager;

    std::size_t m_population_size;
    std::size_t m_individual_size;
    double m_selection_rate;

    void setUpPopulation();
    void setUpBestIndividual();
    void setUpIndividualManager();

    bool cycle();
};