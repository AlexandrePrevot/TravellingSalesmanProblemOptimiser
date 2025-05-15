#pragma once

#include <memory>
#include <vector>

#include "data/individual.hpp"

struct Population {
 public:
  std::vector<std::shared_ptr<Individual>>& GetPopulation() {
    return population_;
  }

  // 0 indexed
  friend std::ostream& operator<<(std::ostream& stream, Population& population);

 private:
  std::vector<std::shared_ptr<Individual>> population_;
};
