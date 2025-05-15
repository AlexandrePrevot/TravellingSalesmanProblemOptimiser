#include "data/population.hpp"

#include <iostream>
#include <memory>
#include <vector>

std::ostream& operator<<(std::ostream& stream, Population& population) {
  const std::vector<std::shared_ptr<Individual>>& individual_list =
      population.GetPopulation();

  stream << "population :\n";

  for (std::size_t i = 0; i < individual_list.size(); i++) {
    stream << "individual " << i << " : { " << *individual_list[i] << " }\n";
  }

  return stream;
}
