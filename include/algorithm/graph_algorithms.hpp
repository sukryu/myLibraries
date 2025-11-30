/**
 * @file graph_algorithms.hpp
 * @brief Comprehensive graph algorithms with flexible interfaces
 * @author Jinhyeok
 * @date 2025-11-30
 * @version 1.0.0
 * 
 * This file provides various graph algorithms with:
 * - Support for both directed and undirected graphs
 * - Weighted and unweighted graph support
 * - Multiple algorithm implementations for same problem
 * - Path reconstruction capabilities
 * - Detailed result structures
 * 
 * Algorithms included:
 * - Shortest Path: Bellman-Ford, Floyd-Warshall
 * - Minimum Spanning Tree: Kruskal, Prim
 * - Utility: Union-Find (Disjoint Set)
 * 
 * Copyright (c) 2025 Jinhyeok
 * MIT License
 */

#ifndef MYLIB_ALGORITHM_GRAPH_ALGORITHMS_HPP
#define MYLIB_ALGORITHM_GRAPH_ALGORITHMS_HPP

#include <cstddef>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <limits>
#include <algorithm>
#include <functional>
#include <optional>
#include <utility>

namespace mylib {
namespace algorithm {

// ============================================
// Edge structure for algorithm input
// ============================================

/**
 * @struct Edge
 * @brief Represents a weighted edge in a graph
 */
template <typename Vertex, typename Weight = double>
struct Edge {
    Vertex from;
    Vertex to;
    Weight weight;
    
    Edge(const Vertex& f, const Vertex& t, Weight w = Weight{1})
        : from(f), to(t), weight(w) {}
    
    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
    
    bool operator>(const Edge& other) const {
        return weight > other.weight;
    }
    
    bool operator==(const Edge& other) const {
        return from == other.from && to == other.to && weight == other.weight;
    }
};

// ============================================
// Result structures
// ============================================

/**
 * @struct ShortestPathResult
 * @brief Result of single-source shortest path algorithms
 */
template <typename Vertex, typename Weight>
struct ShortestPathResult {
    bool has_negative_cycle = false;
    std::unordered_map<Vertex, Weight> distances;
    std::unordered_map<Vertex, Vertex> predecessors;
    
    /**
     * @brief Get the shortest distance to a vertex
     */
    std::optional<Weight> distance_to(const Vertex& v) const {
        auto it = distances.find(v);
        if (it == distances.end()) return std::nullopt;
        if (it->second == std::numeric_limits<Weight>::max()) return std::nullopt;
        return it->second;
    }
    
    /**
     * @brief Reconstruct path from source to target
     */
    std::vector<Vertex> path_to(const Vertex& source, const Vertex& target) const {
        if (has_negative_cycle) return {};
        
        auto dist_it = distances.find(target);
        if (dist_it == distances.end() || 
            dist_it->second == std::numeric_limits<Weight>::max()) {
            return {};
        }
        
        std::vector<Vertex> path;
        Vertex current = target;
        
        while (current != source) {
            path.push_back(current);
            auto pred_it = predecessors.find(current);
            if (pred_it == predecessors.end()) return {};
            current = pred_it->second;
        }
        path.push_back(source);
        
        std::reverse(path.begin(), path.end());
        return path;
    }
};

/**
 * @struct AllPairsShortestPathResult
 * @brief Result of all-pairs shortest path algorithms
 */
template <typename Vertex, typename Weight>
struct AllPairsShortestPathResult {
    bool has_negative_cycle = false;
    std::unordered_map<Vertex, std::unordered_map<Vertex, Weight>> distances;
    std::unordered_map<Vertex, std::unordered_map<Vertex, Vertex>> next_vertex;
    
    /**
     * @brief Get distance between two vertices
     */
    std::optional<Weight> distance(const Vertex& from, const Vertex& to) const {
        auto from_it = distances.find(from);
        if (from_it == distances.end()) return std::nullopt;
        
        auto to_it = from_it->second.find(to);
        if (to_it == from_it->second.end()) return std::nullopt;
        if (to_it->second == std::numeric_limits<Weight>::max()) return std::nullopt;
        
        return to_it->second;
    }
    
    /**
     * @brief Reconstruct path between two vertices
     */
    std::vector<Vertex> path(const Vertex& from, const Vertex& to) const {
        if (has_negative_cycle) return {};
        
        auto dist = distance(from, to);
        if (!dist.has_value()) return {};
        
        std::vector<Vertex> result;
        Vertex current = from;
        
        while (current != to) {
            result.push_back(current);
            
            auto from_it = next_vertex.find(current);
            if (from_it == next_vertex.end()) return {};
            
            auto to_it = from_it->second.find(to);
            if (to_it == from_it->second.end()) return {};
            
            current = to_it->second;
        }
        result.push_back(to);
        
        return result;
    }
};

/**
 * @struct MSTResult
 * @brief Result of minimum spanning tree algorithms
 */
template <typename Vertex, typename Weight>
struct MSTResult {
    bool exists = false;  ///< True if MST exists (graph is connected)
    Weight total_weight = Weight{0};
    std::vector<Edge<Vertex, Weight>> edges;
    
    /**
     * @brief Get number of edges in MST
     */
    std::size_t edge_count() const {
        return edges.size();
    }
    
    /**
     * @brief Check if a specific edge is in the MST
     */
    bool contains_edge(const Vertex& from, const Vertex& to) const {
        for (const auto& e : edges) {
            if ((e.from == from && e.to == to) || 
                (e.from == to && e.to == from)) {
                return true;
            }
        }
        return false;
    }
};

// ============================================
// Union-Find (Disjoint Set) Data Structure
// ============================================

/**
 * @class UnionFind
 * @brief Disjoint Set Union data structure with path compression and union by rank
 * 
 * Time Complexity:
 * - find(): O(α(n)) amortized (nearly constant)
 * - unite(): O(α(n)) amortized
 * - connected(): O(α(n)) amortized
 * 
 * Space Complexity: O(n)
 */
template <typename T>
class UnionFind {
public:
    UnionFind() = default;
    
    /**
     * @brief Initialize with a collection of elements
     */
    template <typename InputIt>
    UnionFind(InputIt first, InputIt last) {
        for (auto it = first; it != last; ++it) {
            make_set(*it);
        }
    }
    
    /**
     * @brief Initialize with initializer list
     */
    UnionFind(std::initializer_list<T> init) {
        for (const auto& elem : init) {
            make_set(elem);
        }
    }
    
    /**
     * @brief Create a new set containing only element x
     */
    void make_set(const T& x) {
        if (m_parent.find(x) == m_parent.end()) {
            m_parent[x] = x;
            m_rank[x] = 0;
            ++m_set_count;
        }
    }
    
    /**
     * @brief Find the representative (root) of the set containing x
     * Uses path compression for efficiency
     */
    T find(const T& x) {
        auto it = m_parent.find(x);
        if (it == m_parent.end()) {
            make_set(x);
            return x;
        }
        
        if (it->second != x) {
            it->second = find(it->second);  // Path compression
        }
        return it->second;
    }
    
    /**
     * @brief Unite the sets containing x and y
     * Uses union by rank for efficiency
     * @return true if sets were different and merged, false if already same set
     */
    bool unite(const T& x, const T& y) {
        T root_x = find(x);
        T root_y = find(y);
        
        if (root_x == root_y) {
            return false;  // Already in same set
        }
        
        // Union by rank
        if (m_rank[root_x] < m_rank[root_y]) {
            m_parent[root_x] = root_y;
        } else if (m_rank[root_x] > m_rank[root_y]) {
            m_parent[root_y] = root_x;
        } else {
            m_parent[root_y] = root_x;
            ++m_rank[root_x];
        }
        
        --m_set_count;
        return true;
    }
    
    /**
     * @brief Check if x and y are in the same set
     */
    bool connected(const T& x, const T& y) {
        return find(x) == find(y);
    }
    
    /**
     * @brief Get the number of disjoint sets
     */
    std::size_t set_count() const {
        return m_set_count;
    }
    
    /**
     * @brief Get the total number of elements
     */
    std::size_t size() const {
        return m_parent.size();
    }
    
    /**
     * @brief Get all elements in the same set as x
     */
    std::vector<T> get_set(const T& x) {
        T root = find(x);
        std::vector<T> result;
        
        for (auto& pair : m_parent) {
            if (find(pair.first) == root) {
                result.push_back(pair.first);
            }
        }
        
        return result;
    }
    
    /**
     * @brief Get all sets as a vector of vectors
     */
    std::vector<std::vector<T>> get_all_sets() {
        std::unordered_map<T, std::vector<T>> sets;
        
        for (auto& pair : m_parent) {
            T root = find(pair.first);
            sets[root].push_back(pair.first);
        }
        
        std::vector<std::vector<T>> result;
        for (auto& pair : sets) {
            result.push_back(std::move(pair.second));
        }
        
        return result;
    }
    
    /**
     * @brief Clear all elements
     */
    void clear() {
        m_parent.clear();
        m_rank.clear();
        m_set_count = 0;
    }

private:
    std::unordered_map<T, T> m_parent;
    std::unordered_map<T, std::size_t> m_rank;
    std::size_t m_set_count = 0;
};

// ============================================
// Bellman-Ford Algorithm
// ============================================

/**
 * @class BellmanFord
 * @brief Single-source shortest path algorithm that handles negative weights
 * 
 * Time Complexity: O(V * E)
 * Space Complexity: O(V)
 * 
 * Advantages over Dijkstra:
 * - Can handle negative edge weights
 * - Can detect negative cycles
 * 
 * Usage:
 * @code
 * std::vector<Edge<int, double>> edges = {{0, 1, 4}, {1, 2, -2}, {0, 2, 5}};
 * auto result = BellmanFord<int, double>::run(edges, 0, 3);
 * if (!result.has_negative_cycle) {
 *     auto dist = result.distance_to(2);  // Optional<double>
 *     auto path = result.path_to(0, 2);   // vector<int>
 * }
 * @endcode
 */
template <typename Vertex, typename Weight = double>
class BellmanFord {
public:
    using EdgeType = Edge<Vertex, Weight>;
    using ResultType = ShortestPathResult<Vertex, Weight>;
    
    /**
     * @brief Run Bellman-Ford from a source vertex
     * @param edges List of all edges in the graph
     * @param source Starting vertex
     * @param vertex_count Number of vertices (for optimization)
     * @return ShortestPathResult containing distances and predecessors
     */
    static ResultType run(const std::vector<EdgeType>& edges, 
                          const Vertex& source,
                          std::size_t vertex_count = 0) {
        ResultType result;
        const Weight INF = std::numeric_limits<Weight>::max();
        
        // Collect all vertices
        std::unordered_set<Vertex> vertices;
        vertices.insert(source);
        for (const auto& edge : edges) {
            vertices.insert(edge.from);
            vertices.insert(edge.to);
        }
        
        if (vertex_count == 0) {
            vertex_count = vertices.size();
        }
        
        // Initialize distances
        for (const auto& v : vertices) {
            result.distances[v] = INF;
        }
        result.distances[source] = Weight{0};
        
        // Relax edges V-1 times
        for (std::size_t i = 0; i < vertex_count - 1; ++i) {
            bool updated = false;
            
            for (const auto& edge : edges) {
                if (result.distances[edge.from] != INF) {
                    Weight new_dist = result.distances[edge.from] + edge.weight;
                    if (new_dist < result.distances[edge.to]) {
                        result.distances[edge.to] = new_dist;
                        result.predecessors[edge.to] = edge.from;
                        updated = true;
                    }
                }
            }
            
            // Early termination if no updates
            if (!updated) break;
        }
        
        // Check for negative cycles
        for (const auto& edge : edges) {
            if (result.distances[edge.from] != INF) {
                Weight new_dist = result.distances[edge.from] + edge.weight;
                if (new_dist < result.distances[edge.to]) {
                    result.has_negative_cycle = true;
                    break;
                }
            }
        }
        
        return result;
    }
    
    /**
     * @brief Run Bellman-Ford from adjacency list representation
     */
    template <typename AdjList>
    static ResultType run_from_adj_list(const AdjList& adj, const Vertex& source) {
        std::vector<EdgeType> edges;
        
        for (const auto& [vertex, neighbors] : adj) {
            for (const auto& neighbor : neighbors) {
                edges.emplace_back(vertex, neighbor.vertex, neighbor.weight);
            }
        }
        
        return run(edges, source);
    }
};

// ============================================
// Floyd-Warshall Algorithm
// ============================================

/**
 * @class FloydWarshall
 * @brief All-pairs shortest path algorithm
 * 
 * Time Complexity: O(V³)
 * Space Complexity: O(V²)
 * 
 * Advantages:
 * - Finds shortest paths between ALL pairs of vertices
 * - Can handle negative weights (but not negative cycles)
 * - Simple implementation
 * 
 * Usage:
 * @code
 * std::vector<Edge<int, double>> edges = {{0, 1, 3}, {1, 2, 1}, {0, 2, 6}};
 * auto result = FloydWarshall<int, double>::run(edges);
 * auto dist = result.distance(0, 2);  // Optional<double>
 * auto path = result.path(0, 2);      // vector<int>
 * @endcode
 */
template <typename Vertex, typename Weight = double>
class FloydWarshall {
public:
    using EdgeType = Edge<Vertex, Weight>;
    using ResultType = AllPairsShortestPathResult<Vertex, Weight>;
    
    /**
     * @brief Run Floyd-Warshall algorithm
     * @param edges List of all edges in the graph
     * @param directed Whether the graph is directed
     * @return AllPairsShortestPathResult containing all distances and paths
     */
    static ResultType run(const std::vector<EdgeType>& edges, bool directed = true) {
        ResultType result;
        const Weight INF = std::numeric_limits<Weight>::max();
        
        // Collect all vertices
        std::vector<Vertex> vertices;
        std::unordered_set<Vertex> vertex_set;
        
        for (const auto& edge : edges) {
            if (vertex_set.insert(edge.from).second) {
                vertices.push_back(edge.from);
            }
            if (vertex_set.insert(edge.to).second) {
                vertices.push_back(edge.to);
            }
        }
        
        // Initialize distances
        for (const auto& i : vertices) {
            for (const auto& j : vertices) {
                if (i == j) {
                    result.distances[i][j] = Weight{0};
                    result.next_vertex[i][j] = j;
                } else {
                    result.distances[i][j] = INF;
                }
            }
        }
        
        // Set direct edge weights
        for (const auto& edge : edges) {
            if (edge.weight < result.distances[edge.from][edge.to]) {
                result.distances[edge.from][edge.to] = edge.weight;
                result.next_vertex[edge.from][edge.to] = edge.to;
            }
            
            if (!directed) {
                if (edge.weight < result.distances[edge.to][edge.from]) {
                    result.distances[edge.to][edge.from] = edge.weight;
                    result.next_vertex[edge.to][edge.from] = edge.from;
                }
            }
        }
        
        // Floyd-Warshall main loop
        for (const auto& k : vertices) {
            for (const auto& i : vertices) {
                for (const auto& j : vertices) {
                    if (result.distances[i][k] != INF && 
                        result.distances[k][j] != INF) {
                        Weight new_dist = result.distances[i][k] + result.distances[k][j];
                        if (new_dist < result.distances[i][j]) {
                            result.distances[i][j] = new_dist;
                            result.next_vertex[i][j] = result.next_vertex[i][k];
                        }
                    }
                }
            }
        }
        
        // Check for negative cycles (diagonal should be 0)
        for (const auto& v : vertices) {
            if (result.distances[v][v] < Weight{0}) {
                result.has_negative_cycle = true;
                break;
            }
        }
        
        return result;
    }
    
    /**
     * @brief Run Floyd-Warshall from adjacency list representation
     */
    template <typename AdjList>
    static ResultType run_from_adj_list(const AdjList& adj, bool directed = true) {
        std::vector<EdgeType> edges;
        
        for (const auto& [vertex, neighbors] : adj) {
            for (const auto& neighbor : neighbors) {
                edges.emplace_back(vertex, neighbor.vertex, neighbor.weight);
            }
        }
        
        return run(edges, directed);
    }
};

// ============================================
// Kruskal's Algorithm
// ============================================

/**
 * @class Kruskal
 * @brief Minimum Spanning Tree using Kruskal's algorithm
 * 
 * Time Complexity: O(E log E) or O(E log V)
 * Space Complexity: O(V)
 * 
 * Strategy: Greedy edge selection with Union-Find
 * - Sort edges by weight
 * - Add edges that don't create cycles
 * 
 * Usage:
 * @code
 * std::vector<Edge<int, double>> edges = {{0, 1, 4}, {0, 2, 3}, {1, 2, 1}};
 * auto result = Kruskal<int, double>::run(edges);
 * if (result.exists) {
 *     std::cout << "MST weight: " << result.total_weight << std::endl;
 *     for (const auto& e : result.edges) {
 *         std::cout << e.from << " - " << e.to << ": " << e.weight << std::endl;
 *     }
 * }
 * @endcode
 */
template <typename Vertex, typename Weight = double>
class Kruskal {
public:
    using EdgeType = Edge<Vertex, Weight>;
    using ResultType = MSTResult<Vertex, Weight>;
    
    /**
     * @brief Run Kruskal's algorithm
     * @param edges List of all edges (undirected graph assumed)
     * @return MSTResult containing MST edges and total weight
     */
    static ResultType run(std::vector<EdgeType> edges) {
        ResultType result;
        
        if (edges.empty()) {
            result.exists = true;
            return result;
        }
        
        // Get all vertices
        std::unordered_set<Vertex> vertex_set;
        for (const auto& edge : edges) {
            vertex_set.insert(edge.from);
            vertex_set.insert(edge.to);
        }
        
        std::size_t vertex_count = vertex_set.size();
        
        // Initialize Union-Find
        UnionFind<Vertex> uf(vertex_set.begin(), vertex_set.end());
        
        // Sort edges by weight
        std::sort(edges.begin(), edges.end());
        
        // Kruskal's algorithm
        for (const auto& edge : edges) {
            if (!uf.connected(edge.from, edge.to)) {
                uf.unite(edge.from, edge.to);
                result.edges.push_back(edge);
                result.total_weight += edge.weight;
                
                // MST has V-1 edges
                if (result.edges.size() == vertex_count - 1) {
                    break;
                }
            }
        }
        
        // Check if MST is complete (graph is connected)
        result.exists = (result.edges.size() == vertex_count - 1);
        
        return result;
    }
    
    /**
     * @brief Run Kruskal's from adjacency list representation
     */
    template <typename AdjList>
    static ResultType run_from_adj_list(const AdjList& adj) {
        std::vector<EdgeType> edges;
        std::unordered_set<std::size_t> seen_edges;
        
        for (const auto& [vertex, neighbors] : adj) {
            for (const auto& neighbor : neighbors) {
                // Avoid duplicate edges for undirected graph
                std::size_t edge_hash = std::hash<Vertex>{}(vertex) ^ 
                                       (std::hash<Vertex>{}(neighbor.vertex) << 1);
                std::size_t reverse_hash = std::hash<Vertex>{}(neighbor.vertex) ^ 
                                          (std::hash<Vertex>{}(vertex) << 1);
                
                if (seen_edges.find(edge_hash) == seen_edges.end() &&
                    seen_edges.find(reverse_hash) == seen_edges.end()) {
                    edges.emplace_back(vertex, neighbor.vertex, neighbor.weight);
                    seen_edges.insert(edge_hash);
                }
            }
        }
        
        return run(std::move(edges));
    }
};

// ============================================
// Prim's Algorithm
// ============================================

/**
 * @class Prim
 * @brief Minimum Spanning Tree using Prim's algorithm
 * 
 * Time Complexity: O(E log V) with binary heap
 * Space Complexity: O(V)
 * 
 * Strategy: Grow MST from starting vertex
 * - Start from arbitrary vertex
 * - Always add minimum weight edge connecting MST to non-MST vertex
 * 
 * Usage:
 * @code
 * std::vector<Edge<int, double>> edges = {{0, 1, 4}, {0, 2, 3}, {1, 2, 1}};
 * auto result = Prim<int, double>::run(edges, 0);
 * @endcode
 */
template <typename Vertex, typename Weight = double>
class Prim {
public:
    using EdgeType = Edge<Vertex, Weight>;
    using ResultType = MSTResult<Vertex, Weight>;
    
    /**
     * @brief Run Prim's algorithm
     * @param edges List of all edges (undirected graph assumed)
     * @param start Starting vertex (optional, uses first vertex if not provided)
     * @return MSTResult containing MST edges and total weight
     */
    static ResultType run(const std::vector<EdgeType>& edges,
                          std::optional<Vertex> start = std::nullopt) {
        ResultType result;
        
        if (edges.empty()) {
            result.exists = true;
            return result;
        }
        
        // Build adjacency list
        std::unordered_map<Vertex, std::vector<std::pair<Vertex, Weight>>> adj;
        std::unordered_set<Vertex> vertex_set;
        
        for (const auto& edge : edges) {
            adj[edge.from].emplace_back(edge.to, edge.weight);
            adj[edge.to].emplace_back(edge.from, edge.weight);
            vertex_set.insert(edge.from);
            vertex_set.insert(edge.to);
        }
        
        std::size_t vertex_count = vertex_set.size();
        
        // Determine starting vertex
        Vertex start_vertex = start.value_or(edges[0].from);
        
        // Priority queue: (weight, from, to)
        using PQElement = std::tuple<Weight, Vertex, Vertex>;
        std::priority_queue<PQElement, std::vector<PQElement>, std::greater<PQElement>> pq;
        
        std::unordered_set<Vertex> in_mst;
        in_mst.insert(start_vertex);
        
        // Add edges from starting vertex
        for (const auto& [neighbor, weight] : adj[start_vertex]) {
            pq.emplace(weight, start_vertex, neighbor);
        }
        
        // Prim's algorithm
        while (!pq.empty() && in_mst.size() < vertex_count) {
            auto [weight, from, to] = pq.top();
            pq.pop();
            
            if (in_mst.count(to)) {
                continue;  // Already in MST
            }
            
            // Add edge to MST
            in_mst.insert(to);
            result.edges.emplace_back(from, to, weight);
            result.total_weight += weight;
            
            // Add new edges from newly added vertex
            for (const auto& [neighbor, w] : adj[to]) {
                if (!in_mst.count(neighbor)) {
                    pq.emplace(w, to, neighbor);
                }
            }
        }
        
        // Check if MST is complete
        result.exists = (in_mst.size() == vertex_count);
        
        return result;
    }
    
    /**
     * @brief Run Prim's from adjacency list representation
     */
    template <typename AdjList>
    static ResultType run_from_adj_list(const AdjList& adj,
                                        std::optional<Vertex> start = std::nullopt) {
        std::vector<EdgeType> edges;
        std::unordered_set<std::size_t> seen_edges;
        
        for (const auto& [vertex, neighbors] : adj) {
            for (const auto& neighbor : neighbors) {
                std::size_t edge_hash = std::hash<Vertex>{}(vertex) ^ 
                                       (std::hash<Vertex>{}(neighbor.vertex) << 1);
                std::size_t reverse_hash = std::hash<Vertex>{}(neighbor.vertex) ^ 
                                          (std::hash<Vertex>{}(vertex) << 1);
                
                if (seen_edges.find(edge_hash) == seen_edges.end() &&
                    seen_edges.find(reverse_hash) == seen_edges.end()) {
                    edges.emplace_back(vertex, neighbor.vertex, neighbor.weight);
                    seen_edges.insert(edge_hash);
                }
            }
        }
        
        return run(edges, start);
    }
};

// ============================================
// Convenience free functions
// ============================================

/**
 * @brief Run Bellman-Ford algorithm
 */
template <typename Vertex, typename Weight>
ShortestPathResult<Vertex, Weight> bellman_ford(
    const std::vector<Edge<Vertex, Weight>>& edges,
    const Vertex& source,
    std::size_t vertex_count = 0) {
    return BellmanFord<Vertex, Weight>::run(edges, source, vertex_count);
}

/**
 * @brief Run Floyd-Warshall algorithm
 */
template <typename Vertex, typename Weight>
AllPairsShortestPathResult<Vertex, Weight> floyd_warshall(
    const std::vector<Edge<Vertex, Weight>>& edges,
    bool directed = true) {
    return FloydWarshall<Vertex, Weight>::run(edges, directed);
}

/**
 * @brief Run Kruskal's algorithm
 */
template <typename Vertex, typename Weight>
MSTResult<Vertex, Weight> kruskal(std::vector<Edge<Vertex, Weight>> edges) {
    return Kruskal<Vertex, Weight>::run(std::move(edges));
}

/**
 * @brief Run Prim's algorithm
 */
template <typename Vertex, typename Weight>
MSTResult<Vertex, Weight> prim(
    const std::vector<Edge<Vertex, Weight>>& edges,
    std::optional<Vertex> start = std::nullopt) {
    return Prim<Vertex, Weight>::run(edges, start);
}

} // namespace algorithm
} // namespace mylib

#endif // MYLIB_ALGORITHM_GRAPH_ALGORITHMS_HPP