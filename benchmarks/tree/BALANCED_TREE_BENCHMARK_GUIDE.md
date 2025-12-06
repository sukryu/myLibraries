# Balanced Tree Benchmark - Installation Guide

## 📋 Quick Setup

### Step 1: Copy the benchmark file
```bash
cd /workspaces/myLibraries

# Copy benchmark file to tree directory
cp /mnt/user-data/outputs/balanced_tree_benchmark.cpp benchmarks/tree/
```

### Step 2: Rebuild the project
```bash
cd build

# Reconfigure CMake to detect new benchmark
cmake ..

# Build
cmake --build .
```

You should see:
```
-- Added benchmark: balanced_tree
```

### Step 3: Run the benchmark
```bash
# From build directory
./benchmarks/benchmark_balanced_tree
```

## 🎯 What the Benchmark Does

### Measured Operations

1. **Insert Sequential** (1→N)
   - Tests worst-case for naive BST
   - Best case for some implementations

2. **Insert Reverse** (N→1)
   - Opposite worst-case pattern
   - Tests balancing robustness

3. **Insert Random**
   - Most realistic scenario
   - Tests average-case performance

4. **Search Hit**
   - Search for existing elements
   - Tests lookup efficiency

5. **Search Miss**
   - Search for non-existing elements
   - Tests early termination

6. **Delete**
   - Delete 50% of elements
   - Tests removal efficiency

### Data Sizes
- **Small**: 1,000 elements
- **Medium**: 10,000 elements
- **Large**: 100,000 elements

### Output Format

For each operation, you'll see:
```
========================================
  Sequential
========================================

Benchmark                             Size           Time         Memory        Throughput      Relative Speed
--------------------------------------------------------------------------------------------------------------
AVL Tree (Sequential)                 10000         12.345 ms       156.25 KB          810000 ops/s          (baseline)
Red-Black Tree (Sequential)           10000         10.234 ms       156.25 KB          977000 ops/s                1.21x
Skip List (Sequential)                10000          8.456 ms       180.00 KB         1183000 ops/s                1.46x

Tree Heights:
  AVL Tree (Sequential)              : 13
  Red-Black Tree (Sequential)        : 17
  Skip List (Sequential)             : 15
```

## 📊 Expected Results

### Performance Characteristics

**AVL Tree:**
- ✅ Fastest searches (minimal height)
- ⚠️ Slower insertions (more rotations)
- ⚠️ Slower deletions (strict balancing)
- Height: ~1.44 log₂(n)

**Red-Black Tree:**
- ✅ Balanced performance
- ✅ Fewer rotations than AVL
- ✅ Industry standard (std::map)
- Height: ~2 log₂(n)

**Skip List:**
- ✅ Fastest insertions
- ✅ Simplest implementation
- ⚠️ Probabilistic guarantees
- Height: Variable (probabilistic)

### When to Use Each

| Use Case | Recommended | Reason |
|----------|-------------|--------|
| Read-heavy (90%+ searches) | AVL Tree | Minimal height = fastest searches |
| Balanced workload | Red-Black Tree | Consistent performance |
| Write-heavy (frequent inserts/deletes) | Skip List | Faster modifications |
| Simple implementation needed | Skip List | No rotations, easy to understand |
| Concurrent access | Skip List | Lock-free implementations easier |
| Standard library compatibility | Red-Black Tree | Used in std::map, std::set |

## 🔍 Analyzing Results

### Look for:

1. **Relative Speed**
   - Which tree is fastest for each operation?
   - How much faster? (1.5x? 2x?)

2. **Tree Heights**
   - AVL should have smallest height
   - RB should be ~1.5x AVL height
   - Skip List varies each run

3. **Memory Usage**
   - Skip List may use more memory (forward pointers)
   - AVL/RB similar memory usage

4. **Consistency**
   - Run benchmark multiple times
   - Check for variance in results

### Sample Analysis

If you see:
```
Insert Random:
- AVL:      15 ms
- RB:       12 ms  (1.25x faster)
- Skip:     10 ms  (1.50x faster)

Search Hit:
- AVL:       8 ms  (baseline)
- RB:       10 ms  (0.80x)
- Skip:     11 ms  (0.73x)
```

**Conclusion:** Skip List wins for insertions, AVL wins for searches.
**Recommendation:** Use AVL for read-heavy, Skip List for write-heavy.

## 🐛 Troubleshooting

### Benchmark not found after build
```bash
# Check if file exists
ls -la benchmarks/tree/balanced_tree_benchmark.cpp

# Reconfigure CMake
cd build
rm -rf *
cmake ..
make
```

### Compilation errors
```bash
# Check include paths
cd build
cmake .. -DCMAKE_VERBOSE_MAKEFILE=ON
make benchmark_balanced_tree
```

### Slow performance
```bash
# Build with optimizations
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

### Out of memory
```bash
# Reduce dataset sizes in balanced_tree_benchmark.cpp
# Change DATASET_SIZES to:
const std::vector<std::size_t> DATASET_SIZES = {
    1000,      // Small
    10000      // Medium only
};
```

## 📈 Advanced Usage

### Save results to file
```bash
./benchmarks/benchmark_balanced_tree > results/balanced_trees_$(date +%Y%m%d).txt
```

### Compare with different configurations
```bash
# Run with different optimization levels
cmake .. -DCMAKE_BUILD_TYPE=Debug
./benchmarks/benchmark_balanced_tree > debug_results.txt

cmake .. -DCMAKE_BUILD_TYPE=Release
./benchmarks/benchmark_balanced_tree > release_results.txt

diff debug_results.txt release_results.txt
```

### Profile with perf (if available)
```bash
perf record ./benchmarks/benchmark_balanced_tree
perf report
```

## 🎯 Next Steps

After running this benchmark:

1. ✅ Understand trade-offs between implementations
2. ✅ Document results in `benchmarks/results/`
3. ✅ Update main README with findings
4. 🎯 Create more benchmarks (sorting, range queries)
5. 🎯 Add visualization (optional)

---

Ready to run? Execute:
```bash
cd /workspaces/myLibraries/build
./benchmarks/benchmark_balanced_tree
```