#include <iostream>
#include <vector>
#include <random>
#include <algorithm> // For std::shuffle
#include <omp.h>
#include <ctime>
#include <chrono>

// Merging two subarrays into a single shuffled array
void merge(std::vector<int>& arr, int left, int mid, int right, std::mt19937& rng) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Create temporary vectors
    std::vector<int> L(n1), R(n2);

    // Copy data to temporary vectors L and R
    for (int i = 0; i < n1; ++i)
        L[i] = arr[left + i];
    for (int i = 0; i < n2; ++i)
        R[i] = arr[mid + 1 + i];

    // Shuffle the temporary vectors L and R
    std::shuffle(L.begin(), L.end(), rng);
    std::shuffle(R.begin(), R.end(), rng);

    // Merge the shuffled vectors back into arr
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (rng() % 2 == 0)
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }

    // Copy remaining elements of L, if any
    while (i < n1)
        arr[k++] = L[i++];

    // Copy remaining elements of R, if any
    while (j < n2)
        arr[k++] = R[j++];
}

void mergeShuffle(std::vector<int>& arr, int left, int right, std::mt19937& rng) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        #pragma omp task shared(arr)
        mergeShuffle(arr, left, mid, rng);

        #pragma omp task shared(arr)
        mergeShuffle(arr, mid + 1, right, rng);

        #pragma omp taskwait

        merge(arr, left, mid, right, rng);
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
            mergeShuffle(arr, 0, arr.size() - 1, rng);
        }
    }

    auto end = std::chrono::high_resolution_clock::now(); // End timing
    std::chrono::duration<double> duration = end - start;
    std::cout << "Run time: " << duration.count() << " seconds" << std::endl;

    // Optional: Verify if the array is shuffled
    // for (const int& num : arr) {
    //     std::cout << num << " ";
    // }
    // std::cout << std::endl;

    return 0;
}
