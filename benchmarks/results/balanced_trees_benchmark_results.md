# Balanced Tree Benchmark Results

**Date:** 2025-12-06  
**Environment:** GitHub Codespaces  
**Compiler:** GCC (Ubuntu)  
**C++ Standard:** C++17  
**Build Type:** Release (-O2)

## Executive Summary

This benchmark compares three balanced search tree implementations:
- **AVL Tree**: Strictly balanced binary search tree with rotations
- **Red-Black Tree**: Loosely balanced tree with color-based balancing
- **Skip List**: Probabilistic balanced structure without rotations

**Key Findings:**
- ✅ **Red-Black Tree wins overall** - Best for insertions and deletions (1.3~2.3x faster)
- ✅ **AVL Tree best for searches** - Faster lookups (1.2~2.2x) due to minimal height
- ❌ **Skip List underperforms** - Simple implementation but slower (0.35~0.59x)

**Recommendation:** Use Red-Black Tree for general-purpose applications, AVL Tree for read-heavy workloads.

---

## Table of Contents

- [Test Configuration](#test-configuration)
- [Results by Dataset Size](#results-by-dataset-size)
  - [Small Dataset (1,000 elements)](#small-dataset-1000-elements)
  - [Medium Dataset (10,000 elements)](#medium-dataset-10000-elements)
  - [Large Dataset (100,000 elements)](#large-dataset-100000-elements)
- [Performance Analysis](#performance-analysis)
- [Tree Height Comparison](#tree-height-comparison)
- [Memory Usage](#memory-usage)
- [Conclusions](#conclusions)
- [Practical Guidelines](#practical-guidelines)

---

## Test Configuration

### Data Structures Tested

| Tree Type | Implementation | Balancing Method | Complexity |
|-----------|----------------|------------------|------------|
| AVL Tree | Custom | Rotations (strict) | O(log n) guaranteed |
| Red-Black Tree | Custom | Color rules + rotations | O(log n) guaranteed |
| Skip List | Custom | Probabilistic (p=0.5) | O(log n) average |

### Operations Benchmarked

1. **Insert Sequential** - Insert 1→N in order
2. **Insert Reverse** - Insert N→1 in reverse order
3. **Insert Random** - Insert shuffled data
4. **Search Hit** - Search for existing keys
5. **Search Miss** - Search for non-existing keys
6. **Delete** - Delete 50% of elements

### Dataset Sizes

- **Small:** 1,000 elements
- **Medium:** 10,000 elements  
- **Large:** 100,000 elements

### Metrics Collected

- Execution time (milliseconds)
- Memory usage (KB/MB)
- Throughput (operations/second)
- Tree height
- Relative speedup

---

## Results by Dataset Size

### Small Dataset (1,000 elements)

#### Insertion Performance

| Operation | AVL Tree | Red-Black Tree | Skip List | Winner |
|-----------|----------|----------------|-----------|--------|
| **Sequential** | 0.331 ms (baseline) | **0.274 ms (1.21x)** ⚡ | 0.887 ms (0.37x) | RB Tree |
| **Reverse** | 0.241 ms (baseline) | **0.204 ms (1.18x)** ⚡ | 0.743 ms (0.32x) | RB Tree |
| **Random** | 0.299 ms (baseline) | **0.233 ms (1.28x)** ⚡ | 0.903 ms (0.33x) | RB Tree |

**Analysis:**
- Red-Black Tree consistently 20-30% faster for insertions
- Skip List 2.5~3x slower than AVL/RB trees
- Pattern has minimal impact on relative performance

#### Search Performance

| Operation | AVL Tree | Red-Black Tree | Skip List | Winner |
|-----------|----------|----------------|-----------|--------|
| **Search Hit** | **0.079 ms** ✅ | 0.149 ms (0.53x) | 0.253 ms (0.31x) | AVL Tree |
| **Search Miss** | **0.026 ms** ✅✅ | 0.134 ms (0.20x) | 0.148 ms (0.18x) | AVL Tree |

**Analysis:**
- AVL Tree 2x faster for search hits, 5x faster for search misses
- Search Miss is extremely fast for AVL (early termination)
- Tree height advantage clearly visible

#### Deletion Performance

| Tree Type | Time | Relative Speed |
|-----------|------|----------------|
| AVL Tree | 0.214 ms (baseline) | 1.00x |
| **Red-Black Tree** | **0.082 ms** ⚡⚡ | **2.62x** |
| Skip List | 0.220 ms | 0.97x |

**Analysis:**
- Red-Black Tree 2.6x faster for deletions
- AVL and Skip List similar performance

#### Tree Heights (1,000 Random Inserts)

| Tree Type | Height | Expected (log₂ n ≈ 10) |
|-----------|--------|------------------------|
| AVL Tree | 12 | 1.20x optimal |
| Red-Black Tree | 12 | 1.20x optimal |
| **Skip List** | **8** ✅ | 0.80x optimal (lucky) |

---

### Medium Dataset (10,000 elements)

#### Insertion Performance

| Operation | AVL Tree | Red-Black Tree | Skip List | Winner |
|-----------|----------|----------------|-----------|--------|
| **Sequential** | 3.801 ms (baseline) | **1.835 ms (2.07x)** ⚡⚡ | 9.266 ms (0.41x) | RB Tree |
| **Reverse** | 4.534 ms (baseline) | **1.492 ms (3.04x)** ⚡⚡⚡ | 7.615 ms (0.60x) | RB Tree |
| **Random** | 3.982 ms (baseline) | **2.801 ms (1.42x)** ⚡ | 11.297 ms (0.35x) | RB Tree |

**Analysis:**
- Red-Black Tree dominates: **1.4~3.0x faster**
- Reverse pattern shows biggest advantage (3.04x)
- Skip List performance degrades (0.35~0.60x)

#### Search Performance

| Operation | AVL Tree | Red-Black Tree | Skip List | Winner |
|-----------|----------|----------------|-----------|--------|
| **Search Hit** | **1.195 ms** ✅ | 1.377 ms (0.87x) | 3.820 ms (0.31x) | AVL Tree |
| **Search Miss** | **0.336 ms** ✅✅ | 0.645 ms (0.52x) | 1.646 ms (0.20x) | AVL Tree |

**Analysis:**
- AVL maintains 13-50% advantage
- Gap widens for search misses
- Skip List 3-5x slower

#### Deletion Performance

| Tree Type | Time | Relative Speed |
|-----------|------|----------------|
| AVL Tree | 2.011 ms (baseline) | 1.00x |
| **Red-Black Tree** | **0.982 ms** ⚡⚡ | **2.05x** |
| Skip List | 2.700 ms | 0.74x |

#### Tree Heights (10,000 Random Inserts)

| Tree Type | Height | Expected (log₂ n ≈ 13.3) |
|-----------|--------|--------------------------|
| AVL Tree | 16 | 1.20x optimal |
| Red-Black Tree | 17 | 1.28x optimal |
| **Skip List** | **15** ✅ | 1.13x optimal |

---

### Large Dataset (100,000 elements)

#### Insertion Performance

| Operation | AVL Tree | Red-Black Tree | Skip List | Winner |
|-----------|----------|----------------|-----------|--------|
| **Sequential** | 52.863 ms (baseline) | **40.591 ms (1.30x)** ⚡ | 89.425 ms (0.59x) | RB Tree |
| **Reverse** | 40.132 ms (baseline) | **17.280 ms (2.32x)** ⚡⚡ | 79.998 ms (0.50x) | RB Tree |
| **Random** | 60.445 ms (baseline) | **34.252 ms (1.76x)** ⚡ | 121.754 ms (0.50x) | RB Tree |

**Analysis:**
- Red-Black Tree **1.3~2.3x faster** than AVL
- Sequential/Random: 30-76% faster
- Reverse: 2.3x faster (biggest advantage)
- Skip List: Consistently ~2x slower than AVL

#### Search Performance

| Operation | AVL Tree | Red-Black Tree | Skip List | Winner |
|-----------|----------|----------------|-----------|--------|
| **Search Hit** | **18.923 ms** ✅ | 22.775 ms (0.83x) | 56.028 ms (0.34x) | AVL Tree |
| **Search Miss** | **3.957 ms** ✅✅ | 8.769 ms (0.45x) | 29.710 ms (0.13x) | AVL Tree |

**Analysis:**
- AVL Tree 17-55% faster for searches
- Search Miss gap widening: 2.2x vs RB, 7.5x vs Skip List
- Skip List 3-7.5x slower than AVL

#### Deletion Performance

| Tree Type | Time | Relative Speed |
|-----------|------|----------------|
| AVL Tree | 29.232 ms (baseline) | 1.00x |
| **Red-Black Tree** | **13.781 ms** ⚡⚡ | **2.12x** |
| Skip List | 33.354 ms | 0.88x |

**Analysis:**
- Red-Black Tree maintains 2x advantage
- Skip List slightly slower than AVL

#### Tree Heights (100,000 Random Inserts)

| Tree Type | Height | Expected (log₂ n ≈ 16.6) |
|-----------|--------|--------------------------|
| AVL Tree | 20 | 1.20x optimal |
| Red-Black Tree | 20 | 1.20x optimal |
| **Skip List** | **15** ✅ | 0.90x optimal |

**Interesting:** Skip List achieves lower height but slower searches (cache effects)

---

## Performance Analysis

### Overall Performance Summary

#### Throughput Comparison (100,000 elements, ops/sec)

| Operation | AVL Tree | Red-Black Tree | Skip List | Best |
|-----------|----------|----------------|-----------|------|
| Insert Sequential | 1,891,679 | **2,463,606** ⚡ | 1,118,254 | RB |
| Insert Reverse | 2,491,789 | **5,787,042** ⚡⚡ | 1,250,038 | RB |
| Insert Random | 1,654,385 | **2,919,530** ⚡ | 821,328 | RB |
| Search Hit | **5,284,454** ✅ | 4,390,837 | 1,784,808 | AVL |
| Search Miss | **25,268,822** ✅✅ | 11,403,421 | 3,365,873 | AVL |
| Delete | 1,710,469 | **3,628,196** ⚡⚡ | 1,499,076 | RB |

### Performance by Operation Type

#### 🔴 Insertion (Winner: Red-Black Tree)

```
Red-Black Tree Performance Advantage:
Sequential:  1.21x → 2.07x → 1.30x  (avg: 1.53x)
Reverse:     1.18x → 3.04x → 2.32x  (avg: 2.18x)
Random:      1.28x → 1.42x → 1.76x  (avg: 1.49x)

Overall: 1.3~2.3x faster than AVL
```

**Why Red-Black Wins:**
- Fewer rotations due to looser balancing
- Color flips cheaper than structural changes
- Better worst-case behavior on sequential data

#### 🟢 Search (Winner: AVL Tree)

```
AVL Tree Performance Advantage:
Search Hit:  1.89x → 1.15x → 1.20x  (avg: 1.41x)
Search Miss: 5.00x → 1.92x → 2.22x  (avg: 3.05x)

Overall: 1.2~5.0x faster than Red-Black
```

**Why AVL Wins:**
- Strictly balanced → minimal height
- Fewer comparisons per search
- Better cache locality in smaller trees

#### 🔴 Deletion (Winner: Red-Black Tree)

```
Red-Black Tree Performance Advantage:
500:     2.62x faster
5,000:   2.05x faster
50,000:  2.12x faster

Overall: Consistently 2.0~2.6x faster
```

**Why Red-Black Wins:**
- Simpler rebalancing after deletion
- Fewer cascading rotations
- Color adjustments instead of height checks

---

## Tree Height Comparison

### Height Growth by Dataset Size

| Dataset Size | AVL Height | RB Height | Skip Height | Theoretical |
|--------------|------------|-----------|-------------|-------------|
| 1,000 (Random) | 12 | 12 | 8 | 10 (log₂) |
| 10,000 (Random) | 16 | 17 | 15 | 13.3 (log₂) |
| 100,000 (Random) | 20 | 20 | 15 | 16.6 (log₂) |

### Height vs Performance

```
Observation: Lower height ≠ Always faster searches!

Skip List has lowest height (15) but slowest searches (56 ms)
AVL/RB have same height (20) but AVL is 20% faster (18.9 vs 22.8 ms)

Factors beyond height:
- Cache locality
- Branch prediction
- Memory access patterns
- Node structure complexity
```

### Height Efficiency

| Tree Type | Height Formula | Efficiency | Notes |
|-----------|----------------|------------|-------|
| AVL | 1.44 log₂(n) | **Optimal** | Strictly balanced |
| Red-Black | 2.0 log₂(n) | Good | Up to 2x optimal |
| Skip List | ~log₁/ₚ(n) | Variable | Probabilistic (p=0.5) |

**Observed Height Ratios (100K elements):**
- AVL: 1.20x theoretical optimal ✅
- RB: 1.20x theoretical optimal ✅
- Skip: 0.90x theoretical optimal ✅ (lucky run)

---

## Memory Usage

### Memory Consumption (100,000 elements)

| Tree Type | Memory | Overhead | Per Node |
|-----------|--------|----------|----------|
| AVL Tree | 2.67 MB | Baseline | ~27 bytes |
| Red-Black Tree | 2.67 MB | Same | ~27 bytes |
| **Skip List** | **8.78 MB** | **3.3x** ⚠️ | ~88 bytes |

### Memory Breakdown

#### AVL Tree Node
```cpp
struct AVLNode {
    int value;        // 4 bytes
    int height;       // 4 bytes
    AVLNode* left;    // 8 bytes
    AVLNode* right;   // 8 bytes
    // Total: 24 bytes + padding ≈ 27 bytes
};
```

#### Red-Black Tree Node
```cpp
struct RBNode {
    int value;        // 4 bytes
    Color color;      // 4 bytes (enum)
    RBNode* left;     // 8 bytes
    RBNode* right;    // 8 bytes
    RBNode* parent;   // 8 bytes
    // Total: 32 bytes (but similar actual usage)
};
```

#### Skip List Node (Average)
```cpp
struct SkipNode {
    int value;              // 4 bytes
    std::vector<SkipNode*>  // 24 bytes (vector overhead)
    + forward pointers      // ~avg 3 levels × 8 bytes = 24 bytes
    // Total: 52+ bytes per node
    // Plus vector allocations: ~88 bytes average
};
```

**Analysis:**
- Skip List uses **3.3x more memory** than AVL/RB
- Caused by variable-length forward pointer arrays
- Each vector allocation has overhead
- Trade-off for simpler implementation

---

## Conclusions

### Overall Winner: **Red-Black Tree** 🏆

| Criteria | AVL | Red-Black | Skip List |
|----------|-----|-----------|-----------|
| **Insertion** | ⭐⭐ | ⭐⭐⭐⭐⭐ | ⭐ |
| **Search** | ⭐⭐⭐⭐⭐ | ⭐⭐⭐⭐ | ⭐⭐ |
| **Deletion** | ⭐⭐ | ⭐⭐⭐⭐⭐ | ⭐⭐ |
| **Memory** | ⭐⭐⭐⭐⭐ | ⭐⭐⭐⭐⭐ | ⭐⭐ |
| **Implementation** | ⭐⭐⭐ | ⭐ | ⭐⭐⭐⭐⭐ |
| **Overall** | ⭐⭐⭐⭐ | ⭐⭐⭐⭐⭐ | ⭐⭐ |

### Key Takeaways

1. **Red-Black Tree is the best general-purpose choice**
   - 1.3~2.3x faster insertions
   - 2.0~2.6x faster deletions
   - Only 17-55% slower searches than AVL
   - Same memory footprint as AVL

2. **AVL Tree excels at read-heavy workloads**
   - 1.2~2.2x faster searches (hit)
   - 2.2~5.0x faster searches (miss)
   - Best when lookups >> insertions/deletions

3. **Skip List underperforms in single-threaded scenarios**
   - 2-3x slower insertions
   - 3-7x slower searches
   - 3.3x more memory
   - Advantage: Simple implementation, good for concurrency

### Why These Results?

#### Red-Black Tree Advantages
✅ **Looser balancing** = fewer rotations  
✅ **Color flips** cheaper than height adjustments  
✅ **Better worst-case** on sequential data  
✅ **Optimized rebalancing** after deletions  

#### AVL Tree Advantages
✅ **Minimal height** = fewer comparisons  
✅ **Strict balancing** = optimal search depth  
✅ **Better cache locality** in smaller trees  

#### Skip List Disadvantages (This Implementation)
⚠️ **Memory allocation overhead** from std::vector  
⚠️ **Poor cache locality** (scattered forward pointers)  
⚠️ **No compiler optimizations** for probabilistic structures  
⚠️ **Dynamic memory** for each level

### Unexpected Findings

1. **Height doesn't always predict performance**
   - Skip List: Lowest height (15) but slowest searches
   - Cache effects and memory patterns matter more

2. **Red-Black Tree faster than expected**
   - Theory suggests similar to AVL
   - Practice shows 1.5-3x advantage for modifications

3. **Sequential/Reverse patterns hurt AVL more than RB**
   - AVL requires more rotations on pathological cases
   - RB's looser balancing helps with worst-case data

---

## Practical Guidelines

### Decision Tree

```
Need a balanced search tree?
│
├─ Mostly searches (90%+)?
│  └─ Use AVL Tree ✅
│     - 1.2~2.2x faster lookups
│     - Best for: caches, dictionaries, read-only datasets
│
├─ Balanced read/write?
│  └─ Use Red-Black Tree ✅ (DEFAULT CHOICE)
│     - 1.5~2.5x faster insertions/deletions
│     - Only 20% slower searches than AVL
│     - Best for: general-purpose, std::map replacement
│
├─ Multi-threaded / Concurrent?
│  └─ Use Skip List ✅
│     - Lock-free implementations possible
│     - Best for: concurrent maps, distributed systems
│
└─ Simple implementation needed?
   └─ Use Skip List ✅
      - ~200 lines vs ~500+ for RB/AVL
      - Best for: prototypes, learning, simple use cases
```

### Use Case Examples

#### AVL Tree ✅
```cpp
// Read-heavy: Configuration cache
AVLTree<std::string, Config> configCache;

// Frequent lookups, rare updates
if (auto* config = configCache.find("database.url")) {
    // Use config
}

// Best for: 90%+ reads, 10%- writes
```

#### Red-Black Tree ✅✅ (Recommended)
```cpp
// General-purpose ordered map
RedBlackTree<int, User> users;

// Balanced read/write workload
users.insert(123, newUser);
auto* user = users.find(123);
users.erase(123);

// Best for: std::map replacement, general use
```

#### Skip List
```cpp
// Concurrent access (with locks or lock-free)
SkipList<int> sharedData;

// Or simple prototype
SkipList<std::string> quickMap;

// Best for: learning, prototypes, concurrency
```

### Performance Expectations

| Workload Pattern | Recommended | Expected Performance |
|------------------|-------------|----------------------|
| 90% Search, 10% Insert/Delete | AVL Tree | 1.2~2x faster searches |
| 50% Search, 50% Insert/Delete | Red-Black Tree | 1.5~2x faster overall |
| 10% Search, 90% Insert/Delete | Red-Black Tree | 2~2.5x faster modifications |
| Heavy concurrent access | Skip List | Variable (lock-free benefits) |
| Limited memory | AVL/RB Tree | 3x less memory than Skip List |

### Migration Guide

#### From std::map (Red-Black)
```cpp
// std::map uses Red-Black Tree internally
std::map<int, std::string> stdMap;

// Direct replacement
RedBlackTree<int, std::string> customMap;

// Performance: Similar or slightly faster
// API: Need adapter layer for full compatibility
```

#### Optimization Tips

**For AVL Tree:**
- Batch insertions then freeze for lookups
- Pre-sort data if building from scratch
- Use for static/semi-static data

**For Red-Black Tree:**
- Default choice for dynamic data
- Optimal for mixed workloads
- Industry-standard algorithm

**For Skip List:**
- Tune probability (try p=0.25 for less memory)
- Implement memory pool for node allocation
- Use in concurrent scenarios
- Consider lock-free implementation

---

## Benchmark Reproduction

### Environment Setup

```bash
git clone <repo>
cd myLibraries
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

### Run Benchmark

```bash
./benchmarks/benchmark_balanced_tree
```

### Expected Runtime

- Small (1K): ~1 second
- Medium (10K): ~5 seconds
- Large (100K): ~30 seconds
- **Total: ~36 seconds**

### Modify Dataset Sizes

Edit `benchmarks/tree/balanced_tree_benchmark.cpp`:

```cpp
const std::vector<std::size_t> DATASET_SIZES = {
    1000,       // Small
    10000,      // Medium
    100000,     // Large
    1000000     // XLarge (optional, takes ~5 minutes)
};
```

---

## Future Work

### Skip List Optimization

**Identified Issues:**
1. Memory allocation overhead from `std::vector`
2. Poor cache locality
3. No memory pooling

**Proposed Improvements:**
```cpp
// Option 1: Adjust probability
SkipList<int> list(16, 0.25);  // Less memory, fewer levels

// Option 2: Memory pool
class SkipListWithPool {
    MemoryPool<SkipNode> pool;
    // Pre-allocate nodes
};

// Option 3: Fixed-size forward arrays
struct SkipNode {
    std::array<SkipNode*, 16> forward;  // Instead of vector
};
```

**Expected improvements:** 1.5~2x speedup, 2x less memory

### Additional Benchmarks

1. **Range Query Trees**
   - Segment Tree vs Fenwick Tree
   - Different query types
   - Update patterns

2. **Sorting Algorithms**
   - QuickSort vs MergeSort vs HeapSort
   - Different data patterns
   - Pivot selection strategies

3. **Real-World Workloads**
   - Database index simulation
   - LRU cache implementation
   - File system directory structure

### Performance Profiling

```bash
# CPU profiling
perf record ./benchmark_balanced_tree
perf report

# Cache analysis
perf stat -e cache-references,cache-misses ./benchmark_balanced_tree

# Memory profiling
valgrind --tool=massif ./benchmark_balanced_tree
```

---

## References

### Implementations

- AVL Tree: `include/tree/avl_tree.hpp`
- Red-Black Tree: `include/tree/red_black_tree.hpp`
- Skip List: `include/tree/skip_list.hpp`
- Benchmark Code: `benchmarks/tree/balanced_tree_benchmark.cpp`

### Theoretical Complexity

| Operation | AVL | Red-Black | Skip List |
|-----------|-----|-----------|-----------|
| Search | O(log n) | O(log n) | O(log n) avg |
| Insert | O(log n) | O(log n) | O(log n) avg |
| Delete | O(log n) | O(log n) | O(log n) avg |
| Space | O(n) | O(n) | O(n) avg |

### Classic Papers

- AVL Tree: Adelson-Velsky and Landis (1962)
- Red-Black Tree: Guibas and Sedgewick (1978)
- Skip List: William Pugh (1990)

---

## Appendix: Raw Data

### Complete Results Table (100,000 elements)

| Operation | AVL Time | RB Time | Skip Time | AVL Memory | RB Memory | Skip Memory |
|-----------|----------|---------|-----------|------------|-----------|-------------|
| Insert Seq | 52.863 ms | 40.591 ms | 89.425 ms | 2.67 MB | 2.67 MB | 8.78 MB |
| Insert Rev | 40.132 ms | 17.280 ms | 79.998 ms | 2.67 MB | 2.67 MB | 8.78 MB |
| Insert Rnd | 60.445 ms | 34.252 ms | 121.754 ms | 2.67 MB | 2.67 MB | 8.78 MB |
| Search Hit | 18.923 ms | 22.775 ms | 56.028 ms | 2.67 MB | 2.67 MB | 8.78 MB |
| Search Miss | 3.957 ms | 8.769 ms | 29.710 ms | 2.67 MB | 2.67 MB | 8.78 MB |
| Delete | 29.232 ms | 13.781 ms | 33.354 ms | 1.34 MB | 1.34 MB | 4.39 MB |

### Tree Heights Table

| Dataset | Pattern | AVL | RB | Skip |
|---------|---------|-----|----|----|
| 1K | Sequential | 10 | 17 | 9 |
| 1K | Reverse | 10 | 17 | 10 |
| 1K | Random | 12 | 12 | 8 |
| 10K | Sequential | 14 | 24 | 12 |
| 10K | Reverse | 14 | 24 | 13 |
| 10K | Random | 16 | 17 | 15 |
| 100K | Sequential | 17 | 31 | 16 |
| 100K | Reverse | 17 | 31 | 16 |
| 100K | Random | 20 | 20 | 15 |

---

**Document Version:** 1.0  
**Last Updated:** 2025-12-06  
**Author:** Jinhyeok  
**License:** MIT