# LloydliusShuffle Algorithm

## Algorithm Overview

Welcome to the LloydliusShuffle project! This algorithm was developed by Jhon Lloyd Omblero and Julius Baliling. This repository contains the implementation of the LloydliusShuffle algorithm, designed as the final project for the Algorithm and Time Complexity 2 course. LloydliusShuffle is an innovative algorithm that combines quicksort-like partitioning with random shuffling to achieve an efficient and effective shuffling of arrays.

## Algorithm Design

LloydliusShuffle leverages the partitioning strategy from quicksort while incorporating random shuffling within each partition. The algorithm works as follows:

### Core Idea:
- **Partitioning**: A random pivot element is chosen, and elements are rearranged such that those smaller than the pivot are placed before it, and those larger than the pivot are placed after it.
- **Random Shuffling**: Within each partition (excluding the pivot), elements are randomly shuffled using an unordered set to track already used indices for swapping.

### Detailed Steps:
1. **Base Case**: If the sub-array has one or zero elements, it is already shuffled (low >= high).
2. **Choose Random Pivot**: Select a random pivot index within the sub-array.
3. **Partitioning**:
   - Swap the pivot element with the first element of the sub-array.
   - Initialize an index `i` to `low + 1`.
4. **Random Shuffling within Partitions**:
   - Use an unordered set to keep track of used indices for swapping.
   - Iterate from `low + 1` to `high` (excluding the pivot), randomly selecting and swapping indices.
5. **Place Pivot**: Move the pivot to its final position.
6. **Recursion**:
   - Recursively apply LloydliusShuffle to the left and right sub-arrays.

## OpenMP Parallelization

LloydliusShuffle utilizes OpenMP to parallelize the recursive shuffling of sub-arrays, enhancing performance on multi-core processors:

- **Task Creation**: Recursive calls to shuffle the left and right sub-arrays are enclosed in `#pragma omp task` directives.
- **Task Synchronization**: The `#pragma omp taskwait` directive ensures all tasks complete before proceeding.

## Implementation

The following C++ code demonstrates the implementation of LloydliusShuffle:

```cpp
#include <iostream>
#include <vector>
#include <random>
#include <omp.h>

// Partition function for Quick Shuffle
int partition(std::vector<int>& arr, int low, int high, std::mt19937& rng) {
    int pivotIndex = low + rng() % (high - low + 1);
    int pivot = arr[pivotIndex];
    std::swap(arr[pivotIndex], arr[high]); // Move pivot to end
    int i = low - 1;

    for (int j = low; j < high; ++j) {
        if (arr[j] <= pivot) {
            ++i;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]); // Move pivot to its final position
    return i + 1;
}

// Quick Shuffle function
void quickShuffle(std::vector<int>& arr, int low, int high, std::mt19937& rng) {
    if (low < high) {
        int pi = partition(arr, low, high, rng);

        #pragma omp task shared(arr)
        quickShuffle(arr, low, pi - 1, rng);

        #pragma omp task shared(arr)
        quickShuffle(arr, pi + 1, high, rng);
    }
}

int main() {
    std::mt19937 rng(std::time(nullptr));
    std::vector<int> arr(10000000); // Adjust size as needed

    // Initialize the array with some values
    for (int i = 0; i < arr.size(); ++i) {
        arr[i] = i;
    }

    #pragma omp parallel
    {
        #pragma omp single
        {
            quickShuffle(arr, 0, arr.size() - 1, rng);
        }
    }

    return 0;
}

    cout << endl;

    return 0;
}
