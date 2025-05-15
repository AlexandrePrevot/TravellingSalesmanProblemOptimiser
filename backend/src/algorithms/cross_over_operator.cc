#include "algorithms/cross_over_operator.hpp"

#include <climits>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "data/individual.hpp"

static inline void AddToAdjacencyMatrix(
    std::unordered_map<int, std::unordered_set<int>>& adjacency_matrix,
    const std::vector<int>& coord_list) {
  const std::size_t length = coord_list.size();
  for (int i = 0; i < length; i++) {
    const int current_node = coord_list[i];

    const int next_node = (i + 1 < length) ? coord_list[i + 1] : coord_list[0];
    const int previous_node =
        (i != 0) ? coord_list[i - 1] : coord_list[length - 1];

    adjacency_matrix[current_node].insert(next_node);

    adjacency_matrix[current_node].insert(previous_node);
  }
}

static inline void RemoveFromNeighboursList(
    std::unordered_map<int, std::unordered_set<int>>& adjacency_matrix,
    int to_remove) {
  auto neighbour_list = adjacency_matrix[to_remove];

  for (const int neighbour : neighbour_list) {
    const auto neighbour_neighbours_list = adjacency_matrix.find(neighbour);

    if (neighbour_neighbours_list == std::end(adjacency_matrix)) {
      continue;
    }

    neighbour_neighbours_list->second.erase(to_remove);
  }
}

static std::unordered_map<int, std::unordered_set<int>> BuildAdjacencyMatrix(
    const std::vector<int>& coord_list1, const std::vector<int>& coord_list2) {
  const std::size_t length = coord_list1.size();
  std::unordered_map<int, std::unordered_set<int>> adjacency_matrix;

  adjacency_matrix.reserve(length);

  AddToAdjacencyMatrix(adjacency_matrix, coord_list1);
  AddToAdjacencyMatrix(adjacency_matrix, coord_list2);

  return adjacency_matrix;
}

static void PrintMatrix(
    const std::unordered_map<int, std::unordered_set<int>>& adjacency_matrix) {
  std::cout << "adjcency matrix : " << std::endl;
  for (const auto& [node, linked_to] : adjacency_matrix) {
    std::cout << " node : " << node << " linked to : { ";

    for (const int linked_node : linked_to) {
      std::cout << linked_node << ", ";
    }

    std::cout << " } \n";
  }
}

static int GetNextNode(
    std::unordered_map<int, std::unordered_set<int>>& adjacency_matrix,
    int removed_node) {
  const auto neighbour_list = adjacency_matrix[removed_node];

  adjacency_matrix.erase(removed_node);

  if (adjacency_matrix.empty()) {
    return -1;
  }

  int min_nbr_of_neighbour = adjacency_matrix.size();
  int node_with_less_neighbour = adjacency_matrix.size();

  bool found = false;

  for (const int neighbour : neighbour_list) {
    auto neighbour_neighbours_list = adjacency_matrix.find(neighbour);

    if (neighbour_neighbours_list == std::end(adjacency_matrix)) {
      continue;
    }

    if (min_nbr_of_neighbour > neighbour_neighbours_list->second.size()) {
      found = true;
      node_with_less_neighbour = neighbour_neighbours_list->first;
      min_nbr_of_neighbour = neighbour_neighbours_list->second.size();
    }
  }

  if (found) {
    return node_with_less_neighbour;
  }

  // if the list of neighbours is empty
  // just take a random node
  if (neighbour_list.empty()) {
    const std::size_t length = adjacency_matrix.size();
    const int next_node = rand() % length;

    auto it = std::begin(adjacency_matrix);

    for (int i = 0; i < next_node; i++) {
      it++;
    }

    return it->first;
  }

  return node_with_less_neighbour;
}

Individual cross_over::CrossOver(Individual& parent1, Individual& parent2) {
  Individual child;

  auto& child_coordinate_list = child.GetCoordinateList();
  const int desired_length = parent1.GetCoordinateList().size();

  child_coordinate_list.reserve(desired_length);

  auto adjacency_matrix = BuildAdjacencyMatrix(parent1.GetCoordinateList(),
                                               parent2.GetCoordinateList());

  int next_node = rand() % desired_length;

  while (child_coordinate_list.size() < desired_length) {
    child_coordinate_list.push_back(next_node);
    RemoveFromNeighboursList(adjacency_matrix, next_node);
    next_node = GetNextNode(adjacency_matrix, next_node);
  }

  return child;
}
