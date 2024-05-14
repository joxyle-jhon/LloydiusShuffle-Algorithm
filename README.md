# LloydliusShuffle Algorithm

## Overview

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
#include <cstdlib>
#include <ctime>
#include <unordered_set>
#include <omp.h>

using namespace std;

void LloydliusShuffle(vector<int>& arr, int low, int high) {
    if (low < high) {
        unordered_set<int> shuffledIndices;

        int pivotIndex = low + rand() % (high - low + 1);
        int pivotValue = arr[pivotIndex];

        swap(arr[low], arr[pivotIndex]);

        int i = low + 1;

        for (int j = low + 1; j <= high; j++) {
            if (j == pivotIndex) continue;
            int indexToSwap = j;

            while (shuffledIndices.find(indexToSwap) != shuffledIndices.end()) {
                indexToSwap = low + rand() % (high - low + 1);
            }

            swap(arr[i], arr[indexToSwap]);
            shuffledIndices.insert(indexToSwap);
            i++;
        }

        swap(arr[low], arr[i - 1]);

        #pragma omp task
        LloydliusShuffle(arr, low, i - 2);
        #pragma omp task
        LloydliusShuffle(arr, i, high);
        #pragma omp taskwait
    }
}

int main() {
    srand(time(NULL));

    vector<int> arr = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
    int size = arr.size();

    #pragma omp parallel
    {
        #pragma omp single
        LloydliusShuffle(arr, 0, size - 1);
    }

    for (int num : arr) {
        cout << num << " ";
    }
    cout << endl;

    return 0;
}
