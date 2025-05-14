#include "data/individual.hpp"

#include <iostream>
#include <utility>
#include <vector>

Individual::Individual(const std::vector<int> &coordinate_list) {
  coordinate_list_ = coordinate_list;
  total_distance_ = 0.;
}

Individual::Individual(const Individual &individual) {
  coordinate_list_ = individual.coordinate_list_;
  total_distance_ = individual.total_distance_;
}

Individual &Individual::operator=(const Individual &individual) {
  if (this == &individual) {
    return *this;
  }

  coordinate_list_ = individual.coordinate_list_;
  total_distance_ = individual.total_distance_;

  return *this;
}

Individual::Individual(Individual &&individual) noexcept {
  total_distance_ = individual.total_distance_;
  coordinate_list_ = std::move(individual.coordinate_list_);

  individual.coordinate_list_.clear();
  individual.total_distance_ = 0.;
}


std::ostream &operator<<(std::ostream &stream, const Individual &individual) {
  const std::vector<int> &coordinate_list = individual.coordinate_list_;
  stream << "coordinate : ";
  for (const int position : coordinate_list) {
    stream << position << "; ";
  }

  stream << " distance : " << individual.GetTotalDistance();

  return stream;
}
