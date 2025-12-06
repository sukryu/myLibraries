# Benchmarks

Performance benchmarks for data structures and algorithms.

## 📁 Structure

```
benchmarks/
├── utils/
│   └── benchmark_utils.hpp      # Benchmark utilities (Timer, DataGenerator, etc.)
├── tree/
│   └── balanced_tree_benchmark.cpp  # AVL vs Red-Black vs Skip List
├── algorithm/
│   └── sorting_benchmark.cpp    # Sorting algorithm comparisons
├── results/
│   └── *.md                     # Benchmark results and analysis
├── test_benchmark_utils.cpp     # Test benchmark utilities
└── CMakeLists.txt
```

## 🚀 Building

```bash
# From project root
mkdir -p build && cd build
cmake ..
cmake --build .

# Benchmarks will be in build/benchmarks/
```

## 🏃 Running

### Run utility tests
```bash
./benchmarks/test_benchmark_utils
```

### Run balanced tree benchmark
```bash
./benchmarks/benchmark_balanced_tree
```

### Run all benchmarks (if configured)
```bash
make run_all_benchmarks
```

## 📊 Available Benchmarks

### 1. Balanced Tree Benchmark
**Compares:** AVL Tree, Red-Black Tree, Skip List

**Operations tested:**
- Sequential insertion (1→N)
- Reverse insertion (N→1)
- Random insertion
- Search (hit/miss)
- Deletion
- Memory usage
- Tree height/structure

**Datasets:** 1K, 10K, 100K, 1M elements

**Output:** Comparison table with timing, memory, and throughput

### 2. Sorting Benchmark (Future)
**Compares:** QuickSort, MergeSort, HeapSort, InsertionSort

**Datasets:**
- Random data
- Nearly sorted data
- Reverse sorted data
- Many duplicates

### 3. Range Query Benchmark (Future)
**Compares:** Segment Tree vs Fenwick Tree

**Operations:**
- Build time
- Range query
- Point update
- Range update (with lazy propagation)

## 🛠️ Benchmark Utilities

### Timer
```cpp
Timer timer;
timer.start();
// ... code to benchmark ...
timer.stop();
std::cout << timer.elapsed_ms() << " ms" << std::endl;
```

### Data Generator
```cpp
DataGenerator<int> gen;
auto data = gen.shuffled(10000, 0);      // Random permutation
auto nearly = gen.nearly_sorted(10000);  // 90% sorted
```

### Benchmark Runner
```cpp
BenchmarkRunner runner;
runner.run_simple("Test", 1000, [&]() {
    // benchmark code
});
runner.print_summary();
```

## 📈 Results

Benchmark results are stored in `results/` directory as Markdown files with:
- Performance comparison tables
- Analysis and recommendations
- Use case guidelines

## 🎯 Adding New Benchmarks

1. Create benchmark file in appropriate directory:
   - `tree/` for tree data structures
   - `algorithm/` for algorithms
   
2. Use benchmark utilities from `utils/benchmark_utils.hpp`

3. Update `CMakeLists.txt` to add new executable

4. Document results in `results/`

Example structure:
```cpp
#include "benchmark_utils.hpp"
#include "tree/your_tree.hpp"

int main() {
    BenchmarkRunner runner;
    
    runner.run_simple("YourTest", 10000, [&]() {
        // benchmark code
    });
    
    runner.print_summary();
    return 0;
}
```

## 🔬 Guidelines

- Use consistent data sizes: 1K, 10K, 100K, 1M
- Run each benchmark multiple times and average
- Include both time and memory measurements
- Document system specs in results
- Use realistic usage patterns

## 📝 License

MIT License - See main project LICENSE for details.