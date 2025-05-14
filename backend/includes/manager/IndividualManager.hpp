#pragma once

#include <vector>

#include "data/coordinate.hpp"
#include "data/individual.hpp"

class IndividualManager {
 public:
  enum MutationPolicy { kSwap, kReverse, kUndefined };

  void SetMutationPolicy(const MutationPolicy& mutation_policy) {
    mutation_policy_ = mutation_policy;
  }

  // mutation rate should be between 1 and 0.
  bool SetMutationRate(const double mutation_rate) {
    if (mutation_rate < 0. || mutation_rate > 1) {
      return false;
    }

    mutation_rate_ = mutation_rate * 100;

    return true;
  }

  void SetMap(const std::vector<Coordinate>& map) { map_ = map; }

  std::vector<Coordinate> GetMap() { return map_; }

  bool MutateIndividual(Individual& individual) const;

  void ResetDistance(Individual& individual) const;
  void SwapCoords(size_t idx_1, size_t idx_2, Individual& individual) const;

 private:
  void ReverseMutation(Individual& individual) const;
  void RandomSwapMutation(Individual& individual) const;

  double mutation_rate_{0.};
  MutationPolicy mutation_policy_{kUndefined};

  std::vector<Coordinate> map_;
};
