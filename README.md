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
├── CMakeLists.txt
├── include/
│   ├── linear/
│   │   ├── dynamic_array.hpp
│   │   ├── stack.hpp
│   │   ├── linked_list.hpp
│   │   ├── queue.hpp
│   │   └── deque.hpp
│   └── tree/
│       ├── binary_search_tree.hpp
│       └── avl_tree.hpp
├── src/
│   ├── linear/
│   │   ├── CMakeLists.txt
│   │   ├── dynamic_array.cpp
│   │   ├── stack.cpp
│   │   ├── linked_list.cpp
│   │   ├── queue.cpp
│   │   └── deque.cpp
│   └── tree/
│       ├── CMakeLists.txt
│       ├── binary_search_tree.cpp
│       └── avl_tree.cpp
├── tests/
│   ├── CMakeLists.txt
│   ├── linear/
│   │   ├── CMakeLists.txt
│   │   ├── test_dynamic_array.cpp
│   │   ├── test_stack.cpp
│   │   ├── test_linked_list.cpp
│   │   ├── test_queue.cpp
│   │   └── test_deque.cpp
│   └── tree/
│       ├── CMakeLists.txt
│       ├── test_binary_search_tree.cpp
│       └── test_avl_tree.cpp
└── README.md
```

## ✅ Implemented Data Structures

### Linear Data Structures

| Data Structure | Description | Key Operations | Time Complexity |
|---------------|-------------|----------------|-----------------|
| **DynamicArray** | Auto-resizing array with capacity management | push_back, pop_back, operator[] | O(1) amortized |
| **Stack** | LIFO container using DynamicArray | push, pop, top | O(1) |
| **LinkedList** | Doubly linked list with bidirectional traversal | push_front/back, insert, erase | O(1) ends, O(n) middle |
| **Queue** | FIFO container using LinkedList | push, pop, front | O(1) |
| **Deque** | Double-ended queue using LinkedList | push_front/back, pop_front/back | O(1) |

### Tree Data Structures

| Data Structure | Description | Key Operations | Time Complexity |
|---------------|-------------|----------------|-----------------|
| **BinarySearchTree** | Ordered binary tree | insert, remove, find, traversals | O(log n) avg, O(n) worst |
| **AVLTree** | Self-balancing BST | insert, remove, find, rotations | O(log n) guaranteed |

### Features

✨ **Modern C++17**
- Move semantics for efficiency
- Template-based generic programming
- RAII and smart memory management
- Exception safety

📝 **Comprehensive Documentation**
- Doxygen-style comments
- Clear function descriptions
- Usage examples in tests

🧪 **Thorough Testing**
- Unit tests for all functions
- Edge case coverage
- Stress tests with large datasets
- Memory leak verification

## 🚀 Getting Started

### Prerequisites

- C++17 compatible compiler (GCC, Clang, MSVC)
- CMake 3.10 or higher
- Git

For Termux (Android):
```bash
pkg install clang cmake git
```

### Building the Project

```bash
# Clone the repository
git clone <your-repo-url>
cd myLibraries

# Create build directory
mkdir -p build
cd build

# Configure and build
cmake ..
cmake --build .

# Run all tests
ctest

# Or run individual tests
./tests/linear/test_dynamic_array
./tests/linear/test_stack
./tests/linear/test_linked_list
./tests/linear/test_queue
./tests/linear/test_deque
./tests/tree/test_binary_search_tree
./tests/tree/test_avl_tree
```

### Quick Example

```cpp
#include "linear/stack.hpp"
#include "linear/queue.hpp"
#include "tree/avl_tree.hpp"
#include <iostream>
#include <vector>

using namespace mylib::linear;
using namespace mylib::tree;

int main() {
    // Stack example (LIFO)
    Stack<int> stack;
    stack.push(1);
    stack.push(2);
    stack.push(3);
    std::cout << stack.top() << std::endl;  // 3
    
    // Queue example (FIFO)
    Queue<int> queue;
    queue.push(1);
    queue.push(2);
    queue.push(3);
    std::cout << queue.front() << std::endl;  // 1
    
    // AVL Tree example (self-balancing BST)
    AVLTree<int> avl;
    for (int i = 1; i <= 1000; ++i) {
        avl.insert(i);  // Stays balanced!
    }
    std::cout << "Size: " << avl.size() << std::endl;    // 1000
    std::cout << "Height: " << avl.height() << std::endl; // ~10 (not 1000!)
    
    // Inorder traversal (sorted output)
    std::vector<int> sorted;
    avl.inorder([&sorted](const int& val) {
        sorted.push_back(val);
    });
    
    return 0;
}
```

## 📊 Implementation Details

### Linear Data Structures

#### DynamicArray
- **Internal Storage**: Raw array with manual memory management
- **Growth Strategy**: 2x capacity when full
- **Memory**: Contiguous allocation for cache efficiency

#### Stack
- **Container Adapter**: Uses DynamicArray internally
- **Operations**: All O(1) time complexity
- **Use Cases**: Expression evaluation, undo/redo, backtracking

#### LinkedList
- **Structure**: Doubly linked nodes with prev/next pointers
- **Optimization**: Bidirectional traversal from nearest end
- **Memory**: Dynamic allocation per node

#### Queue
- **Container Adapter**: Uses LinkedList internally
- **Operations**: O(1) enqueue and dequeue
- **Use Cases**: BFS, task scheduling, buffering

#### Deque
- **Container Adapter**: Uses LinkedList internally
- **Flexibility**: Combines Stack and Queue operations
- **Use Cases**: Sliding window, palindrome checking

### Tree Data Structures

#### BinarySearchTree
- **Property**: Left < Root < Right
- **Traversals**: Inorder, Preorder, Postorder, Level-order
- **Operations**: Insert, Remove (3 cases), Find, Min/Max
- **Weakness**: Can become skewed (O(n) worst case)

#### AVLTree
- **Balance Factor**: |height(left) - height(right)| ≤ 1
- **Rotations**: LL, RR, LR, RL
- **Guarantee**: O(log n) for all operations
- **Height**: Always ≤ 1.44 * log2(n)

**AVL Rotation Cases:**
```
LL Case (Right Rotation):     RR Case (Left Rotation):
      y            x              x            y
     / \          / \            / \          / \
    x  T3  -->  T1   y          T1  y  -->   x  T3
   / \              / \            / \      / \
  T1 T2            T2 T3          T2 T3    T1 T2

LR Case:                      RL Case:
Left rotate x, then           Right rotate y, then
right rotate y                left rotate x
```

## 🧪 Testing

Each data structure has a comprehensive test suite covering:

- ✅ Constructor variations (default, copy, move, initializer list)
- ✅ Basic operations (insert, delete, access)
- ✅ Edge cases (empty container, single element)
- ✅ Exception handling (out of range, empty container)
- ✅ Memory management (no leaks, proper cleanup)
- ✅ Large datasets (stress testing with 1000-10000 elements)
- ✅ Different data types (int, double, custom types)
- ✅ AVL-specific: Rotation cases, balance verification

### Test Results

```
Linear Data Structures:
✓ DynamicArray: 21/21 tests passed
✓ Stack: 23/23 tests passed
✓ LinkedList: 33/33 tests passed
✓ Queue: 26/26 tests passed
✓ Deque: 35/35 tests passed

Tree Data Structures:
✓ BinarySearchTree: 40/40 tests passed
✓ AVLTree: 45/45 tests passed
```

## 🔮 Future Plans

### Upcoming Data Structures
- [ ] **Heap**: Binary heap / Priority Queue
- [ ] **Hash Table**: Hash map with collision handling
- [ ] **Red-Black Tree**: Alternative self-balancing BST
- [ ] **Graph**: Adjacency list/matrix representations
- [ ] **Trie**: Prefix tree for string operations

### Planned Algorithms
- [ ] **Sorting**: QuickSort, MergeSort, HeapSort
- [ ] **Searching**: Binary Search, Graph traversals (DFS/BFS)
- [ ] **String**: KMP, Rabin-Karp pattern matching
- [ ] **Graph**: Dijkstra, Kruskal, Topological Sort
- [ ] **Dynamic Programming**: Common DP problems

## 💻 Development Environment

This project is developed on:
- **Device**: Samsung Galaxy Z Fold
- **OS**: Android (Termux)
- **Editor**: Neovim with LSP (clangd)
- **Compiler**: Clang/LLVM (ARM64)

Special setup for mobile development:
- Custom Neovim configuration with C++ LSP
- Integrated terminal for build/test workflow
- CMake-based modular build system

## 📝 Code Style

- **Naming**: snake_case for functions/variables, PascalCase for classes
- **Comments**: Doxygen-style documentation for all public APIs
- **Headers**: Include guards with project prefix
- **Formatting**: Consistent indentation and spacing
- **Namespace**: `mylib::linear`, `mylib::tree` for organization

## 🤝 Contributing

This is a personal learning project, but suggestions and feedback are welcome!

## 📄 License

MIT License - See LICENSE file for details

Copyright (c) 2025 Jinhyeok

## 🙏 Acknowledgments

- Built with modern C++ best practices
- Inspired by STL design principles
- Developed entirely on mobile (Galaxy Z Fold + Termux + Neovim)

---

**Note**: This is an educational project focused on understanding data structures from first principles. For production use, prefer well-tested libraries like STL.

## 📚 Learning Path

If you're following along to learn:

1. **Linear Structures**
   - Start with DynamicArray to understand memory management
   - Move to LinkedList to learn pointer manipulation
   - Study Stack/Queue to see container adapters

2. **Tree Structures**
   - Implement BST to understand tree concepts and recursion
   - Add AVL to learn self-balancing and rotations
   - Compare BST vs AVL performance with skewed data

3. **Testing**
   - Write tests for each operation
   - Cover edge cases thoroughly
   - Verify with stress tests

**Happy coding!** 🚀