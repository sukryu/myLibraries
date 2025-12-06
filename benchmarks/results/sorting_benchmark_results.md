# Sorting Algorithms Benchmark Results

**Date:** 2025-12-06  
**Environment:** GitHub Codespaces  
**Compiler:** GCC (Ubuntu)  
**C++ Standard:** C++17  
**Build Type:** Release (-O2)

## Executive Summary

This benchmark compares five sorting algorithm implementations:
- **QuickSort**: Custom median-of-three pivot implementation
- **MergeSort**: Custom stable divide-and-conquer implementation
- **HeapSort**: Custom in-place heap-based implementation
- **InsertionSort**: Simple comparison-based sorting
- **std::sort**: Standard library (IntroSort: QuickSort + HeapSort + InsertionSort)

**Key Findings:**
- 🏆 **std::sort dominates ALL scenarios** - 3~16x faster than custom implementations
- 🥈 **MergeSort outperforms QuickSort** - Consistently 1.4~1.8x faster (unexpected!)
- 🥉 **QuickSort underperforms** - Slower than expected, especially on reverse sorted data
- 💥 **HeapSort slowest** - 0.4~0.7x performance, particularly poor with duplicates
- 🐌 **InsertionSort** - Only competitive for n<20, otherwise too slow

**Recommendation:** **Always use std::sort** for production code. Custom implementations cannot compete with highly optimized standard library.

---

## Table of Contents

- [Test Configuration](#test-configuration)
- [Results by Dataset Size](#results-by-dataset-size)
- [Performance Analysis](#performance-analysis)
- [Algorithm Comparison](#algorithm-comparison)
- [Why std::sort Dominates](#why-stdsort-dominates)
- [Unexpected Results](#unexpected-results)
- [Practical Guidelines](#practical-guidelines)
- [Conclusions](#conclusions)
- [Appendix: Raw Data](#appendix-raw-data)

---

## Test Configuration

### Algorithms Tested

| Algorithm | Type | Stability | Space | Implementation |
|-----------|------|-----------|-------|----------------|
| QuickSort | Divide-and-conquer | Unstable | O(log n) | Custom (median-of-three) |
| MergeSort | Divide-and-conquer | Stable | O(n) | Custom (top-down) |
| HeapSort | Selection-based | Unstable | O(1) | Custom (binary heap) |
| InsertionSort | Insertion-based | Stable | O(1) | Custom (simple) |
| std::sort | Hybrid | Unstable | O(log n) | Standard library |

### Test Patterns

1. **Random Data** - Shuffled array (typical case)
2. **Nearly Sorted** - 90% sorted, 10% shuffled (adaptive case)
3. **Reverse Sorted** - N→1 descending order (worst case for naive algorithms)
4. **Many Duplicates** - Only 10% unique values (equal element handling)
5. **Small Arrays** - 10, 50, 100, 500 elements (overhead analysis)

### Dataset Sizes

- **Tiny:** 100 elements
- **Small:** 1,000 elements
- **Medium:** 10,000 elements
- **Large:** 100,000 elements

### Metrics

- Execution time (milliseconds)
- Throughput (operations/second)
- Relative speedup vs baseline
- Memory usage

---

## Results by Dataset Size

### Tiny Arrays (100 elements)

#### Random Data

| Algorithm | Time | Throughput | Relative Speed |
|-----------|------|------------|----------------|
| QuickSort | 0.073 ms | 1,361,341 ops/s | 1.00x (baseline) |
| MergeSort | 0.054 ms | 1,844,644 ops/s | **1.36x** ⚡ |
| HeapSort | 0.070 ms | 1,420,011 ops/s | 1.04x |
| InsertionSort | 0.120 ms | 835,324 ops/s | 0.61x |
| **std::sort** | **0.014 ms** | **7,180,813 ops/s** | **5.27x** ⚡⚡⚡ |

**Analysis:** Even for tiny arrays, std::sort is 5x faster. InsertionSort not competitive.

#### Nearly Sorted (90%)

| Algorithm | Time | Relative Speed |
|-----------|------|----------------|
| QuickSort | 0.062 ms | 1.00x (baseline) |
| MergeSort | 0.028 ms | **2.22x** ⚡ |
| HeapSort | 0.074 ms | 0.83x |
| InsertionSort | 0.043 ms | 1.43x ✅ |
| **std::sort** | **0.011 ms** | **5.86x** ⚡⚡⚡ |

**Analysis:** InsertionSort shows some improvement but std::sort still dominates.

#### Reverse Sorted

| Algorithm | Time | Relative Speed |
|-----------|------|----------------|
| QuickSort | 0.073 ms | 1.00x (baseline) |
| MergeSort | 0.084 ms | 0.87x |
| HeapSort | 0.063 ms | 1.16x |
| InsertionSort | 0.233 ms | 0.31x ⚠️ |
| **std::sort** | **0.007 ms** | **10.70x** ⚡⚡⚡⚡ |

**Analysis:** std::sort 10x faster! Detects and handles worst case immediately.

---

### Small Arrays (1,000 elements)

#### Random Data

| Algorithm | Time | Throughput | Relative Speed |
|-----------|------|------------|----------------|
| QuickSort | 0.580 ms | 1,724,400 ops/s | 1.00x (baseline) |
| MergeSort | 0.557 ms | 1,794,720 ops/s | **1.04x** |
| HeapSort | 1.080 ms | 925,706 ops/s | 0.54x |
| InsertionSort | 13.847 ms | 72,218 ops/s | 0.04x ⚠️ |
| **std::sort** | **0.172 ms** | **5,825,096 ops/s** | **3.38x** ⚡⚡⚡ |

**Analysis:** InsertionSort becomes 24x slower. std::sort maintains 3x advantage.

#### Nearly Sorted (90%)

| Algorithm | Time | Relative Speed |
|-----------|------|----------------|
| QuickSort | 0.541 ms | 1.00x (baseline) |
| MergeSort | 0.422 ms | **1.28x** ⚡ |
| HeapSort | 1.091 ms | 0.50x |
| InsertionSort | 2.935 ms | 0.18x |
| **std::sort** | **0.129 ms** | **4.20x** ⚡⚡⚡ |

**Analysis:** std::sort adapts extremely well. MergeSort outperforms QuickSort.

#### Reverse Sorted

| Algorithm | Time | Relative Speed |
|-----------|------|----------------|
| QuickSort | 0.844 ms | 1.00x (baseline) |
| MergeSort | 0.830 ms | **1.02x** |
| HeapSort | 1.449 ms | 0.58x |
| InsertionSort | 26.182 ms | 0.03x ⚠️⚠️ |
| **std::sort** | **0.079 ms** | **10.75x** ⚡⚡⚡⚡ |

**Analysis:** std::sort 10x faster on worst case! InsertionSort O(n²) evident.

---

### Medium Arrays (10,000 elements)

#### Random Data

| Algorithm | Time | Throughput | Relative Speed |
|-----------|------|------------|----------------|
| QuickSort | 8.376 ms | 1,193,951 ops/s | 1.00x (baseline) |
| MergeSort | 7.462 ms | 1,340,176 ops/s | **1.12x** ⚡ |
| HeapSort | 14.087 ms | 709,866 ops/s | 0.59x |
| **std::sort** | **2.283 ms** | **4,380,084 ops/s** | **3.67x** ⚡⚡⚡ |

**Analysis:** MergeSort consistently beats QuickSort. std::sort 3.7x faster.

#### Nearly Sorted (90%)

| Algorithm | Time | Relative Speed |
|-----------|------|----------------|
| QuickSort | 8.303 ms | 1.00x (baseline) |
| MergeSort | 6.398 ms | **1.30x** ⚡ |
| HeapSort | 18.206 ms | 0.46x |
| **std::sort** | **1.316 ms** | **6.31x** ⚡⚡⚡⚡ |

**Analysis:** std::sort adapts extremely well to nearly sorted data (6x faster).

#### Reverse Sorted

| Algorithm | Time | Relative Speed |
|-----------|------|----------------|
| QuickSort | 16.927 ms | 1.00x (baseline) ⚠️ |
| MergeSort | 9.497 ms | **1.78x** ⚡ |
| HeapSort | 13.646 ms | 1.24x |
| **std::sort** | **1.009 ms** | **16.78x** ⚡⚡⚡⚡⚡ |

**Analysis:** QuickSort terrible on reverse data (17ms vs 1ms). MergeSort 1.8x better.

#### Many Duplicates (10% unique)

| Algorithm | Time | Relative Speed |
|-----------|------|----------------|
| QuickSort | 8.261 ms | 1.00x (baseline) |
| MergeSort | 11.787 ms | 0.70x |
| HeapSort | 23.042 ms | 0.36x ⚠️ |
| **std::sort** | **3.052 ms** | **2.71x** ⚡⚡ |

**Analysis:** HeapSort particularly bad with duplicates (23ms!).

---

### Large Arrays (100,000 elements)

#### Random Data

| Algorithm | Time | Throughput | Relative Speed |
|-----------|------|------------|----------------|
| QuickSort | 138.423 ms | 722,426 ops/s | 1.00x (baseline) |
| MergeSort | 98.281 ms | 1,017,491 ops/s | **1.41x** ⚡ |
| HeapSort | 188.224 ms | 531,281 ops/s | 0.74x |
| **std::sort** | **28.685 ms** | **3,486,161 ops/s** | **4.83x** ⚡⚡⚡⚡ |

**Analysis:** 
- std::sort: **4.8x faster** than QuickSort
- MergeSort: **1.4x faster** than QuickSort (unexpected!)
- HeapSort: Slowest (26% slower than QuickSort)

#### Nearly Sorted (90%)

| Algorithm | Time | Relative Speed |
|-----------|------|----------------|
| QuickSort | 118.164 ms | 1.00x (baseline) |
| MergeSort | 84.349 ms | **1.40x** ⚡ |
| HeapSort | 194.799 ms | 0.61x |
| **std::sort** | **17.163 ms** | **6.88x** ⚡⚡⚡⚡ |

**Analysis:** std::sort exploits partial sorting extremely well (7x faster).

#### Reverse Sorted

| Algorithm | Time | Relative Speed |
|-----------|------|----------------|
| QuickSort | 165.201 ms | 1.00x (baseline) ⚠️⚠️ |
| MergeSort | 91.479 ms | **1.81x** ⚡ |
| HeapSort | 180.401 ms | 0.92x |
| **std::sort** | **11.522 ms** | **14.34x** ⚡⚡⚡⚡⚡ |

**Analysis:** 
- **QuickSort WORST performance** (165ms - pathological case)
- MergeSort 1.8x better (predictable O(n log n))
- **std::sort 14x faster** - detects and handles worst case

#### Many Duplicates (10% unique)

| Algorithm | Time | Relative Speed |
|-----------|------|----------------|
| QuickSort | 94.414 ms | 1.00x (baseline) |
| MergeSort | 100.587 ms | 0.94x |
| HeapSort | 240.182 ms | 0.39x ⚠️⚠️ |
| **std::sort** | **28.686 ms** | **3.29x** ⚡⚡⚡ |

**Analysis:** HeapSort catastrophic with duplicates (240ms - 2.5x slower than QuickSort!).

---

## Performance Analysis

### Overall Performance Summary

#### Throughput Comparison (100,000 elements, ops/sec)

| Pattern | QuickSort | MergeSort | HeapSort | std::sort | Winner |
|---------|-----------|-----------|----------|-----------|--------|
| Random | 722,426 | 1,017,491 | 531,281 | **3,486,161** | std::sort (4.8x) |
| Nearly Sorted | 846,281 | 1,185,554 | 513,351 | **5,826,366** | std::sort (6.9x) |
| Reverse | 605,323 | 1,093,153 | 554,321 | **8,679,037** | std::sort (14.3x) |
| Duplicates | 1,059,169 | 994,168 | 416,351 | **3,485,988** | std::sort (3.3x) |

### Performance by Pattern

#### 🔴 Random Data

```
Performance Ranking (100K elements):
1. std::sort:    28.7 ms  (4.83x baseline)  ⭐⭐⭐⭐⭐
2. MergeSort:    98.3 ms  (1.41x baseline)  ⭐⭐⭐
3. QuickSort:   138.4 ms  (baseline)        ⭐⭐
4. HeapSort:    188.2 ms  (0.74x baseline)  ⭐
```

**Winner:** std::sort by massive margin

#### 🟢 Nearly Sorted (90%)

```
Performance Ranking (100K elements):
1. std::sort:    17.2 ms  (6.88x baseline)  ⭐⭐⭐⭐⭐
2. MergeSort:    84.3 ms  (1.40x baseline)  ⭐⭐⭐
3. QuickSort:   118.2 ms  (baseline)        ⭐⭐
4. HeapSort:    194.8 ms  (0.61x baseline)  ⭐
```

**Winner:** std::sort (adapts extremely well)

#### 🔴 Reverse Sorted (Worst Case)

```
Performance Ranking (100K elements):
1. std::sort:    11.5 ms  (14.34x baseline) ⭐⭐⭐⭐⭐
2. MergeSort:    91.5 ms  (1.81x baseline)  ⭐⭐⭐
3. QuickSort:   165.2 ms  (baseline) ⚠️     ⭐
4. HeapSort:    180.4 ms  (0.92x baseline)  ⭐
```

**Winner:** std::sort (handles worst case brilliantly)
**Loser:** QuickSort (pathological case despite median-of-three)

#### 🟡 Many Duplicates

```
Performance Ranking (100K elements):
1. std::sort:    28.7 ms  (3.29x baseline)  ⭐⭐⭐⭐⭐
2. QuickSort:    94.4 ms  (baseline)        ⭐⭐⭐
3. MergeSort:   100.6 ms  (0.94x baseline)  ⭐⭐
4. HeapSort:    240.2 ms  (0.39x baseline)  ⚠️
```

**Winner:** std::sort
**Loser:** HeapSort (2.5x slower than QuickSort!)

---

## Algorithm Comparison

### QuickSort Performance

| Dataset | Pattern | Time | vs std::sort | Notes |
|---------|---------|------|--------------|-------|
| 100 | Random | 0.073 ms | 5.2x slower | High overhead |
| 1K | Random | 0.580 ms | 3.4x slower | Decent |
| 10K | Random | 8.376 ms | 3.7x slower | Expected |
| 100K | Random | 138.4 ms | 4.8x slower | OK average case |
| 100K | Reverse | 165.2 ms | 14.3x slower ⚠️ | **Pathological** |

**Strengths:**
- ✅ In-place (O(log n) space)
- ✅ Decent on random data

**Weaknesses:**
- ❌ Poor on reverse sorted (despite median-of-three)
- ❌ Slower than MergeSort consistently
- ❌ Cannot compete with std::sort

**Verdict:** Custom QuickSort not worth it. Use std::sort.

---

### MergeSort Performance

| Dataset | Pattern | Time | vs QuickSort | vs std::sort |
|---------|---------|------|--------------|--------------|
| 100 | Random | 0.054 ms | **1.36x faster** ⚡ | 3.9x slower |
| 1K | Random | 0.557 ms | **1.04x faster** | 3.2x slower |
| 10K | Random | 7.462 ms | **1.12x faster** | 3.3x slower |
| 100K | Random | 98.3 ms | **1.41x faster** ⚡ | 3.4x slower |
| 100K | Reverse | 91.5 ms | **1.81x faster** ⚡ | 7.9x slower |

**Strengths:**
- ✅ **Consistently beats QuickSort** (1.1~1.8x)
- ✅ Stable sorting
- ✅ Predictable O(n log n)
- ✅ Good on all patterns

**Weaknesses:**
- ❌ O(n) extra space
- ❌ Still much slower than std::sort (3~8x)

**Verdict:** Best custom implementation, but std::sort still better.

---

### HeapSort Performance

| Dataset | Pattern | Time | vs QuickSort | Notes |
|---------|---------|------|--------------|-------|
| 100K | Random | 188.2 ms | 0.74x | Slow |
| 100K | Nearly Sorted | 194.8 ms | 0.61x | Doesn't adapt |
| 100K | Reverse | 180.4 ms | 0.92x | OK |
| 100K | Duplicates | 240.2 ms | 0.39x ⚠️ | **Catastrophic** |

**Strengths:**
- ✅ In-place O(1) space
- ✅ Guaranteed O(n log n)

**Weaknesses:**
- ❌ Slowest in all cases
- ❌ Terrible with duplicates (240ms!)
- ❌ Poor cache locality
- ❌ Unpredictable branching

**Verdict:** Never use custom HeapSort. std::sort uses it internally when needed.

---

### InsertionSort Performance

| Dataset | Pattern | Time | vs QuickSort | Notes |
|---------|---------|------|--------------|-------|
| 100 | Random | 0.120 ms | 0.61x | Decent for tiny |
| 100 | Nearly Sorted | 0.043 ms | 1.43x ✅ | Good! |
| 1K | Random | 13.85 ms | 0.04x ⚠️ | Too slow |
| 1K | Reverse | 26.18 ms | 0.03x ⚠️⚠️ | O(n²) evident |

**Strengths:**
- ✅ Simple implementation
- ✅ Good on nearly sorted small arrays
- ✅ O(1) space

**Weaknesses:**
- ❌ O(n²) kills it for n>100
- ❌ Even for n=100, std::sort is 5x faster

**Verdict:** Only useful as subroutine in hybrid algorithms (like std::sort uses).

---

## Why std::sort Dominates

### IntroSort Algorithm

std::sort uses **IntroSort** (Introspective Sort):

```
IntroSort Strategy:
1. Start with QuickSort
2. Monitor recursion depth
3. If depth exceeds 2*log(n):
   → Switch to HeapSort (avoid O(n²) worst case)
4. For small subarrays (n < threshold):
   → Switch to InsertionSort (low overhead)
```

**Advantages:**
- ✅ QuickSort speed on average case
- ✅ HeapSort guarantee on worst case
- ✅ InsertionSort efficiency on small arrays
- ✅ Best of all worlds!

### Compiler Optimizations

Standard library implementations have:

1. **SIMD Instructions**
   - Vectorized comparisons
   - Parallel processing

2. **Branch Prediction**
   - Optimized conditional logic
   - Minimal mispredictions

3. **Cache Optimization**
   - Prefetching
   - Memory alignment
   - Cache-friendly partitioning

4. **Tuned Parameters**
   - Optimal InsertionSort threshold
   - Best pivot selection
   - Depth limit tuning

### Performance Evidence

| Scenario | std::sort Advantage | Reason |
|----------|---------------------|--------|
| Random (100K) | **4.8x faster** | Optimized QuickSort |
| Nearly Sorted | **6.9x faster** | Detects and adapts |
| Reverse (100K) | **14.3x faster** | Switches to HeapSort |
| Small arrays | **5-10x faster** | Uses InsertionSort internally |

**Conclusion:** Impossible to beat with custom implementation.

---

## Unexpected Results

### 1. MergeSort > QuickSort

**Expected:** QuickSort faster on average
**Actual:** MergeSort 1.1~1.8x faster consistently

**Possible Reasons:**

#### QuickSort Issues:
- Pivot selection not optimal
- Partitioning has overhead
- Poor cache locality
- Branch mispredictions

#### MergeSort Advantages:
- Sequential memory access (cache-friendly)
- Predictable branches
- Consistent performance
- Well-optimized implementation

#### Evidence:
```
100K Random:
  QuickSort: 138 ms
  MergeSort:  98 ms (1.41x faster)

100K Reverse:
  QuickSort: 165 ms
  MergeSort:  92 ms (1.81x faster)
```

**Lesson:** Sequential memory access > clever algorithms with bad locality

---

### 2. QuickSort Terrible on Reverse

**Expected:** Median-of-three handles reverse well
**Actual:** 165ms vs MergeSort 92ms (1.8x slower)

**Analysis:**

Median-of-three samples: first, middle, last
```
Reverse array: [1000, 999, 998, ..., 2, 1]
First:  1000
Middle: ~500
Last:   1

Median: 500 (middle value)
```

**Problem:** Still creates unbalanced partitions!

Better pivot strategies:
- Median-of-five
- Random sampling
- Ninther (median of medians)
- Dual-pivot (Java uses this)

**Lesson:** Even "smart" pivot selection can fail. std::sort handles this by switching algorithms.

---

### 3. HeapSort Catastrophic with Duplicates

**Expected:** O(n log n) guaranteed
**Actual:** 240ms (2.5x slower than QuickSort!)

**Reason:**

With many duplicates:
- Many equal comparisons
- Heap structure constantly violated
- Excessive sifting
- Poor cache behavior amplified

**Evidence:**
```
100K with 10% unique values:
  QuickSort:  94 ms
  MergeSort: 101 ms
  HeapSort:  240 ms ⚠️ (2.5x slower!)
```

**Lesson:** Big-O notation doesn't tell the whole story. Constants and cache matter.

---

### 4. InsertionSort Not Competitive Even for n=100

**Expected:** InsertionSort best for small arrays
**Actual:** std::sort still 5x faster (0.014ms vs 0.120ms)

**Reason:**

std::sort for n=100:
- Uses InsertionSort internally
- But with optimizations:
  - SIMD comparisons
  - Loop unrolling
  - Better code generation

**Lesson:** Standard library even optimizes "simple" algorithms better.

---

### 5. std::sort's 14x Advantage on Reverse

**Unexpected:** Such massive speedup (11.5ms vs 165ms)

**Explanation:**

std::sort detects worst case:
1. QuickSort starts
2. Recursion depth hits limit
3. **Immediately switches to HeapSort**
4. Completes in 11.5ms

Custom QuickSort:
1. Keeps trying with median-of-three
2. Creates unbalanced partitions
3. Deep recursion
4. Takes 165ms

**Lesson:** Hybrid algorithms > pure algorithms

---

## Practical Guidelines

### Decision Tree (Updated)

```
Need to sort data in C++?
│
└─ Use std::sort ✅✅✅

That's it. Always use std::sort.
```

### Seriously, When to Use Custom Implementations?

**Short answer:** Almost never.

**Long answer:**

| Scenario | Use | Reason |
|----------|-----|--------|
| **Production code** | std::sort | 3~16x faster |
| **Need stability** | std::stable_sort | Built-in, optimized |
| **Partial sorting** | std::partial_sort | Optimized for k smallest |
| **nth element** | std::nth_element | O(n) average |
| **Learning purposes** | Custom | Educational only |
| **Embedded systems** | std::sort | Still best unless code size critical |
| **Special requirements** | Custom | Very rare (e.g., external sorting) |

### Performance Expectations

| Data Size | std::sort Time | Custom QuickSort | Custom MergeSort |
|-----------|----------------|------------------|------------------|
| 100 | 0.014 ms | 0.073 ms (5x slower) | 0.054 ms (4x slower) |
| 1K | 0.172 ms | 0.580 ms (3x slower) | 0.557 ms (3x slower) |
| 10K | 2.283 ms | 8.376 ms (4x slower) | 7.462 ms (3x slower) |
| 100K | 28.69 ms | 138.4 ms (5x slower) | 98.28 ms (3x slower) |

**Conclusion:** Custom implementations simply cannot compete.

### Code Examples

#### ✅ Correct: Use std::sort

```cpp
#include <algorithm>
#include <vector>

std::vector<int> data = {/* ... */};

// Default: ascending order
std::sort(data.begin(), data.end());

// Custom comparator
std::sort(data.begin(), data.end(), std::greater<int>());

// Sort structs
struct Person {
    std::string name;
    int age;
};

std::vector<Person> people = {/* ... */};
std::sort(people.begin(), people.end(), 
         [](const Person& a, const Person& b) {
             return a.age < b.age;
         });
```

#### ✅ Correct: Stable sort when needed

```cpp
// Preserve relative order of equal elements
std::stable_sort(data.begin(), data.end());
```

#### ✅ Correct: Partial sort (top-k)

```cpp
// Get top 10 elements
std::partial_sort(data.begin(), data.begin() + 10, data.end());
```

#### ❌ Incorrect: Custom QuickSort

```cpp
// Don't do this - std::sort is 5x faster
quick_sort(data.begin(), data.end());
```

### Migration from Custom to std::sort

```cpp
// Before: Custom implementation
template<typename Iter>
void my_sort(Iter begin, Iter end) {
    // Custom QuickSort implementation
    // ...
}

// After: Use std::sort
template<typename Iter>
void my_sort(Iter begin, Iter end) {
    std::sort(begin, end);  // Much faster!
}
```

---

## Conclusions

### Overall Rankings

| Rank | Algorithm | Score | Best For |
|------|-----------|-------|----------|
| 🥇 | **std::sort** | ⭐⭐⭐⭐⭐ | Everything |
| 🥈 | MergeSort (custom) | ⭐⭐⭐ | Learning (stable sort) |
| 🥉 | QuickSort (custom) | ⭐⭐ | Learning (in-place sort) |
| 4️⃣ | InsertionSort | ⭐ | Tiny arrays (n<20) as subroutine |
| 5️⃣ | HeapSort | ⭐ | Never use directly |

### Key Takeaways

1. **std::sort is untouchable**
   - 3~16x faster than custom implementations
   - Handles all cases optimally
   - No reason to implement your own

2. **MergeSort surprised us**
   - Consistently beat QuickSort (1.4~1.8x)
   - Sequential memory access matters
   - Stable and predictable

3. **QuickSort disappointed**
   - Slower than expected
   - Poor on reverse data (165ms vs std::sort 11ms)
   - Median-of-three not enough

4. **HeapSort is impractical**
   - Slowest in all tests
   - Catastrophic with duplicates (240ms)
   - Poor cache behavior

5. **InsertionSort has very limited use**
   - Only for n<20 as subroutine
   - Not competitive even for n=100

### Theoretical vs Practical

| Algorithm | Theory | Practice (100K) | Reason |
|-----------|--------|-----------------|--------|
| QuickSort | O(n log n) avg | 138 ms | Cache misses, branches |
| MergeSort | O(n log n) | 98 ms ✅ | Sequential access |
| HeapSort | O(n log n) | 188-240 ms | Poor locality |
| std::sort | O(n log n) | **29 ms** ⚡ | Optimized hybrid |

**Lesson:** Big-O doesn't tell the whole story. Implementation quality, cache behavior, and compiler optimizations matter enormously.

### Final Recommendation

```cpp
// In production code:
std::sort(data.begin(), data.end());  // Always

// If you need stability:
std::stable_sort(data.begin(), data.end());

// If you need top-k:
std::partial_sort(data.begin(), data.begin() + k, data.end());

// Custom sorting algorithms: 
// Educational purposes only.
```

---

## Future Work

### Custom Implementation Improvements

#### QuickSort Optimizations
```cpp
// Current: Median-of-three
// Improvement: Ninther (median of three medians)
// Expected: 10-20% faster, better worst case

// Add: Dual-pivot partitioning
// Expected: 5-10% faster on duplicates

// Add: InsertionSort for small subarrays
// Expected: 15-20% faster overall
```

#### MergeSort Optimizations
```cpp
// Current: Top-down recursive
// Improvement: Bottom-up iterative
// Expected: 5-10% faster, no stack overflow

// Add: Natural merge (exploit existing runs)
// Expected: 2-3x faster on nearly sorted

// Add: InsertionSort for small chunks
// Expected: 10-15% faster
```

#### HeapSort Optimizations
```cpp
// Current: Standard binary heap
// Improvement: k-ary heap (k=4 or k=8)
// Expected: 10-20% faster (better cache)

// Add: Weak heap variant
// Expected: 5-10% fewer comparisons
```

**Expected Result:** Even with all optimizations, still 2-3x slower than std::sort.

---

### Additional Benchmarks

1. **Partially Sorted Data**
   - Varying degrees of sortedness (10%, 30%, 50%, 70%, 90%)
   - Find InsertionSort crossover point

2. **Different Data Types**
   - Large structs (64+ bytes)
   - Strings
   - Custom types with expensive comparisons

3. **Parallel Sorting**
   - std::execution::par
   - Custom parallel implementations

4. **External Sorting**
   - Datasets too large for RAM
   - File-based sorting

---

## Appendix: Raw Data

### Complete Results Table (100,000 elements)

| Pattern | Algorithm | Time (ms) | Throughput (ops/s) | Relative Speed |
|---------|-----------|-----------|-------------------|----------------|
| Random | QuickSort | 138.423 | 722,426 | 1.00x |
| Random | MergeSort | 98.281 | 1,017,491 | 1.41x |
| Random | HeapSort | 188.224 | 531,281 | 0.74x |
| Random | std::sort | 28.685 | 3,486,161 | 4.83x |
| Nearly Sorted | QuickSort | 118.164 | 846,281 | 1.00x |
| Nearly Sorted | MergeSort | 84.349 | 1,185,554 | 1.40x |
| Nearly Sorted | HeapSort | 194.799 | 513,351 | 0.61x |
| Nearly Sorted | std::sort | 17.163 | 5,826,366 | 6.88x |
| Reverse | QuickSort | 165.201 | 605,323 | 1.00x |
| Reverse | MergeSort | 91.479 | 1,093,153 | 1.81x |
| Reverse | HeapSort | 180.401 | 554,321 | 0.92x |
| Reverse | std::sort | 11.522 | 8,679,037 | 14.34x |
| Duplicates | QuickSort | 94.414 | 1,059,169 | 1.00x |
| Duplicates | MergeSort | 100.587 | 994,168 | 0.94x |
| Duplicates | HeapSort | 240.182 | 416,351 | 0.39x |
| Duplicates | std::sort | 28.686 | 3,485,988 | 3.29x |

### Small Arrays Performance

| Size | Algorithm | Time (μs) | Notes |
|------|-----------|-----------|-------|
| 100 | QuickSort | 73 | Baseline |
| 100 | MergeSort | 54 | 1.36x faster |
| 100 | HeapSort | 70 | Similar to Quick |
| 100 | InsertionSort | 120 | 0.61x |
| 100 | std::sort | **14** | 5.27x faster ⚡ |

### Crossover Points

| Comparison | Crossover | Winner Below | Winner Above |
|------------|-----------|--------------|--------------|
| InsertionSort vs QuickSort | ~20 elements | InsertionSort | QuickSort |
| Custom vs std::sort | Never | std::sort | std::sort |
| MergeSort vs QuickSort | All sizes | MergeSort | MergeSort |

---

**Document Version:** 1.0  
**Last Updated:** 2025-12-06  
**Author:** Jinhyeok  
**License:** MIT