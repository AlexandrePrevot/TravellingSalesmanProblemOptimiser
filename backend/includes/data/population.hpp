#pragma once

#include <vector>

#include "data/individual.hpp"

struct Population {
 public:
  std::vector<Individual>& GetPopulation() { return population_; }

  // 0 indexed
  friend std::ostream& operator<<(std::ostream& stream, Population& population);

 private:
  std::vector<Individual> population_;
};
