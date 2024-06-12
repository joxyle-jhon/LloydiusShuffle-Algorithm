#include <iostream>
#include <vector>
#include <random>
#include <omp.h>
#include <ctime>
#include <chrono>

// Partition function for Quick Random Shuffle
int partition(std::vector<int>& arr, int low, int high, std::mt19937& rng) {
    std::uniform_int_distribution<int> dist(low, high);
    int pivotIndex = dist(rng);
    std::swap(arr[pivotIndex], arr[high]); // Move pivot to end
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; ++j) {
        if (rng() % 2 == 0) { // Randomly swap elements for shuffling
            ++i;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]); // Move pivot to its final position
    return i + 1;
}

// Quick Random Shuffle function
void quickRandomShuffle(std::vector<int>& arr, int low, int high, std::mt19937& rng) {
    if (low < high) {
        int pi = partition(arr, low, high, rng);

        #pragma omp task shared(arr)
        quickRandomShuffle(arr, low, pi - 1, rng);

        #pragma omp task shared(arr)
        quickRandomShuffle(arr, pi + 1, high, rng);

        #pragma omp taskwait
    }
}

int main() {
    std::mt19937 rng(std::time(nullptr));
    std::vector<int> arr(10000000); // Adjust size as needed

    // Initialize the array with some values
    for (int i = 0; i < arr.size(); ++i) {
        arr[i] = i;
    }

    auto start = std::chrono::high_resolution_clock::now(); // Start timing

    #pragma omp parallel
    {
        #pragma omp single
        {
            quickRandomShuffle(arr, 0, arr.size() - 1, rng);
        }
    }

    auto end = std::chrono::high_resolution_clock::now(); // End timing
    std::chrono::duration<double> duration = end - start;
    std::cout << "Run time: " << duration.count() << " seconds" << std::endl;

    // Optional: Verify if the array is shuffled
    for (const int& num : arr) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return 0;
}
