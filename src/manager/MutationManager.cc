#include "manager/MutationManager.hpp"

#include <cstdlib>
#include <ctime>

bool MutationManager::mutateIndividual(Individual& individual) const
{
    std::srand(std::time(nullptr));

    const int random_value = std::rand() % 100; // should try to get a double value

    if (random_value >= m_mutation_rate) {
        return false;
    }

    if (m_mutation_policy == kSwap) {
        randomSwapMutation(individual);
    }

    return true;
}

void MutationManager::randomSwapMutation(Individual& individual) const
{
    std::vector<int> &coordinate_list = individual.getCoordinateList();
    const std::size_t individual_size = coordinate_list.size();

    const int random_idx_1 = std::rand() % individual_size;
    const int random_idx_2 = std::rand() % individual_size;

    IndividualManager::swapCoords(random_idx_1, random_idx_2, individual);
}