#include "algorithms/cross_over_operator.hpp"

#include <unordered_map>
#include <unordered_set>

#include "data/individual.hpp"

static inline void addToAdjacencyMatrix(
    std::unordered_map<int, std::unordered_set<int>>& adjacency_matrix,
    const std::vector<int>& coord_list) {
  const std::size_t length = coord_list.size();
  for (int i = 0; i < length; i++) {
    const int current_node1 = coord_list[i];

    // /!\ /!\ WARNING
    // to fix, if end of list, next node is start of list
    const int next_node1 = (i + 1 < length) ? coord_list[i + 1] : -1;
    const int previous_node1 = (i != 0) ? coord_list[i - 1] : -1;

    if (next_node1 != -1) {
      adjacency_matrix[current_node1].insert(next_node1);
    }

    if (previous_node1 != -1) {
      adjacency_matrix[current_node1].insert(previous_node1);
    }
  }
}

static inline void removeFromNeighboursList(
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

static std::unordered_map<int, std::unordered_set<int>> buildAdjacencyMatrix(
    const std::vector<int>& coord_list1, const std::vector<int>& coord_list2) {
  const std::size_t length = coord_list1.size();
  std::unordered_map<int, std::unordered_set<int>> adjacency_matrix;

  adjacency_matrix.reserve(length);

  addToAdjacencyMatrix(adjacency_matrix, coord_list1);
  addToAdjacencyMatrix(adjacency_matrix, coord_list2);

  return adjacency_matrix;
}

static void printMatrix(
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

static int getNextNode(
    std::unordered_map<int, std::unordered_set<int>>& adjacency_matrix,
    int removed_node) {
  const auto neighbour_list = adjacency_matrix[removed_node];

  adjacency_matrix.erase(removed_node);

  if (adjacency_matrix.empty()) return -1;

  int min_nbr_of_neighbour = adjacency_matrix.size();
  int node_with_less_neighbour = adjacency_matrix.size();

  bool found = false;

  for (const int neighbour : neighbour_list) {
    auto neighbour_neighbours_list = adjacency_matrix.find(neighbour);

    if (neighbour_neighbours_list == std::end(adjacency_matrix)) {
      continue;
    }

    // /!\ WARNING /!\
        // take a random one among neighbours having the same number of neighbours here ?

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

// see
// https://stackoverflow.com/questions/44376721/proper-edge-recombination-crossover-for-dna-assembly
// for opimization
Individual cross_over::crossOver(Individual& parent1, Individual& parent2) {
  Individual child;

  auto& child_coordinate_list = child.GetCoordinateList();
  const int desired_length = parent1.GetCoordinateList().size();

  child_coordinate_list.reserve(desired_length);

  auto adjacency_matrix = buildAdjacencyMatrix(parent1.GetCoordinateList(),
                                               parent2.GetCoordinateList());

  int next_node = rand() % desired_length;

  while (child_coordinate_list.size() < desired_length) {
    child_coordinate_list.push_back(next_node);
    removeFromNeighboursList(adjacency_matrix, next_node);
    next_node = getNextNode(adjacency_matrix, next_node);
  }

  return child;
}