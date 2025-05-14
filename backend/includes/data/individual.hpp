#pragma once

#include <iostream>
#include <vector>

struct Individual {
  using CoordinateList = std::vector<int>;

 public:
  Individual() = default;
  explicit Individual(const CoordinateList &coordinate_list);

  ~Individual() = default;

  Individual(const Individual &individual);

  // TODO(XXX): only use pointers to individual
  // Individual &operator=(const Individual &individual) = delete; was stopping
  // the sorting
  Individual &operator=(const Individual &individual);

  Individual(Individual &&individual) noexcept;

  CoordinateList &GetCoordinateList() { return coordinate_list_; }
  double GetTotalDistance() const { return total_distance_; }

  void SetCoordinateList(const CoordinateList &coordinate_list) {
  coordinate_list_ = coordinate_list;
}
  void SetTotalDistance(double distance) {
  total_distance_ = distance;
}

  friend std::ostream &operator<<(std::ostream &stream,
                                  const Individual &individual);

 private:
  CoordinateList coordinate_list_;

  double total_distance_;
};
