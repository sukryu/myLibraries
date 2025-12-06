# Benchmark Setup Guide

## 📁 Step 1: Create Directory Structure

```bash
cd /workspaces/myLibraries

# Create benchmark directories
mkdir -p benchmarks/utils
mkdir -p benchmarks/tree
mkdir -p benchmarks/algorithm
mkdir -p benchmarks/results

# Create placeholder for results
touch benchmarks/results/.gitkeep
```

## 📋 Step 2: Copy Files

### Copy utilities
```bash
# Copy benchmark_utils.hpp
cp /path/to/benchmark_utils.hpp benchmarks/utils/

# Copy test file
cp /path/to/test_benchmark_utils.cpp benchmarks/

# Copy README
cp /path/to/benchmarks_README.md benchmarks/README.md

# Copy CMakeLists.txt
cp /path/to/CMakeLists_benchmarks.txt benchmarks/CMakeLists.txt
```

### Update root CMakeLists.txt
```bash
# Add to root CMakeLists.txt after "add_subdirectory(tests)"
add_subdirectory(benchmarks)
```

Or copy the new root CMakeLists.txt:
```bash
cp /path/to/CMakeLists_root.txt CMakeLists.txt
```

## 🔧 Step 3: Verify Structure

Your structure should look like:

```
myLibraries/
├── include/
│   └── tree/
│       ├── avl_tree.hpp
│       ├── red_black_tree.hpp
│       └── skip_list.hpp
├── benchmarks/                    ← NEW
│   ├── utils/
│   │   └── benchmark_utils.hpp
│   ├── tree/
│   │   └── (benchmarks will go here)
│   ├── algorithm/
│   ├── results/
│   │   └── .gitkeep
│   ├── test_benchmark_utils.cpp
│   ├── CMakeLists.txt
│   └── README.md
├── tests/
├── CMakeLists.txt                 ← UPDATED
└── README.md
```

## 🏗️ Step 4: Build

```bash
# Create/clean build directory
rm -rf build
mkdir build && cd build

# Configure
cmake ..

# Build
cmake --build .

# Or using make
make
```

You should see output like:
```
-- Added benchmark: balanced_tree
========================================
  Benchmark Configuration
========================================
  Utility test: test_benchmark_utils
...
```

## ✅ Step 5: Test

```bash
# Run benchmark utility tests
./benchmarks/test_benchmark_utils
```

Expected output:
```
========================================
Benchmark Utilities Test Suite
========================================

=== Testing Timer ===
Elapsed: 100.947 ms
...

✓ All utility tests completed!
```

## 🚀 Step 6: Run Benchmarks

Once you have benchmark files:

```bash
# Run balanced tree benchmark
./benchmarks/benchmark_balanced_tree

# Run all benchmarks (if configured)
make run_all_benchmarks

# Or from project root
cd .. && make -C build run_all_benchmarks
```

## 📊 Step 7: View Results

Results will be displayed in terminal and optionally saved to:
```
benchmarks/results/
├── balanced_trees.md
├── sorting.md
└── range_query.md
```

## 🔍 Troubleshooting

### "Threads not found"
```bash
# Install pthread library
sudo apt-get install libpthread-stubs0-dev
```

### "benchmark_utils.hpp not found"
```bash
# Check include path in CMakeLists.txt
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/utils)
```

### Build errors
```bash
# Clean rebuild
rm -rf build
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . -- -j$(nproc)
```

## 📝 Quick Setup Script

```bash
#!/bin/bash
# quick_setup.sh

cd /workspaces/myLibraries

# Create directories
mkdir -p benchmarks/{utils,tree,algorithm,results}
touch benchmarks/results/.gitkeep

# Copy files (update paths)
cp /mnt/user-data/outputs/benchmark_utils.hpp benchmarks/utils/
cp /mnt/user-data/outputs/test_benchmark_utils.cpp benchmarks/
cp /mnt/user-data/outputs/CMakeLists_benchmarks.txt benchmarks/CMakeLists.txt
cp /mnt/user-data/outputs/benchmarks_README.md benchmarks/README.md

# Update root CMakeLists.txt (manual step required)
echo "⚠️  Don't forget to add 'add_subdirectory(benchmarks)' to root CMakeLists.txt"

# Build
rm -rf build
mkdir build && cd build
cmake ..
cmake --build .

# Test
./benchmarks/test_benchmark_utils

echo "✓ Setup complete!"
```

## 🎯 Next Steps

1. ✅ Directory structure created
2. ✅ Utilities installed
3. ✅ CMake configured
4. ✅ Tests passing
5. 🎯 **Ready to create benchmarks!**

Now you can create `benchmarks/tree/balanced_tree_benchmark.cpp`