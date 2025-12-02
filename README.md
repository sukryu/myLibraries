# MyLibraries - C++ Data Structures & Algorithms

A comprehensive C++ library implementing fundamental data structures and algorithms from scratch, built for learning and practical use.

## 📖 Overview

This project is a personal journey to deeply understand data structures and algorithms by implementing them from the ground up in modern C++17. Rather than just using STL containers, I wanted to understand **how** they work internally - from memory management to algorithmic complexity.

### Why This Project?

- 🎯 **Deep Learning**: Understand the internals of data structures, not just their APIs
- 🔧 **Practical Skills**: Build real-world implementations with proper memory management
- 📚 **Reference Library**: Create a comprehensive, well-documented codebase for future use
- 🚀 **Foundation**: Prepare for building practical algorithms on top of solid data structures

## 🏗️ Project Structure

```
myLibraries/
├── include/                    # Header files
│   ├── linear/                # Linear data structures
│   │   ├── dynamic_array.hpp
│   │   ├── stack.hpp
│   │   ├── linked_list.hpp
│   │   ├── queue.hpp
│   │   └── deque.hpp
│   ├── tree/                  # Tree data structures
│   │   ├── binary_search_tree.hpp
│   │   ├── avl_tree.hpp
│   │   ├── heap.hpp
│   │   ├── red_black_tree.hpp
│   │   ├── trie.hpp
│   │   ├── b_tree.hpp
│   │   ├── segment_tree.hpp
│   │   └── fenwick_tree.hpp
│   ├── hash/                  # Hash-based structures
│   │   └── hash_table.hpp
│   ├── graph/                 # Graph structures
│   │   └── graph.hpp
│   └── algorithm/             # Algorithms (header-only)
│       ├── sorting.hpp
│       ├── graph_algorithms.hpp
│       └── string_algorithms.hpp
├── src/                       # Implementation files
│   ├── tree/
│   ├── hash/
│   └── graph/
├── tests/                     # Test suites
│   ├── tree/
│   ├── hash/
│   ├── graph/
│   └── algorithm/
├── CMakeLists.txt
└── README.md
```

## ✅ Implemented Data Structures

### Linear Data Structures

| Data Structure | Description | Key Operations | Time Complexity |
|----------------|-------------|----------------|-----------------|
| **DynamicArray** | Auto-resizing array with capacity management | `push_back`, `pop_back`, `operator[]` | O(1) amortized |
| **Stack** | LIFO container using DynamicArray | `push`, `pop`, `top` | O(1) |
| **LinkedList** | Doubly linked list with bidirectional traversal | `push_front/back`, `insert`, `erase` | O(1) ends, O(n) middle |
| **Queue** | FIFO container using LinkedList | `push`, `pop`, `front` | O(1) |
| **Deque** | Double-ended queue using LinkedList | `push_front/back`, `pop_front/back` | O(1) |

### Tree Data Structures

| Data Structure | Description | Key Operations | Time Complexity |
|----------------|-------------|----------------|-----------------|
| **BinarySearchTree** | Basic BST with recursive operations | `insert`, `remove`, `find`, traversals | O(log n) avg, O(n) worst |
| **AVLTree** | Self-balancing BST with rotations | `insert`, `remove`, `find` | O(log n) guaranteed |
| **Heap** | Binary heap with MaxHeap/MinHeap support | `push`, `pop`, `top`, `heapify` | O(log n) push/pop, O(n) heapify |
| **RedBlackTree** | Self-balancing BST with red-black coloring | `insert`, `remove`, `find` | O(log n) guaranteed |
| **Trie** | Prefix tree for string operations | `insert`, `search`, `starts_with`, `autocomplete` | O(m) where m = key length |
| **BTree** | Self-balancing multiway search tree | `insert`, `remove`, `search` | O(log n) guaranteed |
| **SegmentTree** | Range query tree with lazy propagation | `query`, `update`, `range_update` | O(log n) per operation |
| **FenwickTree** | Binary indexed tree for prefix sums | `query`, `update`, `range_query` | O(log n) per operation |

### Hash Data Structures

| Data Structure | Description | Key Operations | Time Complexity |
|----------------|-------------|----------------|-----------------|
| **HashTable** | Separate chaining hash map | `insert`, `erase`, `find`, `operator[]` | O(1) average |

### Graph Data Structures

| Data Structure | Description | Key Operations | Time Complexity |
|----------------|-------------|----------------|-----------------|
| **Graph** | Adjacency list representation | `add_vertex/edge`, `bfs`, `dfs`, `dijkstra` | Varies by operation |

## ✅ Implemented Algorithms

### Sorting Algorithms

| Algorithm | Time Complexity | Space | Stable | Description |
|-----------|----------------|-------|--------|-------------|
| **QuickSort** | O(n log n) avg | O(log n) | No | Median-of-three pivot, tail recursion optimized |
| **MergeSort** | O(n log n) | O(n) | Yes | Divide and conquer, stable sorting |
| **HeapSort** | O(n log n) | O(1) | No | In-place using binary heap |
| **InsertionSort** | O(n²) | O(1) | Yes | Efficient for small/nearly sorted arrays |

#### Sorting Features
- **Fluent Interface**: Chain configuration methods
- **Key-based Sorting**: Sort by extracted key (like Python's `key=`)
- **Statistics Collection**: Track comparisons, swaps, time elapsed
- **Utility Functions**: `sorted()`, `argsort()`, `top_k()`, `bottom_k()`, `shuffle()`

```cpp
#include "algorithm/sorting.hpp"
using namespace mylib::algorithm;

// Simple sort
std::vector<int> v = {5, 2, 8, 1, 9};
quick_sort(v.begin(), v.end());

// Fluent interface with statistics
auto stats = Sorter<int>::with_stats()
    .descending()
    .quick_sort(v);
std::cout << "Comparisons: " << stats.comparisons << std::endl;

// Sort by key (like Python)
std::vector<std::string> words = {"apple", "pie", "banana"};
Sorter<std::string>::by_key([](const std::string& s) {
    return s.length();
}).sort(words);
// Result: {"pie", "apple", "banana"}

// Get sorted indices (like numpy.argsort)
auto indices = Sorter<int>::argsort(v);

// Get top k elements
auto top3 = Sorter<int>::top_k(v, 3);
```

### Graph Algorithms

| Algorithm | Time Complexity | Space | Description |
|-----------|----------------|-------|-------------|
| **Bellman-Ford** | O(V × E) | O(V) | Single-source shortest path, handles negative weights |
| **Floyd-Warshall** | O(V³) | O(V²) | All-pairs shortest path |
| **Kruskal** | O(E log E) | O(V) | Minimum Spanning Tree using Union-Find |
| **Prim** | O(E log V) | O(V) | Minimum Spanning Tree using priority queue |

#### Additional: Union-Find (Disjoint Set)
- Path compression + Union by rank
- Near O(1) operations: `find()`, `unite()`, `connected()`

```cpp
#include "algorithm/graph_algorithms.hpp"
using namespace mylib::algorithm;

// Bellman-Ford: Single-source shortest path (handles negative weights)
std::vector<Edge<int, double>> edges = {
    {0, 1, 4}, {1, 2, -2}, {0, 2, 5}
};
auto result = bellman_ford(edges, 0);
if (!result.has_negative_cycle) {
    auto dist = result.distance_to(2);   // std::optional<double>
    auto path = result.path_to(0, 2);    // std::vector<int>
}

// Floyd-Warshall: All-pairs shortest path
auto all_pairs = floyd_warshall(edges);
auto dist = all_pairs.distance(0, 2);
auto path = all_pairs.path(0, 2);

// Kruskal: Minimum Spanning Tree
auto mst = kruskal(edges);
std::cout << "MST weight: " << mst.total_weight << std::endl;
for (const auto& e : mst.edges) {
    std::cout << e.from << " - " << e.to << ": " << e.weight << std::endl;
}

// Prim: Minimum Spanning Tree
auto mst2 = prim(edges, std::make_optional(0));

// Union-Find
UnionFind<int> uf = {1, 2, 3, 4, 5};
uf.unite(1, 2);
uf.unite(3, 4);
bool same = uf.connected(1, 2);  // true
std::cout << "Sets: " << uf.set_count() << std::endl;  // 3
```

### String Algorithms

| Algorithm | Time Complexity | Space | Description |
|-----------|----------------|-------|-------------|
| **KMP** | O(n + m) | O(m) | Fast single pattern matching with LPS array |
| **Rabin-Karp** | O(n + m) avg | O(1) | Hash-based matching, efficient for multiple patterns |

#### String Utilities
| Function | Description |
|----------|-------------|
| `starts_with()`, `ends_with()` | Prefix/suffix checking |
| `longest_common_prefix/suffix()` | Find common parts |
| `is_palindrome()`, `longest_palindrome()` | Palindrome detection |
| `edit_distance()` | Levenshtein distance |
| `lcs()`, `lcs_length()` | Longest common subsequence |

```cpp
#include "algorithm/string_algorithms.hpp"
using namespace mylib::algorithm;

// KMP: Single pattern matching
auto result = KMP::search("hello world", "world");
if (result.found()) {
    std::cout << "Found at: " << result.first().value() << std::endl;
}

// Find all occurrences
auto matches = KMP::search("abababab", "ab");
std::cout << "Count: " << matches.count() << std::endl;  // 4

// Reusable matcher
KMP matcher("pattern");
auto r1 = matcher.match("text with pattern");
auto r2 = matcher.match("another pattern here");

// Replace and split
auto replaced = KMP::replace_all("hello world", "world", "universe");
auto parts = KMP::split("a,b,c,d", ",");

// Rabin-Karp: Multiple patterns at once
std::vector<std::string> patterns = {"cat", "dog", "bird"};
auto multi = RabinKarp::search_multiple("I have a cat and a dog", patterns);

// String utilities
bool is_pal = is_palindrome("racecar");           // true
auto lcp = longest_common_prefix("hello", "help"); // "hel"
auto dist = edit_distance("kitten", "sitting");    // 3
auto common = lcs("ABCDGH", "AEDFHR");             // "ADH"
```

## 🚀 Getting Started

### Prerequisites

- C++17 compatible compiler (GCC, Clang, MSVC)
- CMake 3.10 or higher

### Building the Project

```bash
# Clone the repository
git clone <your-repo-url>
cd myLibraries

# Create build directory
mkdir -p build && cd build

# Configure and build
cmake ..
cmake --build .

# Run all tests
ctest --output-on-failure

# Or run individual tests
./tests/tree/test_avl_tree
./tests/tree/test_red_black_tree
./tests/tree/test_trie
./tests/tree/test_b_tree
./tests/tree/test_segment_tree
./tests/tree/test_fenwick_tree
./tests/hash/test_hash_table
./tests/graph/test_graph
./tests/algorithm/test_sorting
./tests/algorithm/test_graph_algorithms
./tests/algorithm/test_string_algorithms
```

## 📊 Usage Examples

### Binary Search Tree & AVL Tree
```cpp
#include "tree/avl_tree.hpp"
using namespace mylib::tree;

AVLTree<int> tree;
tree.insert(10);
tree.insert(5);
tree.insert(15);

// Inorder traversal (sorted)
tree.inorder([](int val) { std::cout << val << " "; });
// Output: 5 10 15
```

### Red-Black Tree
```cpp
#include "tree/red_black_tree.hpp"
using namespace mylib::tree;

RedBlackTree<int> tree;
tree.insert(10);
tree.insert(20);
tree.insert(5);

// Guaranteed O(log n) operations
assert(tree.verify());  // Check RB properties
assert(tree.is_root_black());

// Iterator support
for (auto val : tree) {
    std::cout << val << " ";
}
```

### Trie (Prefix Tree)
```cpp
#include "tree/trie.hpp"
using namespace mylib::tree;

Trie<> trie;
trie.insert("apple");
trie.insert("application");
trie.insert("apply");

// Prefix search
bool has_prefix = trie.starts_with("app");  // true

// Autocomplete
auto suggestions = trie.autocomplete("app", 5);
// Returns: ["apple", "application", "apply"]

// Pattern matching with wildcards
auto matches = trie.match_pattern("ap?le");  // "apple"

// Word frequency
trie.insert("apple");  // Insert again
std::size_t freq = trie.count("apple");  // 2
```

### B-Tree
```cpp
#include "tree/b_tree.hpp"
using namespace mylib::tree;

// B-Tree with minimum degree 3
BTree<int> tree(3);

// Insert elements
for (int i = 1; i <= 20; ++i) {
    tree.insert(i);
}

// Search
if (tree.search(15)) {
    std::cout << "Found!" << std::endl;
}

// Remove
tree.remove(10);

// Tree properties
std::cout << "Height: " << tree.height() << std::endl;
assert(tree.verify());  // Verify B-Tree properties

// Inorder traversal (sorted)
tree.inorder([](int val) {
    std::cout << val << " ";
});
```

### Segment Tree
```cpp
#include "tree/segment_tree.hpp"
using namespace mylib::tree;

// Range sum queries
std::vector<int> arr = {1, 3, 5, 7, 9, 11};
auto tree = create_sum_segment_tree(arr, true);  // with lazy propagation

// Query sum of range [1, 4]
int sum = tree.range_query(1, 4);  // 3+5+7+9 = 24

// Point update: set arr[2] = 10
tree.update(2, 10);

// Range update: add 5 to arr[1..3]
tree.range_update(1, 3, 5);

// Range minimum queries
auto min_tree = create_min_segment_tree(arr);
int min_val = min_tree.range_query(0, 5);

// Custom merge operation (e.g., GCD)
auto gcd_tree = create_gcd_segment_tree(arr);
int gcd = gcd_tree.range_query(0, 3);

// Specialized classes
RangeSumSegmentTree<int> sum_tree(arr);
RangeMinSegmentTree<int> min_tree(arr);
RangeMaxSegmentTree<int> max_tree(arr);
```

### Fenwick Tree (Binary Indexed Tree)
```cpp
#include "tree/fenwick_tree.hpp"
using namespace mylib::tree;

// Basic Fenwick Tree for prefix sums
std::vector<int> arr = {3, 2, -1, 6, 5, 4};
FenwickTree<int> tree(arr);

// Prefix sum [0, 3]
int sum = tree.prefix_sum(3);  // 3+2+(-1)+6 = 10

// Range sum [2, 5]
int range_sum = tree.range_sum(2, 5);  // -1+6+5+4 = 14

// Update: add 10 to arr[3]
tree.update(3, 10);

// Set value: arr[2] = 100
tree.set(2, 100);

// Get current value
int val = tree.get(3);

// Binary search: find smallest index with prefix_sum >= target
std::size_t idx = tree.lower_bound(20);

// 2D Fenwick Tree for 2D range sums
FenwickTree2D<int> tree2d(5, 5);
tree2d.update(2, 3, 10);  // Add 10 to position [2,3]

// Rectangle sum query [1,1] to [3,4]
int rect_sum = tree2d.range_sum(1, 1, 3, 4);

// Simpler than Segment Tree, faster constants
// Best for: cumulative frequency, range sum queries
```

### Heap (Priority Queue)
```cpp
#include "tree/heap.hpp"
using namespace mylib::tree;

MaxHeap<int> maxHeap = {3, 1, 4, 1, 5, 9};
std::cout << maxHeap.top() << std::endl;  // 9

MinHeap<int> minHeap = {3, 1, 4, 1, 5, 9};
std::cout << minHeap.top() << std::endl;  // 1
```

### Hash Table
```cpp
#include "hash/hash_table.hpp"
using namespace mylib::hash;

HashTable<std::string, int> table;
table["apple"] = 5;
table["banana"] = 3;

if (table.contains("apple")) {
    std::cout << table["apple"] << std::endl;  // 5
}
```

### Graph
```cpp
#include "graph/graph.hpp"
using namespace mylib::graph;

// Directed weighted graph
Graph<std::string, double> graph(true);
graph.add_edge("Seoul", "Busan", 325.0);
graph.add_edge("Seoul", "Daegu", 237.0);
graph.add_edge("Daegu", "Busan", 88.0);

// Shortest path using Dijkstra
auto [path, distance] = graph.dijkstra("Seoul", "Busan");

// BFS traversal
graph.bfs("Seoul", [](const std::string& city) {
    std::cout << city << " ";
});

// Topological sort (for DAGs)
auto order = graph.topological_sort();
```

## 🧪 Test Coverage

### Data Structures

| Component | Tests | Status |
|-----------|-------|--------|
| BinarySearchTree | 40 | ✅ |
| AVLTree | 45 | ✅ |
| Heap | 42 | ✅ |
| RedBlackTree | 45 | ✅ |
| Trie | 48 | ✅ |
| BTree | 41 | ✅ |
| SegmentTree | 39 | ✅ |
| FenwickTree | 37 | ✅ |
| HashTable | 47 | ✅ |
| Graph | 55 | ✅ |
| **Subtotal** | **439** | ✅ |

### Algorithms

| Component | Tests | Status |
|-----------|-------|--------|
| Sorting (QuickSort, MergeSort, HeapSort) | 52 | ✅ |
| Graph (Bellman-Ford, Floyd-Warshall, Kruskal, Prim) | 47 | ✅ |
| String (KMP, Rabin-Karp) | 47 | ✅ |
| **Subtotal** | **146** | ✅ |

### Total: **585 Tests** ✅

## 🔮 Roadmap

### Completed ✅
- [x] Red-Black Tree
- [x] Trie (Prefix Tree)
- [x] B-Tree
- [x] Segment Tree (with lazy propagation)
- [x] Fenwick Tree / Binary Indexed Tree

### Future Enhancements
- [ ] Suffix Array / Suffix Tree
- [ ] Skip List
- [ ] Bloom Filter
- [ ] Persistent Data Structures
- [ ] Splay Tree
- [ ] Treap (Randomized BST)
- [ ] Dynamic Programming examples
- [ ] More advanced graph algorithms (Tarjan's, Kosaraju's)
- [ ] Network Flow algorithms
- [ ] Geometric algorithms (Convex Hull, Line Sweep)

## 💻 Development Environment

This project was developed on:
- **Device**: Samsung Galaxy Z Fold 7
- **OS**: Android (Termux + Ubuntu 22.04 LTS via proot-distro)
- **Editor**: Neovim with LSP (clangd)
- **Build**: CMake + Clang/LLVM
- **Version Control**: Git with SSH authentication

## 📝 Code Style

- **Naming**: `snake_case` for functions/variables, `PascalCase` for classes
- **Comments**: Doxygen-style documentation
- **Headers**: Include guards with `MYLIB_` prefix
- **Namespace**: `mylib::linear`, `mylib::tree`, `mylib::hash`, `mylib::graph`, `mylib::algorithm`
- **Modern C++**: Using C++17 features (structured bindings, std::optional, etc.)

## 📄 License

MIT License - See LICENSE file for details.

Copyright (c) 2025 Jinhyeok

---

**Happy coding!** 🚀