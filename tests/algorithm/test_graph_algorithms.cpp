/**
 * @file test_graph_algorithms.cpp
 * @brief Test suite for graph algorithms
 * @author Jinhyeok
 * @date 2025-11-30
 * @version 1.0.0
 */

#include "algorithm/graph_algorithms.hpp"
#include <iostream>
#include <cassert>
#include <string>
#include <vector>
#include <cmath>

using namespace mylib::algorithm;

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

// Helper for floating point comparison
bool approx_equal(double a, double b, double eps = 1e-9) {
    return std::abs(a - b) < eps;
}

// ============================================
// Union-Find Tests
// ============================================

void test_union_find_basic() {
    TEST("UnionFind basic operations")
    UnionFind<int> uf;
    
    uf.make_set(1);
    uf.make_set(2);
    uf.make_set(3);
    
    assert(uf.size() == 3);
    assert(uf.set_count() == 3);
    assert(!uf.connected(1, 2));
    END_TEST
}

void test_union_find_unite() {
    TEST("UnionFind unite")
    UnionFind<int> uf = {1, 2, 3, 4, 5};
    
    assert(uf.set_count() == 5);
    
    uf.unite(1, 2);
    assert(uf.set_count() == 4);
    assert(uf.connected(1, 2));
    
    uf.unite(3, 4);
    assert(uf.set_count() == 3);
    
    uf.unite(1, 3);
    assert(uf.set_count() == 2);
    assert(uf.connected(1, 4));
    assert(uf.connected(2, 3));
    END_TEST
}

void test_union_find_same_set() {
    TEST("UnionFind unite same set")
    UnionFind<int> uf = {1, 2, 3};
    
    uf.unite(1, 2);
    bool result = uf.unite(1, 2);  // Already same set
    
    assert(!result);
    assert(uf.set_count() == 2);
    END_TEST
}

void test_union_find_get_set() {
    TEST("UnionFind get_set")
    UnionFind<int> uf = {1, 2, 3, 4, 5};
    
    uf.unite(1, 2);
    uf.unite(2, 3);
    
    auto set1 = uf.get_set(1);
    assert(set1.size() == 3);
    
    auto set4 = uf.get_set(4);
    assert(set4.size() == 1);
    END_TEST
}

void test_union_find_get_all_sets() {
    TEST("UnionFind get_all_sets")
    UnionFind<int> uf = {1, 2, 3, 4, 5};
    
    uf.unite(1, 2);
    uf.unite(3, 4);
    
    auto all_sets = uf.get_all_sets();
    assert(all_sets.size() == 3);  // {1,2}, {3,4}, {5}
    END_TEST
}

void test_union_find_string() {
    TEST("UnionFind with strings")
    UnionFind<std::string> uf;
    
    uf.make_set("apple");
    uf.make_set("banana");
    uf.make_set("cherry");
    
    uf.unite("apple", "banana");
    
    assert(uf.connected("apple", "banana"));
    assert(!uf.connected("apple", "cherry"));
    END_TEST
}

void test_union_find_iterator_constructor() {
    TEST("UnionFind iterator constructor")
    std::vector<int> elements = {10, 20, 30, 40, 50};
    UnionFind<int> uf(elements.begin(), elements.end());
    
    assert(uf.size() == 5);
    assert(uf.set_count() == 5);
    END_TEST
}

// ============================================
// Bellman-Ford Tests
// ============================================

void test_bellman_ford_basic() {
    TEST("Bellman-Ford basic")
    std::vector<Edge<int, double>> edges = {
        {0, 1, 4},
        {0, 2, 5},
        {1, 2, -3},
        {2, 3, 4}
    };
    
    auto result = bellman_ford(edges, 0);
    
    assert(!result.has_negative_cycle);
    assert(approx_equal(result.distance_to(0).value(), 0));
    assert(approx_equal(result.distance_to(1).value(), 4));
    assert(approx_equal(result.distance_to(2).value(), 1));  // 0->1->2 = 4-3 = 1
    assert(approx_equal(result.distance_to(3).value(), 5));  // 0->1->2->3 = 1+4 = 5
    END_TEST
}

void test_bellman_ford_negative_weights() {
    TEST("Bellman-Ford with negative weights")
    std::vector<Edge<int, double>> edges = {
        {0, 1, 1},
        {1, 2, -1},
        {2, 3, -1},
        {0, 3, 2}
    };
    
    auto result = bellman_ford(edges, 0);
    
    assert(!result.has_negative_cycle);
    assert(approx_equal(result.distance_to(3).value(), -1));  // 0->1->2->3 = 1-1-1 = -1
    END_TEST
}

void test_bellman_ford_negative_cycle() {
    TEST("Bellman-Ford negative cycle detection")
    std::vector<Edge<int, double>> edges = {
        {0, 1, 1},
        {1, 2, -1},
        {2, 0, -1}  // Creates negative cycle: 0->1->2->0 = 1-1-1 = -1
    };
    
    auto result = bellman_ford(edges, 0);
    
    assert(result.has_negative_cycle);
    END_TEST
}

void test_bellman_ford_path_reconstruction() {
    TEST("Bellman-Ford path reconstruction")
    std::vector<Edge<int, double>> edges = {
        {0, 1, 2},
        {1, 2, 3},
        {0, 2, 10},
        {2, 3, 1}
    };
    
    auto result = bellman_ford(edges, 0);
    auto path = result.path_to(0, 3);
    
    assert(path.size() == 4);
    assert(path[0] == 0);
    assert(path[1] == 1);
    assert(path[2] == 2);
    assert(path[3] == 3);
    END_TEST
}

void test_bellman_ford_disconnected() {
    TEST("Bellman-Ford disconnected vertex")
    std::vector<Edge<int, double>> edges = {
        {0, 1, 1},
        {1, 2, 1}
        // 3 is disconnected
    };
    
    // Add vertex 3 without edges
    edges.push_back({3, 3, 0});  // Self-loop to include vertex
    
    auto result = bellman_ford(edges, 0);
    
    assert(!result.distance_to(3).has_value() || 
           result.distance_to(3).value() == std::numeric_limits<double>::max());
    END_TEST
}

void test_bellman_ford_single_vertex() {
    TEST("Bellman-Ford single vertex")
    std::vector<Edge<int, double>> edges;
    
    auto result = BellmanFord<int, double>::run(edges, 0, 1);
    
    assert(!result.has_negative_cycle);
    assert(approx_equal(result.distances[0], 0));
    END_TEST
}

void test_bellman_ford_string_vertices() {
    TEST("Bellman-Ford with string vertices")
    std::vector<Edge<std::string, double>> edges = {
        {"A", "B", 1},
        {"B", "C", 2},
        {"A", "C", 5}
    };
    
    auto result = bellman_ford(edges, std::string("A"));
    
    assert(!result.has_negative_cycle);
    assert(approx_equal(result.distance_to("C").value(), 3));  // A->B->C
    END_TEST
}

// ============================================
// Floyd-Warshall Tests
// ============================================

void test_floyd_warshall_basic() {
    TEST("Floyd-Warshall basic")
    std::vector<Edge<int, double>> edges = {
        {0, 1, 3},
        {0, 2, 8},
        {1, 2, 2},
        {2, 3, 1}
    };
    
    auto result = floyd_warshall(edges);
    
    assert(!result.has_negative_cycle);
    assert(approx_equal(result.distance(0, 0).value(), 0));
    assert(approx_equal(result.distance(0, 1).value(), 3));
    assert(approx_equal(result.distance(0, 2).value(), 5));  // 0->1->2
    assert(approx_equal(result.distance(0, 3).value(), 6));  // 0->1->2->3
    END_TEST
}

void test_floyd_warshall_all_pairs() {
    TEST("Floyd-Warshall all pairs")
    std::vector<Edge<int, double>> edges = {
        {0, 1, 1},
        {1, 2, 2},
        {2, 0, 3}
    };
    
    auto result = floyd_warshall(edges);
    
    // Check all pairs have distances
    assert(result.distance(0, 1).has_value());
    assert(result.distance(0, 2).has_value());
    assert(result.distance(1, 0).has_value());
    assert(result.distance(1, 2).has_value());
    assert(result.distance(2, 0).has_value());
    assert(result.distance(2, 1).has_value());
    END_TEST
}

void test_floyd_warshall_path_reconstruction() {
    TEST("Floyd-Warshall path reconstruction")
    std::vector<Edge<int, double>> edges = {
        {0, 1, 1},
        {1, 2, 1},
        {2, 3, 1},
        {0, 3, 10}
    };
    
    auto result = floyd_warshall(edges);
    auto path = result.path(0, 3);
    
    assert(path.size() == 4);
    assert(path[0] == 0);
    assert(path[1] == 1);
    assert(path[2] == 2);
    assert(path[3] == 3);
    END_TEST
}

void test_floyd_warshall_undirected() {
    TEST("Floyd-Warshall undirected graph")
    std::vector<Edge<int, double>> edges = {
        {0, 1, 1},
        {1, 2, 2}
    };
    
    auto result = floyd_warshall(edges, false);  // undirected
    
    // Both directions should work
    assert(approx_equal(result.distance(0, 2).value(), 3));
    assert(approx_equal(result.distance(2, 0).value(), 3));
    END_TEST
}

void test_floyd_warshall_negative_cycle() {
    TEST("Floyd-Warshall negative cycle detection")
    std::vector<Edge<int, double>> edges = {
        {0, 1, 1},
        {1, 2, -3},
        {2, 0, 1}  // Cycle: 0->1->2->0 = 1-3+1 = -1
    };
    
    auto result = floyd_warshall(edges);
    
    assert(result.has_negative_cycle);
    END_TEST
}

void test_floyd_warshall_no_path() {
    TEST("Floyd-Warshall no path exists")
    std::vector<Edge<int, double>> edges = {
        {0, 1, 1},
        {2, 3, 1}  // Disconnected components
    };
    
    auto result = floyd_warshall(edges);
    
    assert(!result.distance(0, 3).has_value());
    END_TEST
}

// ============================================
// Kruskal Tests
// ============================================

void test_kruskal_basic() {
    TEST("Kruskal basic")
    std::vector<Edge<int, double>> edges = {
        {0, 1, 4},
        {0, 2, 3},
        {1, 2, 1},
        {1, 3, 2},
        {2, 3, 4}
    };
    
    auto result = kruskal(edges);
    
    assert(result.exists);
    assert(result.edge_count() == 3);  // V-1 = 4-1 = 3
    assert(approx_equal(result.total_weight, 6));  // 1 + 2 + 3
    END_TEST
}

void test_kruskal_simple_triangle() {
    TEST("Kruskal simple triangle")
    std::vector<Edge<int, double>> edges = {
        {0, 1, 1},
        {1, 2, 2},
        {0, 2, 3}
    };
    
    auto result = kruskal(edges);
    
    assert(result.exists);
    assert(result.edge_count() == 2);
    assert(approx_equal(result.total_weight, 3));  // 1 + 2
    END_TEST
}

void test_kruskal_disconnected() {
    TEST("Kruskal disconnected graph")
    std::vector<Edge<int, double>> edges = {
        {0, 1, 1},
        {2, 3, 1}  // Disconnected
    };
    
    auto result = kruskal(edges);
    
    assert(!result.exists);  // MST doesn't exist for disconnected graph
    END_TEST
}

void test_kruskal_single_edge() {
    TEST("Kruskal single edge")
    std::vector<Edge<int, double>> edges = {
        {0, 1, 5}
    };
    
    auto result = kruskal(edges);
    
    assert(result.exists);
    assert(result.edge_count() == 1);
    assert(approx_equal(result.total_weight, 5));
    END_TEST
}

void test_kruskal_empty() {
    TEST("Kruskal empty graph")
    std::vector<Edge<int, double>> edges;
    
    auto result = kruskal(edges);
    
    assert(result.exists);
    assert(result.edge_count() == 0);
    END_TEST
}

void test_kruskal_contains_edge() {
    TEST("Kruskal contains_edge check")
    std::vector<Edge<int, double>> edges = {
        {0, 1, 1},
        {1, 2, 2},
        {0, 2, 10}  // This should NOT be in MST
    };
    
    auto result = kruskal(edges);
    
    assert(result.contains_edge(0, 1));
    assert(result.contains_edge(1, 2));
    assert(!result.contains_edge(0, 2));
    END_TEST
}

void test_kruskal_string_vertices() {
    TEST("Kruskal with string vertices")
    std::vector<Edge<std::string, double>> edges = {
        {"A", "B", 1},
        {"B", "C", 2},
        {"A", "C", 5}
    };
    
    auto result = kruskal(edges);
    
    assert(result.exists);
    assert(result.edge_count() == 2);
    assert(approx_equal(result.total_weight, 3));
    END_TEST
}

void test_kruskal_same_weights() {
    TEST("Kruskal with same weight edges")
    std::vector<Edge<int, double>> edges = {
        {0, 1, 1},
        {1, 2, 1},
        {2, 3, 1},
        {0, 3, 1}
    };
    
    auto result = kruskal(edges);
    
    assert(result.exists);
    assert(result.edge_count() == 3);
    assert(approx_equal(result.total_weight, 3));
    END_TEST
}

// ============================================
// Prim Tests
// ============================================

void test_prim_basic() {
    TEST("Prim basic")
    std::vector<Edge<int, double>> edges = {
        {0, 1, 4},
        {0, 2, 3},
        {1, 2, 1},
        {1, 3, 2},
        {2, 3, 4}
    };
    
    auto result = prim(edges, std::make_optional(0));
    
    assert(result.exists);
    assert(result.edge_count() == 3);
    assert(approx_equal(result.total_weight, 6));  // Same as Kruskal
    END_TEST
}

void test_prim_simple_triangle() {
    TEST("Prim simple triangle")
    std::vector<Edge<int, double>> edges = {
        {0, 1, 1},
        {1, 2, 2},
        {0, 2, 3}
    };
    
    auto result = prim(edges, std::make_optional(0));
    
    assert(result.exists);
    assert(result.edge_count() == 2);
    assert(approx_equal(result.total_weight, 3));
    END_TEST
}

void test_prim_different_start() {
    TEST("Prim different starting vertices")
    std::vector<Edge<int, double>> edges = {
        {0, 1, 1},
        {1, 2, 2},
        {0, 2, 5}
    };
    
    auto result1 = prim(edges, std::make_optional(0));
    auto result2 = prim(edges, std::make_optional(2));
    
    // MST weight should be same regardless of start
    assert(approx_equal(result1.total_weight, result2.total_weight));
    END_TEST
}

void test_prim_disconnected() {
    TEST("Prim disconnected graph")
    std::vector<Edge<int, double>> edges = {
        {0, 1, 1},
        {2, 3, 1}
    };
    
    auto result = prim(edges, std::make_optional(0));
    
    assert(!result.exists);
    END_TEST
}

void test_prim_single_edge() {
    TEST("Prim single edge")
    std::vector<Edge<int, double>> edges = {
        {0, 1, 5}
    };
    
    auto result = prim(edges);
    
    assert(result.exists);
    assert(result.edge_count() == 1);
    assert(approx_equal(result.total_weight, 5));
    END_TEST
}

void test_prim_empty() {
    TEST("Prim empty graph")
    std::vector<Edge<int, double>> edges;
    
    auto result = prim(edges);
    
    assert(result.exists);
    assert(result.edge_count() == 0);
    END_TEST
}

void test_prim_string_vertices() {
    TEST("Prim with string vertices")
    std::vector<Edge<std::string, double>> edges = {
        {"Seoul", "Busan", 325},
        {"Seoul", "Daegu", 237},
        {"Daegu", "Busan", 88}
    };
    
    auto result = prim(edges, std::make_optional(std::string("Seoul")));
    
    assert(result.exists);
    assert(result.edge_count() == 2);
    assert(approx_equal(result.total_weight, 325));  // Seoul-Daegu + Daegu-Busan
    END_TEST
}

// ============================================
// Kruskal vs Prim Comparison
// ============================================

void test_kruskal_prim_same_result() {
    TEST("Kruskal and Prim produce same MST weight")
    std::vector<Edge<int, double>> edges = {
        {0, 1, 10},
        {0, 2, 6},
        {0, 3, 5},
        {1, 3, 15},
        {2, 3, 4}
    };
    
    auto kruskal_result = kruskal(edges);
    auto prim_result = prim(edges, std::make_optional(0));
    
    assert(kruskal_result.exists);
    assert(prim_result.exists);
    assert(approx_equal(kruskal_result.total_weight, prim_result.total_weight));
    END_TEST
}

// ============================================
// Large Graph Tests
// ============================================

void test_bellman_ford_large() {
    TEST("Bellman-Ford large graph (100 vertices)")
    std::vector<Edge<int, double>> edges;
    
    // Create a chain: 0 -> 1 -> 2 -> ... -> 99
    for (int i = 0; i < 99; ++i) {
        edges.push_back({i, i + 1, 1.0});
    }
    
    auto result = bellman_ford(edges, 0);
    
    assert(!result.has_negative_cycle);
    assert(approx_equal(result.distance_to(99).value(), 99));
    END_TEST
}

void test_floyd_warshall_medium() {
    TEST("Floyd-Warshall medium graph (20 vertices)")
    std::vector<Edge<int, double>> edges;
    
    // Create complete graph
    for (int i = 0; i < 20; ++i) {
        for (int j = i + 1; j < 20; ++j) {
            edges.push_back({i, j, static_cast<double>(i + j)});
        }
    }
    
    auto result = floyd_warshall(edges, false);
    
    assert(!result.has_negative_cycle);
    assert(result.distance(0, 19).has_value());
    END_TEST
}

void test_kruskal_large() {
    TEST("Kruskal large graph (100 vertices)")
    std::vector<Edge<int, double>> edges;
    
    // Create a connected graph
    for (int i = 0; i < 99; ++i) {
        edges.push_back({i, i + 1, static_cast<double>(i % 10 + 1)});
    }
    // Add some cross edges
    for (int i = 0; i < 50; ++i) {
        edges.push_back({i, i + 50, 100.0});
    }
    
    auto result = kruskal(edges);
    
    assert(result.exists);
    assert(result.edge_count() == 99);
    END_TEST
}

void test_prim_large() {
    TEST("Prim large graph (100 vertices)")
    std::vector<Edge<int, double>> edges;
    
    for (int i = 0; i < 99; ++i) {
        edges.push_back({i, i + 1, static_cast<double>(i % 10 + 1)});
    }
    for (int i = 0; i < 50; ++i) {
        edges.push_back({i, i + 50, 100.0});
    }
    
    auto result = prim(edges, std::make_optional(0));
    
    assert(result.exists);
    assert(result.edge_count() == 99);
    END_TEST
}

// ============================================
// Edge Cases
// ============================================

void test_self_loop() {
    TEST("Graph with self-loop")
    std::vector<Edge<int, double>> edges = {
        {0, 0, 0},  // Self-loop
        {0, 1, 1},
        {1, 2, 2}
    };
    
    auto bf_result = bellman_ford(edges, 0);
    assert(!bf_result.has_negative_cycle);
    
    auto fw_result = floyd_warshall(edges);
    assert(!fw_result.has_negative_cycle);
    END_TEST
}

void test_parallel_edges() {
    TEST("Graph with parallel edges")
    std::vector<Edge<int, double>> edges = {
        {0, 1, 5},
        {0, 1, 3},  // Parallel edge with smaller weight
        {1, 2, 2}
    };
    
    auto result = bellman_ford(edges, 0);
    
    // Should use the shorter edge
    assert(approx_equal(result.distance_to(1).value(), 3));
    END_TEST
}

void test_zero_weight_edges() {
    TEST("Graph with zero weight edges")
    std::vector<Edge<int, double>> edges = {
        {0, 1, 0},
        {1, 2, 0},
        {2, 3, 1}
    };
    
    auto result = bellman_ford(edges, 0);
    
    assert(approx_equal(result.distance_to(2).value(), 0));
    assert(approx_equal(result.distance_to(3).value(), 1));
    END_TEST
}

void test_integer_weights() {
    TEST("Graph with integer weights")
    std::vector<Edge<int, int>> edges = {
        {0, 1, 4},
        {1, 2, 3},
        {0, 2, 10}
    };
    
    auto result = bellman_ford(edges, 0);
    
    assert(result.distance_to(2).value() == 7);
    END_TEST
}

// ============================================
// Practical Use Cases
// ============================================

void test_city_network() {
    TEST("City network shortest path")
    std::vector<Edge<std::string, double>> edges = {
        {"Seoul", "Suwon", 30},
        {"Seoul", "Incheon", 40},
        {"Suwon", "Daejeon", 120},
        {"Incheon", "Daejeon", 150},
        {"Daejeon", "Daegu", 80},
        {"Daegu", "Busan", 90}
    };
    
    auto result = bellman_ford(edges, std::string("Seoul"));
    
    // Seoul -> Suwon -> Daejeon -> Daegu -> Busan
    auto path = result.path_to("Seoul", "Busan");
    assert(path.size() == 5);
    assert(approx_equal(result.distance_to("Busan").value(), 320));
    END_TEST
}

void test_network_mst() {
    TEST("Network cable MST")
    // Minimum cost to connect all buildings
    std::vector<Edge<std::string, double>> edges = {
        {"A", "B", 100},
        {"A", "C", 200},
        {"B", "C", 50},
        {"B", "D", 150},
        {"C", "D", 100}
    };
    
    auto result = kruskal(edges);
    
    assert(result.exists);
    assert(result.edge_count() == 3);
    // MST: B-C(50) + A-B(100) + C-D(100) = 250
    assert(approx_equal(result.total_weight, 250));
    END_TEST
}

void test_flight_routes() {
    TEST("Flight routes all pairs")
    std::vector<Edge<std::string, double>> edges = {
        {"NYC", "LAX", 5},
        {"NYC", "CHI", 2},
        {"CHI", "LAX", 2},
        {"LAX", "SEA", 1},
        {"CHI", "SEA", 3}
    };
    
    auto result = floyd_warshall(edges);
    
    // NYC -> CHI -> LAX = 4 (shorter than direct 5)
    assert(approx_equal(result.distance("NYC", "LAX").value(), 4));
    // NYC -> CHI -> LAX -> SEA = 5
    assert(approx_equal(result.distance("NYC", "SEA").value(), 5));
    END_TEST
}

// Main test runner
int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "Graph Algorithms Test Suite" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;

    // Union-Find tests
    std::cout << "--- Union-Find Tests ---" << std::endl;
    test_union_find_basic();
    test_union_find_unite();
    test_union_find_same_set();
    test_union_find_get_set();
    test_union_find_get_all_sets();
    test_union_find_string();
    test_union_find_iterator_constructor();

    // Bellman-Ford tests
    std::cout << std::endl << "--- Bellman-Ford Tests ---" << std::endl;
    test_bellman_ford_basic();
    test_bellman_ford_negative_weights();
    test_bellman_ford_negative_cycle();
    test_bellman_ford_path_reconstruction();
    test_bellman_ford_disconnected();
    test_bellman_ford_single_vertex();
    test_bellman_ford_string_vertices();

    // Floyd-Warshall tests
    std::cout << std::endl << "--- Floyd-Warshall Tests ---" << std::endl;
    test_floyd_warshall_basic();
    test_floyd_warshall_all_pairs();
    test_floyd_warshall_path_reconstruction();
    test_floyd_warshall_undirected();
    test_floyd_warshall_negative_cycle();
    test_floyd_warshall_no_path();

    // Kruskal tests
    std::cout << std::endl << "--- Kruskal Tests ---" << std::endl;
    test_kruskal_basic();
    test_kruskal_simple_triangle();
    test_kruskal_disconnected();
    test_kruskal_single_edge();
    test_kruskal_empty();
    test_kruskal_contains_edge();
    test_kruskal_string_vertices();
    test_kruskal_same_weights();

    // Prim tests
    std::cout << std::endl << "--- Prim Tests ---" << std::endl;
    test_prim_basic();
    test_prim_simple_triangle();
    test_prim_different_start();
    test_prim_disconnected();
    test_prim_single_edge();
    test_prim_empty();
    test_prim_string_vertices();

    // Comparison tests
    std::cout << std::endl << "--- Comparison Tests ---" << std::endl;
    test_kruskal_prim_same_result();

    // Large graph tests
    std::cout << std::endl << "--- Large Graph Tests ---" << std::endl;
    test_bellman_ford_large();
    test_floyd_warshall_medium();
    test_kruskal_large();
    test_prim_large();

    // Edge cases
    std::cout << std::endl << "--- Edge Cases ---" << std::endl;
    test_self_loop();
    test_parallel_edges();
    test_zero_weight_edges();
    test_integer_weights();

    // Practical use cases
    std::cout << std::endl << "--- Practical Use Cases ---" << std::endl;
    test_city_network();
    test_network_mst();
    test_flight_routes();

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