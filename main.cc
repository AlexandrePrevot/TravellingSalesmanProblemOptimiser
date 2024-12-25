#include <iostream>
#include <vector>

#include "algorithms/selector.hpp"
#include "algorithms/PopulationGenerator.hpp"
#include "algorithms/cross_over_operator.hpp"

#include "data/coordinate.hpp"
#include "data/individual.hpp"

#include "manager/IndividualManager.hpp"
#include "manager/MutationManager.hpp"

int main()
{
    Coordinate coord_1;
    coord_1.setX(10.);
    coord_1.setY(10.);

    Coordinate coord_2;
    coord_2.setX(20.);
    coord_2.setY(10.);

    Individual ind({0, 1});

    Individual ind_2(ind);

    Individual ind_3(ind);

    std::vector<Individual> vec_of_ind({ind, ind_2, ind_3});

    std::cout << " individual : " << ind << std::endl;

    std::cout << "number of individual " << vec_of_ind.size() << std::endl;
    selector::cutPopulation(0.99, vec_of_ind);
    std::cout << "number of individual " << vec_of_ind.size() << std::endl;

    std::cout << "distance of ind is " << ind.getTotalDistance() << std::endl;
    IndividualManager::resetDistance(ind);
    std::cout << "distance of ind is " << ind.getTotalDistance() << std::endl;


    // move mutation manager into /algorithms
    std::cout << "mutation " << std::endl;
    MutationManager mutation_manager;
    mutation_manager.setMutationRate(0.75);
    mutation_manager.setMutationPolicy(MutationManager::kSwap);

    mutation_manager.mutateIndividual(ind);
    std::cout << " individual : " << ind << std::endl;


    std::cout << "population generator" << std::endl;

    Population population = PopulationGenerator::generateNewPopulation(10, 15);

    std::cout << population << std::endl;

    std::cout << "cross over result between 1 and 2" << std::endl;

    auto& result_population = population.getPopulation();

    cross_over::crossOver(result_population[1], result_population[2]);

}