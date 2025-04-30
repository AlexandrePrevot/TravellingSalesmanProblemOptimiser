#include "algorithms/GeneticAlgorithm.hpp"

#include <algorithm>

#include "algorithms/selector.hpp"
#include "algorithms/PopulationGenerator.hpp"
#include "algorithms/cross_over_operator.hpp"

#include "manager/MutationManager.hpp"
#include "manager/IndividualManager.hpp"

static constexpr int kMaxStagnationCount = 50;
static constexpr int kMaxPopulationSize = 1000000;
static constexpr int kMaxIndividualSize = 1000000;

GeneticAlgorithm::GeneticAlgorithm() : m_population_size(0), m_individual_size(0), m_selection_rate(-1.) {}


static void sortPopulation(Population& unsorted_population) {
    auto& population = unsorted_population.getPopulation();
    std::sort(population.begin(), population.end(), [](const Individual& left, const Individual& right) { return left.getTotalDistance() < right.getTotalDistance(); });
}

void GeneticAlgorithm::setUpPopulation() {
    m_population = PopulationGenerator::generateNewPopulation(m_population_size, m_individual_size);
    auto& population = m_population.getPopulation();

    for (Individual& individual : population) {
        m_individual_manager.resetDistance(individual);
    }

    sortPopulation(m_population);
}

void GeneticAlgorithm::setUpBestIndividual() {
    m_best_individual = m_population.getPopulation()[0];

    for (auto individual : m_population.getPopulation()) {
        if (individual.getTotalDistance() < m_best_individual.getTotalDistance()) {
            m_best_individual = individual;
        }
    }
}

bool GeneticAlgorithm::cycle() {
    /*
        take first m_selection_rate individuals

        delete all the others, create offsprings from the first individuals
        for each offspring do random mutation (according to mutation rate)
        sort all the individuals
    */

    const std::size_t to_keep = m_population_size * m_selection_rate;

    int individual_created = 0;

    for (int i = 0; i < to_keep; i++) {
        for (int j = 1; j < to_keep; j++) {
            if (individual_created + to_keep >= m_population_size) {
                break;
            }

            auto& individual_to_eliminate = m_population.getPopulation()[individual_created + to_keep];
            individual_to_eliminate = cross_over::crossOver(m_population.getPopulation()[i], m_population.getPopulation()[j]);
            m_individual_manager.resetDistance(individual_to_eliminate);
        }

        if (individual_created + to_keep >= m_population_size) {
            break;
        }
    }

    sortPopulation(m_population);
    return true;
}

bool GeneticAlgorithm::process() {
    if (m_individual_size <= 0) {
        return false;
    }

    if (m_population_size <= 0) {
        return false;
    }

    if (m_individual_size >= kMaxIndividualSize) {
        return false;
    }

    if (m_population_size >= kMaxPopulationSize) {
        return false;
    }

    if (m_selection_rate <= 0) {
        return false;
    }

    if (m_selection_rate >= 1) {
        return false;
    }

    if (m_individual_manager.getMap().size() != m_individual_size) {
        return false;
    }

    setUpPopulation();
    setUpBestIndividual();

    int stagnation_count_guard = 0;


    std::cout << "old population " << std::endl;
    std::cout << m_population << std::endl;
    while (stagnation_count_guard <= kMaxStagnationCount) {
        const double current_best = m_best_individual.getTotalDistance();
        cycle();
        const double new_best = m_best_individual.getTotalDistance();

        if (new_best < current_best) {
            stagnation_count_guard = 0;
            continue;
        }

        stagnation_count_guard++;
    }

    std::cout << "new population " << std::endl;
    std::cout << m_population << std::endl;

    return true;
}