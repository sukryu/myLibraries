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
│   │   └── heap.hpp
│   ├── hash/                  # Hash-based structures
│   │   └── hash_table.hpp
│   └── graph/                 # Graph structures
│       └── graph.hpp
├── src/                       # Implementation files
│   ├── linear/
│   ├── tree/
│   ├── hash/
│   └── graph/
├── tests/                     # Test suites
│   ├── linear/
│   ├── tree/
│   ├── hash/
│   └── graph/
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

#### AVL Tree Rotations
```
Right Rotation (LL):        Left Rotation (RR):
    y                           x
   / \                         / \
  x   C  →  x                 A   y
 / \       / \                   / \
A   B     A   y                 B   C
              / \
             B   C
```

### Hash Data Structures

| Data Structure | Description | Key Operations | Time Complexity |
|----------------|-------------|----------------|-----------------|
| **HashTable** | Separate chaining hash map | `insert`, `erase`, `find`, `operator[]` | O(1) average |

Features:
- Automatic rehashing when load factor exceeds threshold
- Prime number bucket sizing for better distribution
- Support for custom hash functions and key comparators

### Graph Data Structures

| Data Structure | Description | Key Operations | Time Complexity |
|----------------|-------------|----------------|-----------------|
| **Graph** | Adjacency list representation | `add_vertex/edge`, `bfs`, `dfs`, `dijkstra` | Varies by operation |

Features:
- Directed and undirected graphs
- Weighted edges
- BFS/DFS traversals
- Shortest path (BFS for unweighted, Dijkstra for weighted)
- Cycle detection
- Topological sort (DAG)
- Connected components

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
./tests/linear/test_dynamic_array
./tests/tree/test_avl_tree
./tests/hash/test_hash_table
./tests/graph/test_graph
```

## 📊 Usage Examples

### DynamicArray
```cpp
#include "linear/dynamic_array.hpp"
using namespace mylib::linear;

DynamicArray<int> arr = {1, 2, 3, 4, 5};
arr.push_back(6);
arr.pop_back();
std::cout << arr[0] << std::endl;  // 1
```

### Stack & Queue
```cpp
#include "linear/stack.hpp"
#include "linear/queue.hpp"
using namespace mylib::linear;

// Stack (LIFO)
Stack<int> stack;
stack.push(1);
stack.push(2);
std::cout << stack.top() << std::endl;  // 2

// Queue (FIFO)
Queue<int> queue;
queue.push(1);
queue.push(2);
std::cout << queue.front() << std::endl;  // 1
```

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
// path: Seoul -> Daegu -> Busan
// distance: 325.0

// BFS traversal
graph.bfs("Seoul", [](const std::string& city) {
    std::cout << city << " ";
});

// Topological sort (for DAGs)
Graph<std::string> courses(true);
courses.add_edge("Math101", "Math201");
courses.add_edge("Math201", "Math301");
auto order = courses.topological_sort();
```

## 🧪 Test Coverage

| Component | Tests | Status |
|-----------|-------|--------|
| DynamicArray | 21 | ✅ |
| Stack | 23 | ✅ |
| LinkedList | 33 | ✅ |
| Queue | 26 | ✅ |
| Deque | 35 | ✅ |
| BinarySearchTree | 40 | ✅ |
| AVLTree | 45 | ✅ |
| Heap | 42 | ✅ |
| HashTable | 47 | ✅ |
| Graph | 55 | ✅ |
| **Total** | **367** | ✅ |

## 🔮 Roadmap

### Algorithms (Coming Soon)
- [ ] **Sorting**: QuickSort, MergeSort, HeapSort
- [ ] **Graph**: Bellman-Ford, Floyd-Warshall, Kruskal, Prim
- [ ] **String**: KMP, Rabin-Karp pattern matching

### Additional Data Structures
- [ ] Red-Black Tree
- [ ] Trie (Prefix Tree)
- [ ] Disjoint Set (Union-Find)
- [ ] B-Tree

## 💻 Development Environment

This project was developed on:
- **Device**: Samsung Galaxy Z Fold
- **OS**: Android (Termux)
- **Editor**: Neovim with LSP (clangd)
- **Build**: CMake + Clang/LLVM

## 📝 Code Style

- **Naming**: `snake_case` for functions/variables, `PascalCase` for classes
- **Comments**: Doxygen-style documentation
- **Headers**: Include guards with `MYLIB_` prefix
- **Namespace**: `mylib::linear`, `mylib::tree`, `mylib::hash`, `mylib::graph`

## 📄 License

MIT License - See LICENSE file for details.

Copyright (c) 2025 Jinhyeok

---

**Happy coding!** 🚀