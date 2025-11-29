/**
 * @file graph.cpp
 * @brief Implementation of Graph class
 * @author Jinhyeok
 * @date 2025-11-29
 * @version 1.0.0
 */

#include "graph/graph.hpp"

namespace mylib {
namespace graph {

// Constructors
template <typename Vertex, typename Weight>
Graph<Vertex, Weight>::Graph()
    : m_adj(), m_edge_count(0), m_directed(true) {
}

template <typename Vertex, typename Weight>
Graph<Vertex, Weight>::Graph(bool directed)
    : m_adj(), m_edge_count(0), m_directed(directed) {
}

template <typename Vertex, typename Weight>
Graph<Vertex, Weight>::Graph(std::initializer_list<Vertex> vertices, bool directed)
    : m_adj(), m_edge_count(0), m_directed(directed) {
    for (const auto& v : vertices) {
        add_vertex(v);
    }
}

template <typename Vertex, typename Weight>
Graph<Vertex, Weight>::Graph(const Graph& other)
    : m_adj(other.m_adj)
    , m_edge_count(other.m_edge_count)
    , m_directed(other.m_directed) {
}

template <typename Vertex, typename Weight>
Graph<Vertex, Weight>::Graph(Graph&& other) noexcept
    : m_adj(std::move(other.m_adj))
    , m_edge_count(other.m_edge_count)
    , m_directed(other.m_directed) {
    other.m_edge_count = 0;
}

// Assignment operators
template <typename Vertex, typename Weight>
Graph<Vertex, Weight>& Graph<Vertex, Weight>::operator=(const Graph& other) {
    if (this != &other) {
        m_adj = other.m_adj;
        m_edge_count = other.m_edge_count;
        m_directed = other.m_directed;
    }
    return *this;
}

template <typename Vertex, typename Weight>
Graph<Vertex, Weight>& Graph<Vertex, Weight>::operator=(Graph&& other) noexcept {
    if (this != &other) {
        m_adj = std::move(other.m_adj);
        m_edge_count = other.m_edge_count;
        m_directed = other.m_directed;
        other.m_edge_count = 0;
    }
    return *this;
}

// Properties
template <typename Vertex, typename Weight>
bool Graph<Vertex, Weight>::is_directed() const noexcept {
    return m_directed;
}

template <typename Vertex, typename Weight>
bool Graph<Vertex, Weight>::empty() const noexcept {
    return m_adj.empty();
}

template <typename Vertex, typename Weight>
typename Graph<Vertex, Weight>::size_type 
Graph<Vertex, Weight>::vertex_count() const noexcept {
    return m_adj.size();
}

template <typename Vertex, typename Weight>
typename Graph<Vertex, Weight>::size_type 
Graph<Vertex, Weight>::edge_count() const noexcept {
    return m_edge_count;
}

// Vertex operations
template <typename Vertex, typename Weight>
bool Graph<Vertex, Weight>::add_vertex(const Vertex& vertex) {
    if (has_vertex(vertex)) {
        return false;
    }
    m_adj[vertex] = AdjacencyList();
    return true;
}

template <typename Vertex, typename Weight>
bool Graph<Vertex, Weight>::remove_vertex(const Vertex& vertex) {
    auto it = m_adj.find(vertex);
    if (it == m_adj.end()) {
        return false;
    }
    
    // Count edges to be removed
    size_type edges_removed = it->second.size();
    
    // Remove all edges pointing to this vertex
    for (auto& pair : m_adj) {
        if (pair.first != vertex) {
            auto neighbor_it = find_neighbor(pair.second, vertex);
            if (neighbor_it != pair.second.end()) {
                pair.second.erase(neighbor_it);
                if (m_directed) {
                    ++edges_removed;
                }
            }
        }
    }
    
    // Remove the vertex
    m_adj.erase(it);
    
    if (m_directed) {
        m_edge_count -= edges_removed;
    } else {
        m_edge_count -= edges_removed;  // Each edge counted once
    }
    
    return true;
}

template <typename Vertex, typename Weight>
bool Graph<Vertex, Weight>::has_vertex(const Vertex& vertex) const {
    return m_adj.find(vertex) != m_adj.end();
}

template <typename Vertex, typename Weight>
std::vector<Vertex> Graph<Vertex, Weight>::vertices() const {
    std::vector<Vertex> result;
    result.reserve(m_adj.size());
    for (const auto& pair : m_adj) {
        result.push_back(pair.first);
    }
    return result;
}

template <typename Vertex, typename Weight>
typename Graph<Vertex, Weight>::size_type 
Graph<Vertex, Weight>::degree(const Vertex& vertex) const {
    auto it = m_adj.find(vertex);
    if (it == m_adj.end()) {
        throw std::out_of_range("Graph::degree: vertex not found");
    }
    
    if (m_directed) {
        return in_degree(vertex) + out_degree(vertex);
    }
    return it->second.size();
}

template <typename Vertex, typename Weight>
typename Graph<Vertex, Weight>::size_type 
Graph<Vertex, Weight>::in_degree(const Vertex& vertex) const {
    if (!has_vertex(vertex)) {
        throw std::out_of_range("Graph::in_degree: vertex not found");
    }
    
    if (!m_directed) {
        return m_adj.at(vertex).size();
    }
    
    size_type count = 0;
    for (const auto& pair : m_adj) {
        if (find_neighbor(pair.second, vertex) != pair.second.end()) {
            ++count;
        }
    }
    return count;
}

template <typename Vertex, typename Weight>
typename Graph<Vertex, Weight>::size_type 
Graph<Vertex, Weight>::out_degree(const Vertex& vertex) const {
    auto it = m_adj.find(vertex);
    if (it == m_adj.end()) {
        throw std::out_of_range("Graph::out_degree: vertex not found");
    }
    return it->second.size();
}

// Edge operations
template <typename Vertex, typename Weight>
bool Graph<Vertex, Weight>::add_edge(const Vertex& from, const Vertex& to, Weight weight) {
    // Add vertices if they don't exist
    add_vertex(from);
    add_vertex(to);
    
    // Check if edge already exists
    if (has_edge(from, to)) {
        return false;
    }
    
    // Add edge
    m_adj[from].emplace_back(to, weight);
    ++m_edge_count;
    
    // For undirected graph, add reverse edge
    if (!m_directed && from != to) {
        m_adj[to].emplace_back(from, weight);
    }
    
    return true;
}

template <typename Vertex, typename Weight>
bool Graph<Vertex, Weight>::remove_edge(const Vertex& from, const Vertex& to) {
    auto from_it = m_adj.find(from);
    if (from_it == m_adj.end()) {
        return false;
    }
    
    auto neighbor_it = find_neighbor(from_it->second, to);
    if (neighbor_it == from_it->second.end()) {
        return false;
    }
    
    from_it->second.erase(neighbor_it);
    --m_edge_count;
    
    // For undirected graph, remove reverse edge
    if (!m_directed && from != to) {
        auto to_it = m_adj.find(to);
        if (to_it != m_adj.end()) {
            auto rev_neighbor_it = find_neighbor(to_it->second, from);
            if (rev_neighbor_it != to_it->second.end()) {
                to_it->second.erase(rev_neighbor_it);
            }
        }
    }
    
    return true;
}

template <typename Vertex, typename Weight>
bool Graph<Vertex, Weight>::has_edge(const Vertex& from, const Vertex& to) const {
    auto it = m_adj.find(from);
    if (it == m_adj.end()) {
        return false;
    }
    return find_neighbor(it->second, to) != it->second.end();
}

template <typename Vertex, typename Weight>
Weight Graph<Vertex, Weight>::get_weight(const Vertex& from, const Vertex& to) const {
    auto it = m_adj.find(from);
    if (it == m_adj.end()) {
        throw std::out_of_range("Graph::get_weight: edge not found");
    }
    
    auto neighbor_it = find_neighbor(it->second, to);
    if (neighbor_it == it->second.end()) {
        throw std::out_of_range("Graph::get_weight: edge not found");
    }
    
    return neighbor_it->weight;
}

template <typename Vertex, typename Weight>
bool Graph<Vertex, Weight>::set_weight(const Vertex& from, const Vertex& to, Weight weight) {
    auto it = m_adj.find(from);
    if (it == m_adj.end()) {
        return false;
    }
    
    auto neighbor_it = find_neighbor(it->second, to);
    if (neighbor_it == it->second.end()) {
        return false;
    }
    
    neighbor_it->weight = weight;
    
    // For undirected graph, update reverse edge too
    if (!m_directed && from != to) {
        auto to_it = m_adj.find(to);
        if (to_it != m_adj.end()) {
            auto rev_it = find_neighbor(to_it->second, from);
            if (rev_it != to_it->second.end()) {
                rev_it->weight = weight;
            }
        }
    }
    
    return true;
}

template <typename Vertex, typename Weight>
std::vector<typename Graph<Vertex, Weight>::Edge> 
Graph<Vertex, Weight>::edges() const {
    std::vector<Edge> result;
    
    if (m_directed) {
        for (const auto& pair : m_adj) {
            for (const auto& neighbor : pair.second) {
                result.emplace_back(pair.first, neighbor.vertex, neighbor.weight);
            }
        }
    } else {
        std::unordered_set<Vertex> visited;
        for (const auto& pair : m_adj) {
            for (const auto& neighbor : pair.second) {
                if (visited.find(neighbor.vertex) == visited.end()) {
                    result.emplace_back(pair.first, neighbor.vertex, neighbor.weight);
                }
            }
            visited.insert(pair.first);
        }
    }
    
    return result;
}

template <typename Vertex, typename Weight>
std::vector<Vertex> Graph<Vertex, Weight>::neighbors(const Vertex& vertex) const {
    auto it = m_adj.find(vertex);
    if (it == m_adj.end()) {
        throw std::out_of_range("Graph::neighbors: vertex not found");
    }
    
    std::vector<Vertex> result;
    result.reserve(it->second.size());
    for (const auto& neighbor : it->second) {
        result.push_back(neighbor.vertex);
    }
    return result;
}

template <typename Vertex, typename Weight>
std::vector<std::pair<Vertex, Weight>> 
Graph<Vertex, Weight>::neighbors_with_weights(const Vertex& vertex) const {
    auto it = m_adj.find(vertex);
    if (it == m_adj.end()) {
        throw std::out_of_range("Graph::neighbors_with_weights: vertex not found");
    }
    
    std::vector<std::pair<Vertex, Weight>> result;
    result.reserve(it->second.size());
    for (const auto& neighbor : it->second) {
        result.emplace_back(neighbor.vertex, neighbor.weight);
    }
    return result;
}

// Traversals
template <typename Vertex, typename Weight>
void Graph<Vertex, Weight>::bfs(const Vertex& start, 
                                 std::function<void(const Vertex&)> visitor) const {
    if (!has_vertex(start)) {
        return;
    }
    
    std::unordered_set<Vertex> visited;
    std::queue<Vertex> queue;
    
    visited.insert(start);
    queue.push(start);
    
    while (!queue.empty()) {
        Vertex current = queue.front();
        queue.pop();
        
        visitor(current);
        
        auto it = m_adj.find(current);
        if (it != m_adj.end()) {
            for (const auto& neighbor : it->second) {
                if (visited.find(neighbor.vertex) == visited.end()) {
                    visited.insert(neighbor.vertex);
                    queue.push(neighbor.vertex);
                }
            }
        }
    }
}

template <typename Vertex, typename Weight>
void Graph<Vertex, Weight>::dfs(const Vertex& start, 
                                 std::function<void(const Vertex&)> visitor) const {
    if (!has_vertex(start)) {
        return;
    }
    
    std::unordered_set<Vertex> visited;
    std::stack<Vertex> stack;
    
    stack.push(start);
    
    while (!stack.empty()) {
        Vertex current = stack.top();
        stack.pop();
        
        if (visited.find(current) != visited.end()) {
            continue;
        }
        
        visited.insert(current);
        visitor(current);
        
        auto it = m_adj.find(current);
        if (it != m_adj.end()) {
            for (const auto& neighbor : it->second) {
                if (visited.find(neighbor.vertex) == visited.end()) {
                    stack.push(neighbor.vertex);
                }
            }
        }
    }
}

template <typename Vertex, typename Weight>
void Graph<Vertex, Weight>::dfs_recursive(const Vertex& start, 
                                           std::function<void(const Vertex&)> visitor) const {
    if (!has_vertex(start)) {
        return;
    }
    
    std::unordered_set<Vertex> visited;
    dfs_helper(start, visited, visitor);
}

// Path finding
template <typename Vertex, typename Weight>
std::vector<Vertex> Graph<Vertex, Weight>::shortest_path_bfs(
    const Vertex& from, const Vertex& to) const {
    
    if (!has_vertex(from) || !has_vertex(to)) {
        return {};
    }
    
    if (from == to) {
        return {from};
    }
    
    std::unordered_map<Vertex, Vertex> parent;
    std::unordered_set<Vertex> visited;
    std::queue<Vertex> queue;
    
    visited.insert(from);
    queue.push(from);
    
    bool found = false;
    
    while (!queue.empty() && !found) {
        Vertex current = queue.front();
        queue.pop();
        
        auto it = m_adj.find(current);
        if (it != m_adj.end()) {
            for (const auto& neighbor : it->second) {
                if (visited.find(neighbor.vertex) == visited.end()) {
                    visited.insert(neighbor.vertex);
                    parent[neighbor.vertex] = current;
                    queue.push(neighbor.vertex);
                    
                    if (neighbor.vertex == to) {
                        found = true;
                        break;
                    }
                }
            }
        }
    }
    
    if (!found) {
        return {};
    }
    
    // Reconstruct path
    std::vector<Vertex> path;
    Vertex current = to;
    while (current != from) {
        path.push_back(current);
        current = parent[current];
    }
    path.push_back(from);
    
    std::reverse(path.begin(), path.end());
    return path;
}

template <typename Vertex, typename Weight>
std::pair<std::vector<Vertex>, Weight> Graph<Vertex, Weight>::dijkstra(
    const Vertex& from, const Vertex& to) const {
    
    if (!has_vertex(from) || !has_vertex(to)) {
        return {{}, Weight{}};
    }
    
    if (from == to) {
        return {{from}, Weight{0}};
    }
    
    const Weight INF = std::numeric_limits<Weight>::max();
    
    std::unordered_map<Vertex, Weight> dist;
    std::unordered_map<Vertex, Vertex> parent;
    
    // Initialize distances
    for (const auto& pair : m_adj) {
        dist[pair.first] = INF;
    }
    dist[from] = Weight{0};
    
    // Priority queue: (distance, vertex)
    using PQElement = std::pair<Weight, Vertex>;
    std::priority_queue<PQElement, std::vector<PQElement>, std::greater<PQElement>> pq;
    pq.emplace(Weight{0}, from);
    
    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        
        if (d > dist[u]) {
            continue;  // Outdated entry
        }
        
        if (u == to) {
            break;  // Found shortest path to destination
        }
        
        auto it = m_adj.find(u);
        if (it != m_adj.end()) {
            for (const auto& neighbor : it->second) {
                Weight new_dist = dist[u] + neighbor.weight;
                if (new_dist < dist[neighbor.vertex]) {
                    dist[neighbor.vertex] = new_dist;
                    parent[neighbor.vertex] = u;
                    pq.emplace(new_dist, neighbor.vertex);
                }
            }
        }
    }
    
    if (dist[to] == INF) {
        return {{}, Weight{}};
    }
    
    // Reconstruct path
    std::vector<Vertex> path;
    Vertex current = to;
    while (current != from) {
        path.push_back(current);
        current = parent[current];
    }
    path.push_back(from);
    
    std::reverse(path.begin(), path.end());
    return {path, dist[to]};
}

template <typename Vertex, typename Weight>
std::unordered_map<Vertex, Weight> Graph<Vertex, Weight>::dijkstra_all(
    const Vertex& from) const {
    
    const Weight INF = std::numeric_limits<Weight>::max();
    
    std::unordered_map<Vertex, Weight> dist;
    
    if (!has_vertex(from)) {
        return dist;
    }
    
    // Initialize distances
    for (const auto& pair : m_adj) {
        dist[pair.first] = INF;
    }
    dist[from] = Weight{0};
    
    using PQElement = std::pair<Weight, Vertex>;
    std::priority_queue<PQElement, std::vector<PQElement>, std::greater<PQElement>> pq;
    pq.emplace(Weight{0}, from);
    
    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        
        if (d > dist[u]) {
            continue;
        }
        
        auto it = m_adj.find(u);
        if (it != m_adj.end()) {
            for (const auto& neighbor : it->second) {
                Weight new_dist = dist[u] + neighbor.weight;
                if (new_dist < dist[neighbor.vertex]) {
                    dist[neighbor.vertex] = new_dist;
                    pq.emplace(new_dist, neighbor.vertex);
                }
            }
        }
    }
    
    return dist;
}

template <typename Vertex, typename Weight>
bool Graph<Vertex, Weight>::has_path(const Vertex& from, const Vertex& to) const {
    if (!has_vertex(from) || !has_vertex(to)) {
        return false;
    }
    
    if (from == to) {
        return true;
    }
    
    std::unordered_set<Vertex> visited;
    std::queue<Vertex> queue;
    
    visited.insert(from);
    queue.push(from);
    
    while (!queue.empty()) {
        Vertex current = queue.front();
        queue.pop();
        
        auto it = m_adj.find(current);
        if (it != m_adj.end()) {
            for (const auto& neighbor : it->second) {
                if (neighbor.vertex == to) {
                    return true;
                }
                if (visited.find(neighbor.vertex) == visited.end()) {
                    visited.insert(neighbor.vertex);
                    queue.push(neighbor.vertex);
                }
            }
        }
    }
    
    return false;
}

// Graph properties
template <typename Vertex, typename Weight>
bool Graph<Vertex, Weight>::has_cycle() const {
    if (m_adj.empty()) {
        return false;
    }
    
    std::unordered_set<Vertex> visited;
    
    if (m_directed) {
        // For directed graph, use rec_stack approach
        std::unordered_set<Vertex> rec_stack;
        for (const auto& pair : m_adj) {
            if (visited.find(pair.first) == visited.end()) {
                if (has_cycle_dfs(pair.first, visited, rec_stack)) {
                    return true;
                }
            }
        }
    } else {
        // For undirected graph, use parent tracking via map
        std::unordered_map<Vertex, Vertex> parent;
        for (const auto& pair : m_adj) {
            if (visited.find(pair.first) == visited.end()) {
                // BFS-based cycle detection for undirected graph
                std::queue<Vertex> queue;
                queue.push(pair.first);
                visited.insert(pair.first);
                parent[pair.first] = pair.first;  // Parent of start is itself
                
                while (!queue.empty()) {
                    Vertex current = queue.front();
                    queue.pop();
                    
                    auto it = m_adj.find(current);
                    if (it != m_adj.end()) {
                        for (const auto& neighbor : it->second) {
                            if (visited.find(neighbor.vertex) == visited.end()) {
                                visited.insert(neighbor.vertex);
                                parent[neighbor.vertex] = current;
                                queue.push(neighbor.vertex);
                            } else if (neighbor.vertex != parent[current]) {
                                // Found a visited vertex that is not parent -> cycle
                                return true;
                            }
                        }
                    }
                }
            }
        }
    }
    
    return false;
}

template <typename Vertex, typename Weight>
bool Graph<Vertex, Weight>::is_connected() const {
    if (m_adj.empty()) {
        return true;
    }
    
    // For undirected graph, simple BFS from any vertex
    // For directed graph, check weak connectivity (treat as undirected)
    std::unordered_set<Vertex> visited;
    std::queue<Vertex> queue;
    
    const Vertex& start = m_adj.begin()->first;
    visited.insert(start);
    queue.push(start);
    
    while (!queue.empty()) {
        Vertex current = queue.front();
        queue.pop();
        
        // Outgoing edges
        auto it = m_adj.find(current);
        if (it != m_adj.end()) {
            for (const auto& neighbor : it->second) {
                if (visited.find(neighbor.vertex) == visited.end()) {
                    visited.insert(neighbor.vertex);
                    queue.push(neighbor.vertex);
                }
            }
        }
        
        // For directed graph, also consider incoming edges (weak connectivity)
        if (m_directed) {
            for (const auto& pair : m_adj) {
                if (visited.find(pair.first) == visited.end()) {
                    for (const auto& neighbor : pair.second) {
                        if (neighbor.vertex == current) {
                            visited.insert(pair.first);
                            queue.push(pair.first);
                            break;
                        }
                    }
                }
            }
        }
    }
    
    return visited.size() == m_adj.size();
}

template <typename Vertex, typename Weight>
std::vector<Vertex> Graph<Vertex, Weight>::topological_sort() const {
    if (!m_directed) {
        throw std::runtime_error("Graph::topological_sort: only valid for directed graphs");
    }
    
    if (has_cycle()) {
        throw std::runtime_error("Graph::topological_sort: graph contains a cycle");
    }
    
    std::unordered_set<Vertex> visited;
    std::vector<Vertex> result;
    
    for (const auto& pair : m_adj) {
        if (visited.find(pair.first) == visited.end()) {
            topological_sort_dfs(pair.first, visited, result);
        }
    }
    
    std::reverse(result.begin(), result.end());
    return result;
}

template <typename Vertex, typename Weight>
std::vector<std::vector<Vertex>> Graph<Vertex, Weight>::connected_components() const {
    std::vector<std::vector<Vertex>> components;
    std::unordered_set<Vertex> visited;
    
    for (const auto& pair : m_adj) {
        if (visited.find(pair.first) == visited.end()) {
            std::vector<Vertex> component;
            
            // BFS to find all vertices in this component
            std::queue<Vertex> queue;
            queue.push(pair.first);
            visited.insert(pair.first);
            
            while (!queue.empty()) {
                Vertex current = queue.front();
                queue.pop();
                component.push_back(current);
                
                auto it = m_adj.find(current);
                if (it != m_adj.end()) {
                    for (const auto& neighbor : it->second) {
                        if (visited.find(neighbor.vertex) == visited.end()) {
                            visited.insert(neighbor.vertex);
                            queue.push(neighbor.vertex);
                        }
                    }
                }
                
                // For directed graph, check reverse edges too
                if (m_directed) {
                    for (const auto& p : m_adj) {
                        if (visited.find(p.first) == visited.end()) {
                            for (const auto& n : p.second) {
                                if (n.vertex == current) {
                                    visited.insert(p.first);
                                    queue.push(p.first);
                                    break;
                                }
                            }
                        }
                    }
                }
            }
            
            components.push_back(std::move(component));
        }
    }
    
    return components;
}

// Utility
template <typename Vertex, typename Weight>
void Graph<Vertex, Weight>::clear() noexcept {
    m_adj.clear();
    m_edge_count = 0;
}

template <typename Vertex, typename Weight>
void Graph<Vertex, Weight>::swap(Graph& other) noexcept {
    m_adj.swap(other.m_adj);
    std::swap(m_edge_count, other.m_edge_count);
    std::swap(m_directed, other.m_directed);
}

template <typename Vertex, typename Weight>
Graph<Vertex, Weight> Graph<Vertex, Weight>::transpose() const {
    Graph<Vertex, Weight> result(m_directed);
    
    // Add all vertices
    for (const auto& pair : m_adj) {
        result.add_vertex(pair.first);
    }
    
    // Add reversed edges
    for (const auto& pair : m_adj) {
        for (const auto& neighbor : pair.second) {
            result.m_adj[neighbor.vertex].emplace_back(pair.first, neighbor.weight);
            ++result.m_edge_count;
        }
    }
    
    if (!m_directed) {
        result.m_edge_count /= 2;
    }
    
    return result;
}

// Private helpers
template <typename Vertex, typename Weight>
typename Graph<Vertex, Weight>::AdjacencyList::iterator 
Graph<Vertex, Weight>::find_neighbor(AdjacencyList& adj, const Vertex& vertex) {
    for (auto it = adj.begin(); it != adj.end(); ++it) {
        if (it->vertex == vertex) {
            return it;
        }
    }
    return adj.end();
}

template <typename Vertex, typename Weight>
typename Graph<Vertex, Weight>::AdjacencyList::const_iterator 
Graph<Vertex, Weight>::find_neighbor(const AdjacencyList& adj, const Vertex& vertex) const {
    for (auto it = adj.begin(); it != adj.end(); ++it) {
        if (it->vertex == vertex) {
            return it;
        }
    }
    return adj.end();
}

template <typename Vertex, typename Weight>
bool Graph<Vertex, Weight>::has_cycle_dfs(
    const Vertex& vertex,
    std::unordered_set<Vertex>& visited,
    std::unordered_set<Vertex>& rec_stack) const {
    
    visited.insert(vertex);
    rec_stack.insert(vertex);
    
    auto it = m_adj.find(vertex);
    if (it != m_adj.end()) {
        for (const auto& neighbor : it->second) {
            // Back edge found - cycle detected
            if (rec_stack.find(neighbor.vertex) != rec_stack.end()) {
                return true;
            }
            // Explore unvisited neighbors
            if (visited.find(neighbor.vertex) == visited.end()) {
                if (has_cycle_dfs(neighbor.vertex, visited, rec_stack)) {
                    return true;
                }
            }
        }
    }
    
    rec_stack.erase(vertex);
    return false;
}

template <typename Vertex, typename Weight>
void Graph<Vertex, Weight>::dfs_helper(
    const Vertex& vertex,
    std::unordered_set<Vertex>& visited,
    std::function<void(const Vertex&)>& visitor) const {
    
    visited.insert(vertex);
    visitor(vertex);
    
    auto it = m_adj.find(vertex);
    if (it != m_adj.end()) {
        for (const auto& neighbor : it->second) {
            if (visited.find(neighbor.vertex) == visited.end()) {
                dfs_helper(neighbor.vertex, visited, visitor);
            }
        }
    }
}

template <typename Vertex, typename Weight>
void Graph<Vertex, Weight>::topological_sort_dfs(
    const Vertex& vertex,
    std::unordered_set<Vertex>& visited,
    std::vector<Vertex>& result) const {
    
    visited.insert(vertex);
    
    auto it = m_adj.find(vertex);
    if (it != m_adj.end()) {
        for (const auto& neighbor : it->second) {
            if (visited.find(neighbor.vertex) == visited.end()) {
                topological_sort_dfs(neighbor.vertex, visited, result);
            }
        }
    }
    
    result.push_back(vertex);
}

// Explicit template instantiations
template class Graph<int, double>;
template class Graph<int, int>;
template class Graph<int, float>;
template class Graph<std::string, double>;
template class Graph<std::string, int>;
template class Graph<char, double>;
template class Graph<char, int>;
template class Graph<long, double>;
template class Graph<long, int>;

} // namespace graph
} // namespace mylib