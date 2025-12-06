# Sorting Algorithms Benchmark - Setup Guide

## 🚀 Quick Setup

### Step 1: Copy the benchmark file
```bash
cd /workspaces/myLibraries

# Create algorithm directory if needed
mkdir -p benchmarks/algorithm

# Copy benchmark file
cp /mnt/user-data/outputs/sorting_benchmark.cpp benchmarks/algorithm/
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
-- Added benchmark: sorting
```

### Step 3: Run the benchmark
```bash
# From build directory
./benchmarks/benchmark_sorting
```

---

## 🎯 What the Benchmark Does

### Algorithms Tested

1. **QuickSort**
   - Median-of-three pivot selection
   - Tail recursion optimization
   - In-place sorting

2. **MergeSort**
   - Stable sorting
   - Divide and conquer
   - O(n) auxiliary space

3. **HeapSort**
   - In-place sorting
   - Binary heap based
   - O(1) extra space

4. **InsertionSort**
   - Simple comparison-based
   - Efficient for small arrays
   - Adaptive (fast on nearly sorted)

5. **std::sort** (Baseline)
   - IntroSort (QuickSort + HeapSort)
   - Industry standard
   - Optimized by compiler vendors

### Test Patterns

1. **Random Data**
   - Shuffled array
   - Typical use case

2. **Nearly Sorted (90%)**
   - 10% elements shuffled
   - Tests adaptive behavior

3. **Reverse Sorted**
   - Worst case for naive QuickSort
   - Tests pivot selection

4. **Many Duplicates (10% unique)**
   - Repeated values
   - Tests handling of equal elements

5. **Small Arrays**
   - 10, 50, 100, 500 elements
   - Where InsertionSort shines

### Dataset Sizes
- **Tiny**: 100 elements
- **Small**: 1,000 elements
- **Medium**: 10,000 elements
- **Large**: 100,000 elements

---

## 📊 Expected Results

### Performance Predictions

#### Random Data (100,000 elements)
```
QuickSort:    ~10-15 ms  (fastest on average)
MergeSort:    ~15-20 ms  (stable, predictable)
HeapSort:     ~20-25 ms  (slower but in-place)
std::sort:    ~8-12 ms   (highly optimized)
```

#### Nearly Sorted (100,000 elements)
```
QuickSort:    ~5-8 ms    (good with median-of-three)
MergeSort:    ~15-18 ms  (doesn't adapt)
HeapSort:     ~20-25 ms  (doesn't adapt)
InsertionSort: N/A      (too slow for 100K)
```

#### Small Arrays (100 elements)
```
QuickSort:     ~0.01 ms
MergeSort:     ~0.02 ms
HeapSort:      ~0.02 ms
InsertionSort: ~0.005 ms  (fastest!)
```

### When Each Algorithm Wins

| Scenario | Winner | Reason |
|----------|--------|--------|
| **Random data** | QuickSort / std::sort | Best average case |
| **Nearly sorted** | InsertionSort (small) | Adaptive O(n) |
| **Reverse sorted** | MergeSort | No worst case |
| **Many duplicates** | QuickSort | Good pivot handling |
| **Small arrays (n<50)** | InsertionSort | Low overhead |
| **Need stability** | MergeSort | Preserves order |
| **Memory limited** | HeapSort | In-place |
| **Production use** | std::sort | Highly optimized |

---

## 🔍 Reading the Results

### Sample Output

```
========================================
  Random Data
========================================

Benchmark                             Size           Time         Memory        Throughput      Relative Speed
--------------------------------------------------------------------------------------------------------------
QuickSort - Random                  100000         12.345 ms        390.62 KB           8100000 ops/s          (baseline)
MergeSort - Random                  100000         18.234 ms        390.62 KB           5484000 ops/s                0.68x
HeapSort - Random                   100000         23.456 ms        390.62 KB           4263000 ops/s                0.53x
std::sort - Random                  100000          9.876 ms        390.62 KB          10125000 ops/s                1.25x
```

**Interpretation:**
- QuickSort is baseline (1.00x)
- MergeSort is 68% as fast (slower)
- HeapSort is 53% as fast (slowest)
- std::sort is 25% faster (best)

### Key Metrics

1. **Time** - Absolute execution time
2. **Throughput** - Operations per second
3. **Relative Speed** - Multiplier vs baseline
4. **Memory** - Peak memory usage

---

## 💡 Algorithm Selection Guide

### Decision Tree

```
Choosing a sorting algorithm?
│
├─ Need stable sort (preserve order)?
│  └─ Use MergeSort ✅
│     - Guaranteed O(n log n)
│     - Stable
│     - Extra O(n) space
│
├─ Memory constrained?
│  └─ Use HeapSort ✅
│     - In-place O(1) extra space
│     - Guaranteed O(n log n)
│     - Slower than Quick/Merge
│
├─ Small array (n < 50)?
│  └─ Use InsertionSort ✅
│     - Simple, fast for small n
│     - O(n) for nearly sorted
│     - Low overhead
│
├─ Nearly sorted data?
│  └─ Use InsertionSort ✅
│     - Adaptive O(n) behavior
│     - Fast on nearly sorted
│
├─ Production code?
│  └─ Use std::sort ✅
│     - Highly optimized
│     - IntroSort algorithm
│     - Best general-purpose choice
│
└─ General random data?
   └─ Use QuickSort ✅
      - Fast average case
      - In-place
      - Good pivot selection
```

### Real-World Examples

#### Example 1: Sorting User Records by Name
```cpp
struct User {
    std::string name;
    int age;
};

std::vector<User> users = /* ... */;

// Need stability to preserve age order for same names
merge_sort(users.begin(), users.end(), 
          [](const User& a, const User& b) {
              return a.name < b.name;
          });
```

#### Example 2: Sorting Large Dataset with Limited RAM
```cpp
std::vector<int> huge_data = /* millions of elements */;

// HeapSort: O(1) extra space
heap_sort(huge_data.begin(), huge_data.end());
```

#### Example 3: Sorting Small Cached Data
```cpp
std::array<int, 20> cache = /* ... */;

// InsertionSort: fastest for small arrays
insertion_sort(cache.begin(), cache.end());
```

#### Example 4: Production Code
```cpp
std::vector<int> data = /* ... */;

// std::sort: default choice
std::sort(data.begin(), data.end());
```

---

## 🐛 Troubleshooting

### Benchmark takes too long
```bash
# Edit sorting_benchmark.cpp
# Reduce DATASET_SIZES:
const std::vector<std::size_t> DATASET_SIZES = {
    100,     // Tiny
    1000,    // Small
    10000    // Medium only
};
```

### Compilation errors
```bash
# Make sure algorithm library is included
ls -la include/algorithm/sorting.hpp

# Rebuild clean
cd build
rm -rf *
cmake ..
make
```

### InsertionSort skipped for large arrays
```
This is intentional!
InsertionSort is O(n²), too slow for n > 1000
Only tested on small arrays (≤1000 elements)
```

---

## 📈 Performance Tips

### Optimizing QuickSort
```cpp
// Already implemented:
// 1. Median-of-three pivot
// 2. Tail recursion optimization
// 3. Switch to InsertionSort for small subarrays
```

### Optimizing MergeSort
```cpp
// Improvements:
// 1. Bottom-up merge (iterative)
// 2. Natural merge (exploit runs)
// 3. Switch to InsertionSort for small chunks
```

### When to Use Each

| Data Characteristics | Best Algorithm |
|---------------------|----------------|
| Random, large (n>1000) | QuickSort, std::sort |
| Nearly sorted | InsertionSort |
| Need stability | MergeSort |
| Memory limited | HeapSort |
| Small (n<50) | InsertionSort |
| Production code | std::sort |

---

## 🎯 Next Steps

After running the benchmark:

1. ✅ **Analyze results** - Which algorithm won?
2. ✅ **Document findings** - Save results to `benchmarks/results/`
3. 🎯 **Compare with balanced trees** - Different performance profiles
4. 🎯 **Try optimization** - Tune parameters
5. 🎯 **Real-world test** - Apply to actual problems

---

## 📊 Expected Runtime

- **Tiny (100)**: < 1 second
- **Small (1,000)**: ~5 seconds
- **Medium (10,000)**: ~30 seconds
- **Large (100,000)**: ~2 minutes
- **Total**: ~2.5 minutes

---

Ready to run? Execute:
```bash
cd /workspaces/myLibraries/build
./benchmarks/benchmark_sorting
```

Happy benchmarking! 🚀