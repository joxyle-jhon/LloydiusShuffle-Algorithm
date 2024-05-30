#include <iostream>
#include <vector>
#include <random>
#include <algorithm> // For std::shuffle
#include <omp.h>
#include <ctime>
#include <chrono>

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
    std::vector<int> arr(1000000); // Adjust size as needed

    // Initialize the array with some values
    for (int i = 0; i < arr.size(); ++i) {
        arr[i] = i;
    }

    auto start = std::chrono::high_resolution_clock::now(); // Start timing

    #pragma omp parallel
    {
        #pragma omp single
        {
            quickShuffle(arr, 0, arr.size() - 1, rng);
        }
    }

    auto end = std::chrono::high_resolution_clock::now(); // End timing
    std::chrono::duration<double> duration = end - start;
    std::cout << "Run time: " << duration.count() << " seconds" << std::endl;

    // Shuffle the array using std::shuffle
    std::shuffle(arr.begin(), arr.end(), rng);

    // Optional: Verify if the array is shuffled
    // for (const int& num : arr) {
    //     std::cout << num << " ";
    // }
    // std::cout << std::endl;

    return 0;
}
