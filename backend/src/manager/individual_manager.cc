#include "manager/individual_manager.hpp"

#include <algorithm>
#include <utility>
#include <vector>

static inline double GetDistance(double x1, double x2, double y1, double y2) {
  const double x_dist{x1 - x2};
  const double y_dist{y1 - y2};

  return x_dist * x_dist + y_dist * y_dist;
}

bool IndividualManager::MutateIndividual(Individual &individual) const {
  const int random_value =
      std::rand() % 100;  // should try to get a double value
  if (random_value >= mutation_rate_ * 100) {
    return false;
  }

  if (mutation_policy_ == kSwap) {
    RandomSwapMutation(individual);
  }

  if (mutation_policy_ == kReverse) {
    ReverseMutation(individual);
  }

  return true;
}

void IndividualManager::ReverseMutation(Individual &individual) {
  std::vector<int> &coordinate_list = individual.GetCoordinateList();
  const std::size_t individual_size = coordinate_list.size();

  int random_idx_1 = std::rand() % individual_size;
  int random_idx_2 = std::rand() % individual_size;

  if (random_idx_1 > random_idx_2) {
    std::swap(random_idx_1, random_idx_2);
  }

  std::reverse(coordinate_list.begin() + random_idx_1,
               coordinate_list.begin() + random_idx_2);
}

void IndividualManager::RandomSwapMutation(Individual &individual) {
  std::vector<int> &coordinate_list = individual.GetCoordinateList();
  const std::size_t individual_size = coordinate_list.size();

  const int random_idx_1 = std::rand() % individual_size;
  const int random_idx_2 = std::rand() % individual_size;

  SwapCoords(random_idx_1, random_idx_2, individual);
}

void IndividualManager::ResetDistance(Individual &individual) const {
  double res_distance{0.};
  const std::vector<int> &coordinate_list = individual.GetCoordinateList();
  const std::size_t size = coordinate_list.size();

  if (size <= 1) {
    individual.SetTotalDistance(res_distance);
  }

  for (int i = 0; i < size - 1; i++) {
    Coordinate coord = map_[coordinate_list[i]];
    Coordinate next_coord = map_[coordinate_list[i + 1]];
    const double x_dist{coord.GetX() - next_coord.GetX()};
    const double y_dist{coord.GetY() - next_coord.GetY()};

    res_distance += x_dist * x_dist + y_dist * y_dist;
  }

  Coordinate start_coord = map_[coordinate_list[0]];
  Coordinate end_coord = map_[coordinate_list[size - 1]];

  const double x_dist{end_coord.GetX() - start_coord.GetX()};
  const double y_dist{end_coord.GetY() - start_coord.GetY()};

  res_distance += x_dist * x_dist + y_dist * y_dist;

  individual.SetTotalDistance(res_distance);
}

void IndividualManager::SwapCoords(size_t idx_1, size_t idx_2,
                                   Individual &individual) {
  std::vector<int> &coordinate_list{individual.GetCoordinateList()};
  const size_t size = coordinate_list.size();

  if (idx_1 < 0) {
    return;
  }

  if (idx_1 >= size) {
    return;
  }

  if (idx_2 < 0) {
    return;
  }

  if (idx_2 >= size) {
    return;
  }

  std::swap(coordinate_list.at(idx_1), coordinate_list.at(idx_2));
}
