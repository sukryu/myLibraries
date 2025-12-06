# Benchmark Results

This directory contains detailed performance analysis and benchmark results for data structures and algorithms.

## 📊 Available Results

### [Balanced Trees Benchmark](balanced_trees_benchmark_results.md)

**Compared:** AVL Tree vs Red-Black Tree vs Skip List

**Key Findings:**
- ✅ **Red-Black Tree wins overall** (1.3~2.3x faster insertions, 2.0~2.6x faster deletions)
- ✅ **AVL Tree best for searches** (1.2~2.2x faster)
- ❌ **Skip List underperforms** (simple but slow)

**Recommendation:** Use Red-Black Tree for general-purpose, AVL for read-heavy workloads.

---

### [Sorting Algorithms Benchmark](sorting_benchmark_results.md)

**Compared:** QuickSort vs MergeSort vs HeapSort vs InsertionSort vs std::sort

**Key Findings:**
- 🏆 **std::sort dominates everything** (3~16x faster than custom implementations!)
- 🥈 **MergeSort beats QuickSort** (1.4~1.8x faster - unexpected!)
- 🥉 **QuickSort disappoints** (poor on reverse data despite median-of-three)
- 💥 **HeapSort slowest** (catastrophic with duplicates: 240ms!)
- 🐌 **InsertionSort** (not competitive even for n=100)

**Recommendation:** **Always use std::sort** for production code. Custom implementations cannot compete.

---

## 🎯 Quick Recommendations

### Data Structures

| Use Case | Recommended Tree | Reason |
|----------|------------------|--------|
| **General-purpose** | Red-Black Tree | Balanced performance |
| **Read-heavy (90%+ searches)** | AVL Tree | Minimal height, faster lookups |
| **Write-heavy (frequent inserts/deletes)** | Red-Black Tree | 2x faster modifications |
| **Concurrent access** | Skip List | Lock-free possible |
| **Simple implementation** | Skip List | ~200 lines vs 500+ |
| **Limited memory** | AVL/Red-Black | 3x less than Skip List |

### Sorting Algorithms

| Use Case | Recommended | Reason |
|----------|-------------|--------|
| **Everything** | std::sort | 3~16x faster than custom |
| **Need stability** | std::stable_sort | Optimized, preserves order |
| **Top-k elements** | std::partial_sort | O(n + k log k) |
| **Find median** | std::nth_element | O(n) average |
| **Learning** | Custom implementations | Educational only |

---

## 📈 Performance Summary

### Balanced Trees (100,000 elements)

#### Insertion (Winner: Red-Black Tree ⚡)
- Sequential: **40.6 ms** (1.30x faster than AVL)
- Reverse: **17.3 ms** (2.32x faster)
- Random: **34.3 ms** (1.76x faster)

#### Search (Winner: AVL Tree ✅)
- Search Hit: **18.9 ms** (1.20x faster than RB)
- Search Miss: **4.0 ms** (2.22x faster)

#### Deletion (Winner: Red-Black Tree ⚡)
- 50K deletes: **13.8 ms** (2.12x faster than AVL)

#### Memory Usage
- AVL: 2.67 MB
- Red-Black: 2.67 MB (same)
- Skip List: 8.78 MB (3.3x more ⚠️)

---

### Sorting Algorithms (100,000 elements)

#### Random Data
- **std::sort: 28.7 ms** (4.8x faster than QuickSort) ⚡⚡⚡
- MergeSort: 98.3 ms (1.4x faster than QuickSort)
- QuickSort: 138.4 ms (baseline)
- HeapSort: 188.2 ms (slowest)

#### Nearly Sorted (90%)
- **std::sort: 17.2 ms** (6.9x faster than QuickSort) ⚡⚡⚡⚡
- MergeSort: 84.3 ms
- QuickSort: 118.2 ms

#### Reverse Sorted
- **std::sort: 11.5 ms** (14.3x faster than QuickSort!) ⚡⚡⚡⚡⚡
- MergeSort: 91.5 ms (1.8x faster than QuickSort)
- QuickSort: 165.2 ms (worst case ⚠️)
- HeapSort: 180.4 ms

#### Many Duplicates
- **std::sort: 28.7 ms** (3.3x faster)
- QuickSort: 94.4 ms
- MergeSort: 100.6 ms
- HeapSort: 240.2 ms (catastrophic! ⚠️)

---

## 🔬 Test Environment

- **Platform:** GitHub Codespaces
- **Compiler:** GCC (Ubuntu)
- **Optimization:** -O2 (Release)
- **C++ Standard:** C++17
- **Date:** 2025-12-06

---

## 📝 How to Run Benchmarks

### Balanced Trees
```bash
cd /workspaces/myLibraries/build
./benchmarks/benchmark_balanced_tree
```
Takes ~36 seconds (1K, 10K, 100K datasets)

### Sorting Algorithms
```bash
cd /workspaces/myLibraries/build
./benchmarks/benchmark_sorting
```
Takes ~2.5 minutes (100, 1K, 10K, 100K datasets)

---

## 📂 File Structure

```
benchmarks/results/
├── README.md                              # This file
├── balanced_trees_benchmark_results.md    # Detailed tree analysis
├── sorting_benchmark_results.md           # Detailed sorting analysis
└── (future benchmark results...)
```

---

## 🎯 Coming Soon

- Range query benchmark (Segment Tree vs Fenwick Tree)
- Graph algorithms benchmark (DFS vs BFS vs Dijkstra)
- Hash table benchmark (Chaining vs Open Addressing)
- Skip List optimization study

---

## 💡 Key Lessons Learned

### From Balanced Trees:
1. **Red-Black Tree is the best general-purpose choice**
2. Height doesn't always predict performance (Skip List paradox)
3. Memory access patterns matter as much as algorithms
4. Industry chose Red-Black for good reasons (std::map)

### From Sorting:
1. **Never implement your own sort for production** - std::sort is untouchable
2. MergeSort's sequential access beats QuickSort's clever partitioning
3. HeapSort is impractical despite O(n log n) guarantee
4. Big-O notation doesn't tell the whole story - constants and cache matter

---

For detailed analysis, see individual result files.