/**
 * @file test_graph.cpp
 * @brief Test suite for Graph class
 * @author Jinhyeok
 * @date 2025-11-29
 * @version 1.0.0
 */

#include "graph/graph.hpp"
#include <iostream>
#include <cassert>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace mylib::graph;

// Test result counter
int tests_passed = 0;
int tests_failed = 0;

// Helper macro for testing
#define TEST(name) \
    std::cout << "Testing: " << name << "... "; \
    try {

#define END_TEST \
        std::cout << "✓ PASSED" << std::endl; \
        tests_passed++; \
    } catch (const std::exception& e) { \
        std::cout << "✗ FAILED: " << e.what() << std::endl; \
        tests_failed++; \
    }

// ============================================
// Constructor Tests
// ============================================

void test_default_constructor() {
    TEST("Default constructor (directed)")
    Graph<int> graph;
    assert(graph.empty());
    assert(graph.vertex_count() == 0);
    assert(graph.edge_count() == 0);
    assert(graph.is_directed());
    END_TEST
}

void test_undirected_constructor() {
    TEST("Undirected graph constructor")
    Graph<int> graph(false);
    assert(graph.empty());
    assert(!graph.is_directed());
    END_TEST
}

void test_initializer_list_constructor() {
    TEST("Initializer list constructor")
    Graph<int> graph({1, 2, 3, 4, 5}, true);
    assert(graph.vertex_count() == 5);
    assert(graph.edge_count() == 0);
    assert(graph.has_vertex(1));
    assert(graph.has_vertex(5));
    END_TEST
}

void test_copy_constructor() {
    TEST("Copy constructor")
    Graph<int> graph1;
    graph1.add_edge(1, 2);
    graph1.add_edge(2, 3);
    
    Graph<int> graph2(graph1);
    assert(graph2.vertex_count() == 3);
    assert(graph2.edge_count() == 2);
    assert(graph2.has_edge(1, 2));
    
    // Verify deep copy
    graph2.add_edge(3, 4);
    assert(graph1.edge_count() == 2);
    assert(graph2.edge_count() == 3);
    END_TEST
}

void test_move_constructor() {
    TEST("Move constructor")
    Graph<int> graph1;
    graph1.add_edge(1, 2);
    graph1.add_edge(2, 3);
    
    Graph<int> graph2(std::move(graph1));
    assert(graph2.vertex_count() == 3);
    assert(graph2.edge_count() == 2);
    assert(graph1.empty());
    END_TEST
}

void test_copy_assignment() {
    TEST("Copy assignment operator")
    Graph<int> graph1;
    graph1.add_edge(1, 2);
    
    Graph<int> graph2;
    graph2 = graph1;
    assert(graph2.has_edge(1, 2));
    
    graph2.add_edge(2, 3);
    assert(graph1.edge_count() == 1);
    END_TEST
}

void test_move_assignment() {
    TEST("Move assignment operator")
    Graph<int> graph1;
    graph1.add_edge(1, 2);
    
    Graph<int> graph2;
    graph2 = std::move(graph1);
    assert(graph2.has_edge(1, 2));
    assert(graph1.empty());
    END_TEST
}

// ============================================
// Vertex Operations Tests
// ============================================

void test_add_vertex() {
    TEST("add_vertex")
    Graph<int> graph;
    
    assert(graph.add_vertex(1));
    assert(graph.add_vertex(2));
    assert(graph.add_vertex(3));
    assert(graph.vertex_count() == 3);
    
    // Duplicate
    assert(!graph.add_vertex(1));
    assert(graph.vertex_count() == 3);
    END_TEST
}

void test_remove_vertex() {
    TEST("remove_vertex")
    Graph<int> graph;
    graph.add_edge(1, 2);
    graph.add_edge(2, 3);
    graph.add_edge(1, 3);
    
    assert(graph.remove_vertex(2));
    assert(graph.vertex_count() == 2);
    assert(!graph.has_vertex(2));
    assert(!graph.has_edge(1, 2));
    assert(!graph.has_edge(2, 3));
    assert(graph.has_edge(1, 3));
    END_TEST
}

void test_has_vertex() {
    TEST("has_vertex")
    Graph<int> graph;
    graph.add_vertex(1);
    graph.add_vertex(2);
    
    assert(graph.has_vertex(1));
    assert(graph.has_vertex(2));
    assert(!graph.has_vertex(3));
    END_TEST
}

void test_vertices() {
    TEST("vertices()")
    Graph<int> graph({1, 2, 3, 4, 5});
    
    auto verts = graph.vertices();
    assert(verts.size() == 5);
    
    std::sort(verts.begin(), verts.end());
    assert(verts[0] == 1);
    assert(verts[4] == 5);
    END_TEST
}

void test_degree_directed() {
    TEST("degree() for directed graph")
    Graph<int> graph(true);
    graph.add_edge(1, 2);
    graph.add_edge(1, 3);
    graph.add_edge(2, 1);
    
    assert(graph.out_degree(1) == 2);
    assert(graph.in_degree(1) == 1);
    assert(graph.degree(1) == 3);
    END_TEST
}

void test_degree_undirected() {
    TEST("degree() for undirected graph")
    Graph<int> graph(false);
    graph.add_edge(1, 2);
    graph.add_edge(1, 3);
    graph.add_edge(1, 4);
    
    assert(graph.degree(1) == 3);
    assert(graph.degree(2) == 1);
    END_TEST
}

// ============================================
// Edge Operations Tests
// ============================================

void test_add_edge_directed() {
    TEST("add_edge (directed)")
    Graph<int> graph(true);
    
    assert(graph.add_edge(1, 2));
    assert(graph.edge_count() == 1);
    assert(graph.has_edge(1, 2));
    assert(!graph.has_edge(2, 1));  // Directed
    
    // Duplicate
    assert(!graph.add_edge(1, 2));
    assert(graph.edge_count() == 1);
    END_TEST
}

void test_add_edge_undirected() {
    TEST("add_edge (undirected)")
    Graph<int> graph(false);
    
    assert(graph.add_edge(1, 2));
    assert(graph.edge_count() == 1);
    assert(graph.has_edge(1, 2));
    assert(graph.has_edge(2, 1));  // Undirected
    END_TEST
}

void test_add_edge_with_weight() {
    TEST("add_edge with weight")
    Graph<int, double> graph;
    
    graph.add_edge(1, 2, 5.5);
    graph.add_edge(2, 3, 3.2);
    
    assert(std::abs(graph.get_weight(1, 2) - 5.5) < 0.001);
    assert(std::abs(graph.get_weight(2, 3) - 3.2) < 0.001);
    END_TEST
}

void test_remove_edge_directed() {
    TEST("remove_edge (directed)")
    Graph<int> graph(true);
    graph.add_edge(1, 2);
    graph.add_edge(2, 3);
    
    assert(graph.remove_edge(1, 2));
    assert(!graph.has_edge(1, 2));
    assert(graph.edge_count() == 1);
    
    assert(!graph.remove_edge(1, 2));  // Already removed
    END_TEST
}

void test_remove_edge_undirected() {
    TEST("remove_edge (undirected)")
    Graph<int> graph(false);
    graph.add_edge(1, 2);
    
    assert(graph.remove_edge(1, 2));
    assert(!graph.has_edge(1, 2));
    assert(!graph.has_edge(2, 1));
    assert(graph.edge_count() == 0);
    END_TEST
}

void test_set_weight() {
    TEST("set_weight")
    Graph<int, double> graph;
    graph.add_edge(1, 2, 5.0);
    
    assert(graph.set_weight(1, 2, 10.0));
    assert(std::abs(graph.get_weight(1, 2) - 10.0) < 0.001);
    
    assert(!graph.set_weight(1, 3, 5.0));  // Edge doesn't exist
    END_TEST
}

void test_edges() {
    TEST("edges()")
    Graph<int> graph(true);
    graph.add_edge(1, 2);
    graph.add_edge(2, 3);
    graph.add_edge(3, 1);
    
    auto edges = graph.edges();
    assert(edges.size() == 3);
    END_TEST
}

void test_neighbors() {
    TEST("neighbors()")
    Graph<int> graph(true);
    graph.add_edge(1, 2);
    graph.add_edge(1, 3);
    graph.add_edge(1, 4);
    
    auto nbrs = graph.neighbors(1);
    assert(nbrs.size() == 3);
    
    std::sort(nbrs.begin(), nbrs.end());
    assert(nbrs[0] == 2);
    assert(nbrs[1] == 3);
    assert(nbrs[2] == 4);
    END_TEST
}

void test_neighbors_with_weights() {
    TEST("neighbors_with_weights()")
    Graph<int, double> graph;
    graph.add_edge(1, 2, 1.5);
    graph.add_edge(1, 3, 2.5);
    
    auto nbrs = graph.neighbors_with_weights(1);
    assert(nbrs.size() == 2);
    END_TEST
}

// ============================================
// BFS Tests
// ============================================

void test_bfs_basic() {
    TEST("BFS basic traversal")
    Graph<int> graph(true);
    graph.add_edge(1, 2);
    graph.add_edge(1, 3);
    graph.add_edge(2, 4);
    graph.add_edge(3, 4);
    
    std::vector<int> visited;
    graph.bfs(1, [&visited](const int& v) {
        visited.push_back(v);
    });
    
    assert(visited.size() == 4);
    assert(visited[0] == 1);  // Start vertex first
    END_TEST
}

void test_bfs_disconnected() {
    TEST("BFS on disconnected graph")
    Graph<int> graph(true);
    graph.add_edge(1, 2);
    graph.add_vertex(3);  // Disconnected
    
    std::vector<int> visited;
    graph.bfs(1, [&visited](const int& v) {
        visited.push_back(v);
    });
    
    assert(visited.size() == 2);
    assert(std::find(visited.begin(), visited.end(), 3) == visited.end());
    END_TEST
}

// ============================================
// DFS Tests
// ============================================

void test_dfs_basic() {
    TEST("DFS basic traversal")
    Graph<int> graph(true);
    graph.add_edge(1, 2);
    graph.add_edge(1, 3);
    graph.add_edge(2, 4);
    graph.add_edge(3, 4);
    
    std::vector<int> visited;
    graph.dfs(1, [&visited](const int& v) {
        visited.push_back(v);
    });
    
    assert(visited.size() == 4);
    assert(visited[0] == 1);
    END_TEST
}

void test_dfs_recursive() {
    TEST("DFS recursive traversal")
    Graph<int> graph(true);
    graph.add_edge(1, 2);
    graph.add_edge(2, 3);
    graph.add_edge(3, 4);
    
    std::vector<int> visited;
    graph.dfs_recursive(1, [&visited](const int& v) {
        visited.push_back(v);
    });
    
    assert(visited.size() == 4);
    assert(visited[0] == 1);
    assert(visited[1] == 2);
    assert(visited[2] == 3);
    assert(visited[3] == 4);
    END_TEST
}

// ============================================
// Shortest Path Tests
// ============================================

void test_shortest_path_bfs() {
    TEST("shortest_path_bfs")
    Graph<int> graph(true);
    graph.add_edge(1, 2);
    graph.add_edge(2, 3);
    graph.add_edge(1, 3);  // Direct path
    graph.add_edge(3, 4);
    
    auto path = graph.shortest_path_bfs(1, 4);
    assert(path.size() == 3);
    assert(path[0] == 1);
    assert(path[1] == 3);
    assert(path[2] == 4);
    END_TEST
}

void test_shortest_path_bfs_no_path() {
    TEST("shortest_path_bfs with no path")
    Graph<int> graph(true);
    graph.add_edge(1, 2);
    graph.add_vertex(3);  // Disconnected
    
    auto path = graph.shortest_path_bfs(1, 3);
    assert(path.empty());
    END_TEST
}

void test_shortest_path_bfs_same_vertex() {
    TEST("shortest_path_bfs same vertex")
    Graph<int> graph(true);
    graph.add_vertex(1);
    
    auto path = graph.shortest_path_bfs(1, 1);
    assert(path.size() == 1);
    assert(path[0] == 1);
    END_TEST
}

void test_dijkstra_basic() {
    TEST("Dijkstra basic")
    Graph<int, double> graph(true);
    graph.add_edge(1, 2, 1.0);
    graph.add_edge(2, 3, 2.0);
    graph.add_edge(1, 3, 5.0);  // Longer direct path
    
    auto [path, dist] = graph.dijkstra(1, 3);
    assert(path.size() == 3);
    assert(path[0] == 1);
    assert(path[1] == 2);
    assert(path[2] == 3);
    assert(std::abs(dist - 3.0) < 0.001);  // 1 + 2 = 3
    END_TEST
}

void test_dijkstra_complex() {
    TEST("Dijkstra complex graph")
    Graph<int, int> graph(true);
    //     2
    //   / | \
    //  1  |  4
    //   \ | /
    //     3
    graph.add_edge(1, 2, 1);
    graph.add_edge(1, 3, 4);
    graph.add_edge(2, 3, 2);
    graph.add_edge(2, 4, 5);
    graph.add_edge(3, 4, 1);
    
    auto [path, dist] = graph.dijkstra(1, 4);
    // Shortest: 1->2->3->4 = 1+2+1 = 4
    assert(dist == 4);
    END_TEST
}

void test_dijkstra_all() {
    TEST("dijkstra_all")
    Graph<int, int> graph(true);
    graph.add_edge(1, 2, 1);
    graph.add_edge(1, 3, 4);
    graph.add_edge(2, 3, 2);
    
    auto distances = graph.dijkstra_all(1);
    assert(distances[1] == 0);
    assert(distances[2] == 1);
    assert(distances[3] == 3);  // 1->2->3
    END_TEST
}

void test_has_path() {
    TEST("has_path")
    Graph<int> graph(true);
    graph.add_edge(1, 2);
    graph.add_edge(2, 3);
    graph.add_vertex(4);  // Disconnected
    
    assert(graph.has_path(1, 3));
    assert(graph.has_path(1, 2));
    assert(!graph.has_path(1, 4));
    assert(!graph.has_path(3, 1));  // Directed
    END_TEST
}

// ============================================
// Graph Properties Tests
// ============================================

void test_has_cycle_directed() {
    TEST("has_cycle (directed)")
    Graph<int> graph(true);
    graph.add_edge(1, 2);
    graph.add_edge(2, 3);
    assert(!graph.has_cycle());
    
    graph.add_edge(3, 1);  // Create cycle
    assert(graph.has_cycle());
    END_TEST
}

void test_has_cycle_undirected() {
    TEST("has_cycle (undirected)")
    Graph<int> graph(false);
    graph.add_edge(1, 2);
    graph.add_edge(2, 3);
    assert(!graph.has_cycle());
    
    graph.add_edge(3, 1);  // Create cycle
    assert(graph.has_cycle());
    END_TEST
}

void test_has_cycle_self_loop() {
    TEST("has_cycle with self-loop")
    Graph<int> graph(true);
    graph.add_edge(1, 1);  // Self-loop
    assert(graph.has_cycle());
    END_TEST
}

void test_is_connected_undirected() {
    TEST("is_connected (undirected)")
    Graph<int> graph(false);
    graph.add_edge(1, 2);
    graph.add_edge(2, 3);
    assert(graph.is_connected());
    
    graph.add_vertex(4);  // Disconnected vertex
    assert(!graph.is_connected());
    
    graph.add_edge(3, 4);
    assert(graph.is_connected());
    END_TEST
}

void test_is_connected_directed() {
    TEST("is_connected (directed, weak connectivity)")
    Graph<int> graph(true);
    graph.add_edge(1, 2);
    graph.add_edge(2, 3);
    assert(graph.is_connected());
    END_TEST
}

void test_topological_sort() {
    TEST("topological_sort")
    Graph<int> graph(true);
    // DAG: 1 -> 2 -> 4
    //      1 -> 3 -> 4
    graph.add_edge(1, 2);
    graph.add_edge(1, 3);
    graph.add_edge(2, 4);
    graph.add_edge(3, 4);
    
    auto order = graph.topological_sort();
    assert(order.size() == 4);
    
    // Verify topological order
    std::unordered_map<int, size_t> pos;
    for (size_t i = 0; i < order.size(); ++i) {
        pos[order[i]] = i;
    }
    
    assert(pos[1] < pos[2]);
    assert(pos[1] < pos[3]);
    assert(pos[2] < pos[4]);
    assert(pos[3] < pos[4]);
    END_TEST
}

void test_topological_sort_cycle_exception() {
    TEST("topological_sort throws on cycle")
    Graph<int> graph(true);
    graph.add_edge(1, 2);
    graph.add_edge(2, 3);
    graph.add_edge(3, 1);  // Cycle
    
    bool exception_thrown = false;
    try {
        graph.topological_sort();
    } catch (const std::runtime_error&) {
        exception_thrown = true;
    }
    assert(exception_thrown);
    END_TEST
}

void test_connected_components() {
    TEST("connected_components")
    Graph<int> graph(false);
    // Component 1: 1-2-3
    graph.add_edge(1, 2);
    graph.add_edge(2, 3);
    // Component 2: 4-5
    graph.add_edge(4, 5);
    // Component 3: 6 (isolated)
    graph.add_vertex(6);
    
    auto components = graph.connected_components();
    assert(components.size() == 3);
    END_TEST
}

// ============================================
// Utility Tests
// ============================================

void test_clear() {
    TEST("clear()")
    Graph<int> graph;
    graph.add_edge(1, 2);
    graph.add_edge(2, 3);
    
    graph.clear();
    assert(graph.empty());
    assert(graph.vertex_count() == 0);
    assert(graph.edge_count() == 0);
    END_TEST
}

void test_swap() {
    TEST("swap()")
    Graph<int> graph1(true);
    graph1.add_edge(1, 2);
    
    Graph<int> graph2(false);
    graph2.add_edge(10, 20);
    graph2.add_edge(20, 30);
    
    graph1.swap(graph2);
    
    assert(graph1.edge_count() == 2);
    assert(!graph1.is_directed());
    assert(graph2.edge_count() == 1);
    assert(graph2.is_directed());
    END_TEST
}

void test_transpose() {
    TEST("transpose()")
    Graph<int> graph(true);
    graph.add_edge(1, 2);
    graph.add_edge(2, 3);
    
    auto transposed = graph.transpose();
    assert(transposed.has_edge(2, 1));
    assert(transposed.has_edge(3, 2));
    assert(!transposed.has_edge(1, 2));
    END_TEST
}

// ============================================
// String Vertex Tests
// ============================================

void test_string_vertices() {
    TEST("String vertices")
    Graph<std::string> graph;
    graph.add_edge("A", "B");
    graph.add_edge("B", "C");
    graph.add_edge("A", "C");
    
    assert(graph.vertex_count() == 3);
    assert(graph.has_edge("A", "B"));
    assert(graph.has_path("A", "C"));
    END_TEST
}

void test_string_dijkstra() {
    TEST("Dijkstra with string vertices")
    Graph<std::string, double> graph;
    graph.add_edge("Seoul", "Busan", 325.0);
    graph.add_edge("Seoul", "Daegu", 237.0);
    graph.add_edge("Daegu", "Busan", 88.0);
    
    auto [path, dist] = graph.dijkstra("Seoul", "Busan");
    // Direct: 325, Via Daegu: 237 + 88 = 325 (same)
    assert(path.size() >= 2);
    assert(path.front() == "Seoul");
    assert(path.back() == "Busan");
    END_TEST
}

// ============================================
// Edge Cases
// ============================================

void test_empty_graph() {
    TEST("Empty graph operations")
    Graph<int> graph;
    
    assert(graph.empty());
    assert(!graph.has_vertex(1));
    assert(!graph.has_edge(1, 2));
    assert(!graph.has_cycle());
    assert(graph.is_connected());  // Empty is connected
    END_TEST
}

void test_single_vertex() {
    TEST("Single vertex graph")
    Graph<int> graph;
    graph.add_vertex(1);
    
    assert(graph.vertex_count() == 1);
    assert(graph.edge_count() == 0);
    assert(graph.is_connected());
    assert(!graph.has_cycle());
    END_TEST
}

void test_self_loop() {
    TEST("Self-loop edge")
    Graph<int> graph;
    graph.add_edge(1, 1);
    
    assert(graph.has_edge(1, 1));
    assert(graph.edge_count() == 1);
    END_TEST
}

// ============================================
// Stress Tests
// ============================================

void test_large_graph() {
    TEST("Large graph (stress test)")
    Graph<int> graph(true);
    const int n = 1000;
    
    // Create a chain
    for (int i = 0; i < n - 1; ++i) {
        graph.add_edge(i, i + 1);
    }
    
    assert(graph.vertex_count() == n);
    assert(graph.edge_count() == n - 1);
    assert(graph.has_path(0, n - 1));
    assert(!graph.has_cycle());
    END_TEST
}

void test_dense_graph() {
    TEST("Dense graph")
    Graph<int> graph(true);
    const int n = 50;
    
    // Complete graph
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i != j) {
                graph.add_edge(i, j);
            }
        }
    }
    
    assert(graph.vertex_count() == n);
    assert(graph.edge_count() == n * (n - 1));
    END_TEST
}

void test_bfs_large() {
    TEST("BFS on large graph")
    Graph<int> graph(true);
    const int n = 500;
    
    for (int i = 0; i < n - 1; ++i) {
        graph.add_edge(i, i + 1);
    }
    
    int count = 0;
    graph.bfs(0, [&count](const int&) { ++count; });
    assert(count == n);
    END_TEST
}

// ============================================
// Practical Use Cases
// ============================================

void test_social_network() {
    TEST("Social network simulation")
    Graph<std::string> network(false);  // Undirected friendships
    
    network.add_edge("Alice", "Bob");
    network.add_edge("Alice", "Charlie");
    network.add_edge("Bob", "David");
    network.add_edge("Charlie", "David");
    
    // Are Alice and David connected?
    assert(network.has_path("Alice", "David"));
    
    // Friends of Alice
    auto friends = network.neighbors("Alice");
    assert(friends.size() == 2);
    END_TEST
}

void test_course_prerequisites() {
    TEST("Course prerequisites (topological sort)")
    Graph<std::string> courses(true);
    
    courses.add_edge("Math101", "Math201");
    courses.add_edge("Math201", "Math301");
    courses.add_edge("CS101", "CS201");
    courses.add_edge("Math101", "CS201");
    
    auto order = courses.topological_sort();
    
    std::unordered_map<std::string, size_t> pos;
    for (size_t i = 0; i < order.size(); ++i) {
        pos[order[i]] = i;
    }
    
    assert(pos["Math101"] < pos["Math201"]);
    assert(pos["Math201"] < pos["Math301"]);
    assert(pos["CS101"] < pos["CS201"]);
    assert(pos["Math101"] < pos["CS201"]);
    END_TEST
}

void test_city_routes() {
    TEST("City routes (shortest path)")
    Graph<std::string, int> routes(true);
    
    routes.add_edge("A", "B", 5);
    routes.add_edge("A", "C", 10);
    routes.add_edge("B", "C", 3);
    routes.add_edge("B", "D", 9);
    routes.add_edge("C", "D", 2);
    
    auto [path, dist] = routes.dijkstra("A", "D");
    // A->B->C->D = 5+3+2 = 10
    assert(dist == 10);
    assert(path.size() == 4);
    END_TEST
}

// Main test runner
int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "Graph Test Suite" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;

    // Constructor tests
    std::cout << "--- Constructor Tests ---" << std::endl;
    test_default_constructor();
    test_undirected_constructor();
    test_initializer_list_constructor();
    test_copy_constructor();
    test_move_constructor();
    test_copy_assignment();
    test_move_assignment();

    // Vertex operations
    std::cout << std::endl << "--- Vertex Operations ---" << std::endl;
    test_add_vertex();
    test_remove_vertex();
    test_has_vertex();
    test_vertices();
    test_degree_directed();
    test_degree_undirected();

    // Edge operations
    std::cout << std::endl << "--- Edge Operations ---" << std::endl;
    test_add_edge_directed();
    test_add_edge_undirected();
    test_add_edge_with_weight();
    test_remove_edge_directed();
    test_remove_edge_undirected();
    test_set_weight();
    test_edges();
    test_neighbors();
    test_neighbors_with_weights();

    // BFS tests
    std::cout << std::endl << "--- BFS Tests ---" << std::endl;
    test_bfs_basic();
    test_bfs_disconnected();

    // DFS tests
    std::cout << std::endl << "--- DFS Tests ---" << std::endl;
    test_dfs_basic();
    test_dfs_recursive();

    // Shortest path tests
    std::cout << std::endl << "--- Shortest Path Tests ---" << std::endl;
    test_shortest_path_bfs();
    test_shortest_path_bfs_no_path();
    test_shortest_path_bfs_same_vertex();
    test_dijkstra_basic();
    test_dijkstra_complex();
    test_dijkstra_all();
    test_has_path();

    // Graph properties
    std::cout << std::endl << "--- Graph Properties Tests ---" << std::endl;
    test_has_cycle_directed();
    test_has_cycle_undirected();
    test_has_cycle_self_loop();
    test_is_connected_undirected();
    test_is_connected_directed();
    test_topological_sort();
    test_topological_sort_cycle_exception();
    test_connected_components();

    // Utility tests
    std::cout << std::endl << "--- Utility Tests ---" << std::endl;
    test_clear();
    test_swap();
    test_transpose();

    // String vertex tests
    std::cout << std::endl << "--- String Vertex Tests ---" << std::endl;
    test_string_vertices();
    test_string_dijkstra();

    // Edge cases
    std::cout << std::endl << "--- Edge Cases ---" << std::endl;
    test_empty_graph();
    test_single_vertex();
    test_self_loop();

    // Stress tests
    std::cout << std::endl << "--- Stress Tests ---" << std::endl;
    test_large_graph();
    test_dense_graph();
    test_bfs_large();

    // Practical use cases
    std::cout << std::endl << "--- Practical Use Cases ---" << std::endl;
    test_social_network();
    test_course_prerequisites();
    test_city_routes();

    // Print summary
    std::cout << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Test Results" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "✓ Passed: " << tests_passed << std::endl;
    std::cout << "✗ Failed: " << tests_failed << std::endl;
    std::cout << "Total: " << (tests_passed + tests_failed) << std::endl;
    std::cout << "========================================" << std::endl;

    return tests_failed > 0 ? 1 : 0;
}