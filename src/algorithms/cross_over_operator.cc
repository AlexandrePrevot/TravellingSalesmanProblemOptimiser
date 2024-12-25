#include "algorithms/cross_over_operator.hpp"

#include "data/individual.hpp"

#include <unordered_set>
#include <unordered_map>



static inline void addToAdjacencyMatrix(std::unordered_map<int, std::unordered_set<int>>& adjacency_matrix, const std::vector<int>& coord_list)
{
    const std::size_t length = coord_list.size();
    for (int i = 0; i < length; i++)
    {
        const int current_node1 = coord_list[i];

        const int next_node1 = (i+1 < length) ? coord_list[i+1] : -1;
        const int previous_node1 = (i != 0) ? coord_list[i-1] : -1;

        if (next_node1 != -1)
        {
            adjacency_matrix[current_node1].insert(next_node1);
        }

        if (previous_node1 != -1)
        {
            adjacency_matrix[current_node1].insert(previous_node1);
        }
    }
}

static inline void removeFromNeighboursList(std::unordered_map<int, std::unordered_set<int>>& adjacency_matrix, int to_remove)
{
    auto neighbour_list = adjacency_matrix[to_remove];

    for (const int neighbour : neighbour_list)
    {
        const auto neighbour_neighbours_list = adjacency_matrix.find(neighbour);

        if (neighbour_neighbours_list == std::end(adjacency_matrix))
        {
            continue;
        }

        neighbour_neighbours_list->second.erase(to_remove);

        if (neighbour_neighbours_list->second.empty())
        {
            adjacency_matrix.erase(neighbour_neighbours_list);
        }
    }
}

static std::unordered_map<int, std::unordered_set<int>> buildAdjacencyMatrix(const std::vector<int>& coord_list1, const std::vector<int>& coord_list2)
{
    const std::size_t length = coord_list1.size();
    std::unordered_map<int, std::unordered_set<int>> adjacency_matrix;
    
    adjacency_matrix.reserve(length);

    addToAdjacencyMatrix(adjacency_matrix, coord_list1);
    addToAdjacencyMatrix(adjacency_matrix, coord_list2);

    return adjacency_matrix;
}


static void printMatrix(const std::unordered_map<int, std::unordered_set<int>>& adjacency_matrix)
{
    std::cout << "adjcency matrix : " << std::endl;
    for (const auto &[node, linked_to] : adjacency_matrix)
    {
        std::cout << " node : " << node << " linked to : { ";

        for (const int linked_node : linked_to) {
            std::cout << linked_node << ", ";
        }

        std::cout << " } \n";
    }
}

static int getNextNode(std::unordered_map<int, std::unordered_set<int>>& adjacency_matrix, int removed_node)
{
    /*
        we have removed node

        check if removed node has neighbours, take the one with the fewest neighbours
        if equal take a random one

        if no neighbours are found for removed node, take random next node in adjacency matrix
    */

    const auto neighbour_list = adjacency_matrix[removed_node];

    if (neighbour_list.empty())
    {
        adjacency_matrix.erase(removed_node);
        

        const std::size_t length = adjacency_matrix.size();
        const int next_node = rand()%(length);

        auto it = std::begin(adjacency_matrix);
        
        for (int i = 0; i < next_node; i++)
        {
            it++;
        }

        return it->first;
    }

    int node_with_less_neighbour = adjacency_matrix.size();

    for (const int neighbour : neighbour_list)
    {
        const auto neighbour_neighbours_list = adjacency_matrix.find(neighbour);

        if (neighbour_neighbours_list == std::end(adjacency_matrix)) {
            continue;
        }

        if (node_with_less_neighbour > neighbour_neighbours_list->second.size()) {
            node_with_less_neighbour = neighbour_neighbours_list->first;
        }
    }

    return node_with_less_neighbour;
    
}

// see https://stackoverflow.com/questions/44376721/proper-edge-recombination-crossover-for-dna-assembly
// for opimization
Individual cross_over::crossOver(Individual& parent1, Individual& parent2)
{
    Individual child;

    auto adjacency_matrix = buildAdjacencyMatrix(parent1.getCoordinateList(), parent2.getCoordinateList());


    printMatrix(adjacency_matrix);

    
    removeFromNeighboursList(adjacency_matrix, 3);

    std::cout << "after remove" << std::endl;

    printMatrix(adjacency_matrix);

    std::cout << "next node : " << getNextNode(adjacency_matrix, 3) << std::endl;


    return child;
}