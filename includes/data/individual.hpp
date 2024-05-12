#include <vector>

#include "coordinate.hpp"

class Individual
{
public:
    Coordinate(const std::vector<Coordinate>& coordinate_list);

    

private:
    std::vector<Coordinate> coordinate_list_;

    double total_distance_;
};