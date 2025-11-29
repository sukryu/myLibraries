/**
 * @file graph.hpp
 * @brief Graph implementation using adjacency list representation
 * @author Jinhyeok
 * @date 2025-11-29
 * @version 1.0.0
 * 
 * Copyright (c) 2025 Jinhyeok
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef MYLIB_GRAPH_GRAPH_HPP
#define MYLIB_GRAPH_GRAPH_HPP

#include <cstddef>
#include <stdexcept>
#include <utility>
#include <vector>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stack>
#include <functional>
#include <limits>
#include <algorithm>

namespace mylib {
namespace graph {

/**
 * @class Graph
 * @brief A graph implementation using adjacency list representation
 * 
 * This class implements a graph data structure that supports both directed
 * and undirected graphs, with optional edge weights. It uses an adjacency
 * list representation for efficient storage of sparse graphs.
 * 
 * Features:
 * - Directed and undirected graphs
 * - Weighted edges
 * - Graph traversals (BFS, DFS)
 * - Shortest path algorithms
 * - Cycle detection
 * - Topological sort (for DAGs)
 * 
 * @tparam Vertex The type of vertex identifiers
 * @tparam Weight The type of edge weights (default: double)
 */
template <typename Vertex, typename Weight = double>
class Graph {
public:
    // Type aliases
    using vertex_type = Vertex;
    using weight_type = Weight;
    using size_type = std::size_t;
    
    /**
     * @struct Edge
     * @brief Represents an edge in the graph
     */
    struct Edge {
        Vertex from;        ///< Source vertex
        Vertex to;          ///< Destination vertex
        Weight weight;      ///< Edge weight
        
        Edge(const Vertex& f, const Vertex& t, Weight w = Weight{1})
            : from(f), to(t), weight(w) {}
        
        bool operator==(const Edge& other) const {
            return from == other.from && to == other.to;
        }
    };

private:
    /**
     * @struct Neighbor
     * @brief Represents a neighbor vertex with edge weight
     */
    struct Neighbor {
        Vertex vertex;      ///< Adjacent vertex
        Weight weight;      ///< Edge weight to this neighbor
        
        Neighbor(const Vertex& v, Weight w) : vertex(v), weight(w) {}
    };

    using AdjacencyList = std::list<Neighbor>;
    using AdjacencyMap = std::unordered_map<Vertex, AdjacencyList>;

public:
    /**
     * @brief Default constructor
     * Creates an empty directed graph
     */
    Graph();

    /**
     * @brief Constructor with direction specification
     * @param directed true for directed graph, false for undirected
     */
    explicit Graph(bool directed);

    /**
     * @brief Constructor with initial vertices
     * @param vertices List of vertices to add
     * @param directed true for directed graph, false for undirected
     */
    Graph(std::initializer_list<Vertex> vertices, bool directed = true);

    /**
     * @brief Copy constructor
     * @param other Graph to copy from
     */
    Graph(const Graph& other);

    /**
     * @brief Move constructor
     * @param other Graph to move from
     */
    Graph(Graph&& other) noexcept;

    /**
     * @brief Destructor
     */
    ~Graph() = default;

    /**
     * @brief Copy assignment operator
     * @param other Graph to copy from
     * @return Reference to this graph
     */
    Graph& operator=(const Graph& other);

    /**
     * @brief Move assignment operator
     * @param other Graph to move from
     * @return Reference to this graph
     */
    Graph& operator=(Graph&& other) noexcept;

    // Properties
    /**
     * @brief Check if graph is directed
     * @return true if directed, false if undirected
     */
    bool is_directed() const noexcept;

    /**
     * @brief Check if graph is empty (no vertices)
     * @return true if empty
     */
    bool empty() const noexcept;

    /**
     * @brief Get number of vertices
     * @return Number of vertices
     */
    size_type vertex_count() const noexcept;

    /**
     * @brief Get number of edges
     * @return Number of edges
     */
    size_type edge_count() const noexcept;

    // Vertex operations
    /**
     * @brief Add a vertex to the graph
     * @param vertex Vertex to add
     * @return true if added, false if already exists
     */
    bool add_vertex(const Vertex& vertex);

    /**
     * @brief Remove a vertex and all its edges
     * @param vertex Vertex to remove
     * @return true if removed, false if not found
     */
    bool remove_vertex(const Vertex& vertex);

    /**
     * @brief Check if vertex exists
     * @param vertex Vertex to check
     * @return true if exists
     */
    bool has_vertex(const Vertex& vertex) const;

    /**
     * @brief Get all vertices
     * @return Vector of all vertices
     */
    std::vector<Vertex> vertices() const;

    /**
     * @brief Get degree of a vertex (number of edges)
     * @param vertex Vertex to check
     * @return Degree of vertex
     * @throws std::out_of_range if vertex not found
     */
    size_type degree(const Vertex& vertex) const;

    /**
     * @brief Get in-degree of a vertex (for directed graphs)
     * @param vertex Vertex to check
     * @return In-degree of vertex
     */
    size_type in_degree(const Vertex& vertex) const;

    /**
     * @brief Get out-degree of a vertex (for directed graphs)
     * @param vertex Vertex to check
     * @return Out-degree of vertex
     */
    size_type out_degree(const Vertex& vertex) const;

    // Edge operations
    /**
     * @brief Add an edge between two vertices
     * @param from Source vertex
     * @param to Destination vertex
     * @param weight Edge weight (default: 1)
     * @return true if added, false if edge already exists
     * 
     * For undirected graphs, adds edges in both directions.
     * Automatically adds vertices if they don't exist.
     */
    bool add_edge(const Vertex& from, const Vertex& to, Weight weight = Weight{1});

    /**
     * @brief Remove an edge between two vertices
     * @param from Source vertex
     * @param to Destination vertex
     * @return true if removed, false if not found
     */
    bool remove_edge(const Vertex& from, const Vertex& to);

    /**
     * @brief Check if edge exists
     * @param from Source vertex
     * @param to Destination vertex
     * @return true if edge exists
     */
    bool has_edge(const Vertex& from, const Vertex& to) const;

    /**
     * @brief Get weight of an edge
     * @param from Source vertex
     * @param to Destination vertex
     * @return Edge weight
     * @throws std::out_of_range if edge not found
     */
    Weight get_weight(const Vertex& from, const Vertex& to) const;

    /**
     * @brief Set weight of an edge
     * @param from Source vertex
     * @param to Destination vertex
     * @param weight New weight
     * @return true if updated, false if edge not found
     */
    bool set_weight(const Vertex& from, const Vertex& to, Weight weight);

    /**
     * @brief Get all edges
     * @return Vector of all edges
     */
    std::vector<Edge> edges() const;

    /**
     * @brief Get neighbors of a vertex
     * @param vertex Vertex to get neighbors of
     * @return Vector of neighbor vertices
     * @throws std::out_of_range if vertex not found
     */
    std::vector<Vertex> neighbors(const Vertex& vertex) const;

    /**
     * @brief Get neighbors with weights
     * @param vertex Vertex to get neighbors of
     * @return Vector of (neighbor, weight) pairs
     */
    std::vector<std::pair<Vertex, Weight>> neighbors_with_weights(const Vertex& vertex) const;

    // Traversals
    /**
     * @brief Breadth-first search traversal
     * @param start Starting vertex
     * @param visitor Function to call for each visited vertex
     */
    void bfs(const Vertex& start, std::function<void(const Vertex&)> visitor) const;

    /**
     * @brief Depth-first search traversal
     * @param start Starting vertex
     * @param visitor Function to call for each visited vertex
     */
    void dfs(const Vertex& start, std::function<void(const Vertex&)> visitor) const;

    /**
     * @brief Depth-first search (recursive version)
     * @param start Starting vertex
     * @param visitor Function to call for each visited vertex
     */
    void dfs_recursive(const Vertex& start, std::function<void(const Vertex&)> visitor) const;

    // Path finding
    /**
     * @brief Find shortest path using BFS (unweighted)
     * @param from Source vertex
     * @param to Destination vertex
     * @return Vector of vertices in path, empty if no path
     */
    std::vector<Vertex> shortest_path_bfs(const Vertex& from, const Vertex& to) const;

    /**
     * @brief Find shortest path using Dijkstra (weighted, non-negative)
     * @param from Source vertex
     * @param to Destination vertex
     * @return Pair of (path, total distance), empty path if no path
     */
    std::pair<std::vector<Vertex>, Weight> dijkstra(const Vertex& from, const Vertex& to) const;

    /**
     * @brief Find all shortest distances from a vertex (Dijkstra)
     * @param from Source vertex
     * @return Map of vertex to shortest distance
     */
    std::unordered_map<Vertex, Weight> dijkstra_all(const Vertex& from) const;

    /**
     * @brief Check if path exists between two vertices
     * @param from Source vertex
     * @param to Destination vertex
     * @return true if path exists
     */
    bool has_path(const Vertex& from, const Vertex& to) const;

    // Graph properties
    /**
     * @brief Check if graph contains a cycle
     * @return true if cycle exists
     */
    bool has_cycle() const;

    /**
     * @brief Check if graph is connected
     * @return true if connected (all vertices reachable from any vertex)
     * 
     * For directed graphs, checks weak connectivity.
     */
    bool is_connected() const;

    /**
     * @brief Topological sort (for directed acyclic graphs)
     * @return Vector of vertices in topological order
     * @throws std::runtime_error if graph contains a cycle
     */
    std::vector<Vertex> topological_sort() const;

    /**
     * @brief Get connected components
     * @return Vector of vectors, each containing vertices in a component
     */
    std::vector<std::vector<Vertex>> connected_components() const;

    // Utility
    /**
     * @brief Clear all vertices and edges
     */
    void clear() noexcept;

    /**
     * @brief Swap contents with another graph
     * @param other Graph to swap with
     */
    void swap(Graph& other) noexcept;

    /**
     * @brief Create a transposed (reversed) graph
     * @return New graph with all edges reversed
     */
    Graph transpose() const;

private:
    AdjacencyMap m_adj;         ///< Adjacency list representation
    size_type m_edge_count;     ///< Number of edges
    bool m_directed;            ///< Whether graph is directed

    /**
     * @brief Find neighbor in adjacency list
     * @param adj Adjacency list to search
     * @param vertex Vertex to find
     * @return Iterator to neighbor, or end() if not found
     */
    typename AdjacencyList::iterator find_neighbor(AdjacencyList& adj, const Vertex& vertex);
    typename AdjacencyList::const_iterator find_neighbor(const AdjacencyList& adj, const Vertex& vertex) const;

    /**
     * @brief DFS helper for cycle detection
     * @param vertex Current vertex
     * @param visited Visited set
     * @param rec_stack Recursion stack (for directed graphs)
     * @return true if cycle found
     */
    bool has_cycle_dfs(const Vertex& vertex,
                       std::unordered_set<Vertex>& visited,
                       std::unordered_set<Vertex>& rec_stack) const;

    /**
     * @brief DFS helper for recursive DFS
     * @param vertex Current vertex
     * @param visited Visited set
     * @param visitor Visitor function
     */
    void dfs_helper(const Vertex& vertex,
                    std::unordered_set<Vertex>& visited,
                    std::function<void(const Vertex&)>& visitor) const;

    /**
     * @brief DFS helper for topological sort
     * @param vertex Current vertex
     * @param visited Visited set
     * @param result Result stack
     */
    void topological_sort_dfs(const Vertex& vertex,
                              std::unordered_set<Vertex>& visited,
                              std::vector<Vertex>& result) const;
};

/**
 * @brief Type alias for directed graph
 */
template <typename Vertex, typename Weight = double>
using DirectedGraph = Graph<Vertex, Weight>;

/**
 * @brief Helper function to create an undirected graph
 */
template <typename Vertex, typename Weight = double>
Graph<Vertex, Weight> make_undirected_graph() {
    return Graph<Vertex, Weight>(false);
}

} // namespace graph
} // namespace mylib

#endif // MYLIB_GRAPH_GRAPH_HPP