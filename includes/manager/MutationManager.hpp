#pragma once

#include "manager/IndividualManager.hpp"

class MutationManager {
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

    // returns false if no mutation occurred
    bool mutateIndividual(Individual& individual) const;

private:

    void randomSwapMutation(Individual& individual) const;

    double m_mutation_rate{0.};
    MutationPolicy m_mutation_policy{kUndefined};
};