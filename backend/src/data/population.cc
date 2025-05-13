#include "data/population.hpp"

#include <iostream>

std::ostream &operator<<(std::ostream &stream, Population& population) {
    const std::vector<Individual>& individual_list = population.getPopulation();

    stream << "population :\n";

    for (std::size_t i = 0; i < individual_list.size(); i++) {
        stream << "individual " << i << " : { " << individual_list[i] << " }\n";
    }

    return stream;
}