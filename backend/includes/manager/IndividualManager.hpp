#pragma once

#include "data/individual.hpp"
#include "data/coordinate.hpp"

class IndividualManager
{
public:
    enum MutationPolicy {
        kSwap,
        kReverse,
        kUndefined
    };

    void setMutationPolicy(const MutationPolicy& mutation_policy)
    {
        m_mutation_policy = mutation_policy;
    }

    // mutation rate should be between 1 and 0.
    bool setMutationRate(const double mutation_rate)
    {
        if (mutation_rate < 0. || mutation_rate > 1)
        {
            return false;
        }

        m_mutation_rate = mutation_rate * 100;

        return true;
    }

    void setMap(const std::vector<Coordinate>& map) {
        m_map = map;
    }

    std::vector<Coordinate> getMap() {
        return m_map;
    }

    bool mutateIndividual(Individual& individual) const;

    void resetDistance(Individual &individual) const;
    void swapCoords(size_t idx_1, size_t idx_2, Individual &individual) const;

private:
    void reverseMutation(Individual& individual) const;
    void randomSwapMutation(Individual& individual) const;

    double m_mutation_rate{0.};
    MutationPolicy m_mutation_policy{kUndefined};

    std::vector<Coordinate> m_map;
};