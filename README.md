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
├── include/               # Header files
│   └── linear/           # Linear data structures
│       ├── dynamic_array.hpp
│       ├── stack.hpp
│       ├── linked_list.hpp
│       ├── queue.hpp
│       └── deque.hpp
├── src/                  # Implementation files
│   └── linear/
│       ├── dynamic_array.cpp
│       ├── stack.cpp
│       ├── linked_list.cpp
│       ├── queue.cpp
│       └── deque.cpp
├── tests/                # Comprehensive test suites
│   ├── test_dynamic_array.cpp
│   ├── test_stack.cpp
│   ├── test_linked_list.cpp
│   ├── test_queue.cpp
│   └── test_deque.cpp
├── build/                # Build artifacts (gitignored)
├── CMakeLists.txt        # CMake build configuration
└── README.md            # This file
```

## ✅ Implemented Data Structures

### Linear Data Structures

| Data Structure | Description | Key Operations | Time Complexity |
|---------------|-------------|----------------|-----------------|
| **DynamicArray** | Auto-resizing array with capacity management | push_back, pop_back, operator[] | O(1) amortized |
| **Stack** | LIFO (Last-In-First-Out) container | push, pop, top | O(1) |
| **LinkedList** | Doubly linked list with bidirectional traversal | push_front/back, insert, erase | O(1) ends, O(n) middle |
| **Queue** | FIFO (First-In-First-Out) container | push, pop, front | O(1) |
| **Deque** | Double-ended queue supporting both ends | push_front/back, pop_front/back | O(1) |

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
./test_dynamic_array
./test_stack
./test_linked_list
./test_queue
./test_deque
```

### Quick Example

```cpp
#include "linear/stack.hpp"
#include "linear/queue.hpp"
#include <iostream>

using namespace mylib::linear;

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
    
    // DynamicArray example
    DynamicArray<int> arr = {1, 2, 3, 4, 5};
    arr.push_back(6);
    std::cout << arr.size() << std::endl;  // 6
    
    return 0;
}
```

## 📊 Implementation Details

### DynamicArray
- **Internal Storage**: Raw array with manual memory management
- **Growth Strategy**: 2x capacity when full
- **Memory**: Contiguous allocation for cache efficiency

### Stack
- **Container Adapter**: Uses DynamicArray internally
- **Operations**: All O(1) time complexity
- **Use Cases**: Expression evaluation, undo/redo, backtracking

### LinkedList
- **Structure**: Doubly linked nodes with prev/next pointers
- **Optimization**: Bidirectional traversal from nearest end
- **Memory**: Dynamic allocation per node

### Queue
- **Container Adapter**: Uses LinkedList internally
- **Operations**: O(1) enqueue and dequeue
- **Use Cases**: BFS, task scheduling, buffering

### Deque
- **Container Adapter**: Uses LinkedList internally
- **Flexibility**: Combines Stack and Queue operations
- **Use Cases**: Sliding window, palindrome checking

## 🧪 Testing

Each data structure has a comprehensive test suite covering:

- ✅ Constructor variations (default, copy, move, initializer list)
- ✅ Basic operations (insert, delete, access)
- ✅ Edge cases (empty container, single element, full capacity)
- ✅ Exception handling (out of range, empty container)
- ✅ Memory management (no leaks, proper cleanup)
- ✅ Large datasets (stress testing with 1000-10000 elements)
- ✅ Different data types (int, double, custom types)

### Test Results

All tests passing:
```
✓ DynamicArray: 21/21 tests passed
✓ Stack: 22/22 tests passed
✓ LinkedList: 33/33 tests passed
✓ Queue: 26/26 tests passed
✓ Deque: 35/35 tests passed
```

## 🔮 Future Plans

### Upcoming Data Structures
- [ ] **Tree**: Binary Search Tree (BST)
- [ ] **AVL Tree**: Self-balancing BST
- [ ] **Heap**: Priority Queue implementation
- [ ] **Hash Table**: Hash map with collision handling
- [ ] **Graph**: Adjacency list/matrix representations

### Planned Algorithms
- [ ] **Sorting**: QuickSort, MergeSort, HeapSort
- [ ] **Searching**: Binary Search, Graph traversals (DFS/BFS)
- [ ] **String**: KMP, Rabin-Karp pattern matching
- [ ] **Graph**: Dijkstra, Kruskal, Topological Sort
- [ ] **Practical**: LRU Cache, Expression Evaluator

## 💻 Development Environment

This project was developed on:
- **Device**: Samsung Galaxy Z Fold7
- **OS**: Android (Termux)
- **Editor**: Neovim with LSP (clangd)
- **Compiler**: Clang/LLVM (ARM64)

Special setup for mobile development:
- Custom Neovim configuration with C++ LSP
- Integrated terminal for build/test workflow
- CMake-based build system optimized for Termux

## 📝 Code Style

- **Naming**: snake_case for functions/variables, PascalCase for classes
- **Comments**: Doxygen-style documentation for all public APIs
- **Headers**: Include guards with project prefix
- **Formatting**: Consistent indentation and spacing
- **Namespace**: `mylib::linear` for organization

## 🤝 Contributing

This is a personal learning project, but suggestions and feedback are welcome!

## 📄 License

MIT License - See LICENSE file for details

Copyright (c) 2025 Your Name

## 🙏 Acknowledgments

- Built with modern C++ best practices
- Inspired by STL design principles
- Developed entirely on mobile (Galaxy Z Fold7 + Termux + Neovim)

---

**Note**: This is an educational project focused on understanding data structures from first principles. For production use, prefer well-tested libraries like STL.

## 📚 Learning Resources

If you're following along to learn:
1. Start with DynamicArray to understand memory management
2. Move to LinkedList to learn pointer manipulation
3. Study Stack/Queue to see container adapters
4. Implement Deque to combine concepts
5. Test thoroughly to verify correctness

**Happy coding!** 🚀