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

## 🎯 Quick Recommendations

| Use Case | Recommended Tree | Reason |
|----------|------------------|--------|
| **General-purpose** | Red-Black Tree | Balanced performance |
| **Read-heavy (90%+ searches)** | AVL Tree | Minimal height, faster lookups |
| **Write-heavy (frequent inserts/deletes)** | Red-Black Tree | 2x faster modifications |
| **Concurrent access** | Skip List | Lock-free possible |
| **Simple implementation** | Skip List | ~200 lines vs 500+ |
| **Limited memory** | AVL/Red-Black | 3x less than Skip List |

---

## 📈 Performance Summary (100,000 elements)

### Insertion (Winner: Red-Black Tree ⚡)
- Sequential: **40.6 ms** (1.30x faster than AVL)
- Reverse: **17.3 ms** (2.32x faster)
- Random: **34.3 ms** (1.76x faster)

### Search (Winner: AVL Tree ✅)
- Search Hit: **18.9 ms** (1.20x faster than RB)
- Search Miss: **4.0 ms** (2.22x faster)

### Deletion (Winner: Red-Black Tree ⚡)
- 50K deletes: **13.8 ms** (2.12x faster than AVL)

### Memory Usage
- AVL: 2.67 MB
- Red-Black: 2.67 MB (same)
- Skip List: 8.78 MB (3.3x more ⚠️)

---

## 🔬 Test Environment

- **Platform:** GitHub Codespaces
- **Compiler:** GCC (Ubuntu)
- **Optimization:** -O2 (Release)
- **C++ Standard:** C++17
- **Date:** 2025-12-06

---

## 📝 How to Run Benchmarks

```bash
cd /workspaces/myLibraries/build
./benchmarks/benchmark_balanced_tree
```

Results will be printed to console. Takes ~36 seconds for all datasets (1K, 10K, 100K).

---

## 📂 File Structure

```
benchmarks/results/
├── README.md                              # This file
├── balanced_trees_benchmark_results.md    # Detailed analysis
└── (future benchmark results...)
```

---

## 🎯 Coming Soon

- Sorting algorithms benchmark (QuickSort vs MergeSort vs HeapSort)
- Range query benchmark (Segment Tree vs Fenwick Tree)
- Real-world workload simulations
- Skip List optimization study

---

For detailed analysis, see [balanced_trees_benchmark_results.md](balanced_trees_benchmark_results.md).