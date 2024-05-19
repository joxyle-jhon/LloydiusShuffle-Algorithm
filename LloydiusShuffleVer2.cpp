//Version 2 

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <unordered_set>
#include <omp.h>

using namespace std;

// Combined Lloydius Shuffle function
void LloydiusShuffle(vector<int>& arr, int low, int high) {
    if (low < high) {
        // Step 1: Apply Fisher-Yates shuffle
        for (int i = high; i > low; --i) {
            int j = low + rand() % (i - low + 1);
            swap(arr[i], arr[j]);
        }

        // Step 2: Apply Lloydlius Shuffle with divide-and-conquer
        unordered_set<int> shuffledIndices; // To store shuffled indices

        // Choose a random pivot index
        int pivotIndex = low + rand() % (high - low + 1);
        int pivotValue = arr[pivotIndex]; // Store the value of the pivot

        // Swap pivot with the first element
        swap(arr[low], arr[pivotIndex]);

        int i = low + 1; // Index to start shuffling from

        // Shuffle elements around the pivot (excluding the pivot itself)
        for (int j = low + 1; j <= high; j++) {
            if (j == pivotIndex) continue; // Skip the pivot
            int indexToSwap = j;

            // Randomly choose an index to swap with, ensuring it hasn't been chosen before
            while (shuffledIndices.find(indexToSwap) != shuffledIndices.end()) {
                indexToSwap = low + rand() % (high - low + 1);
            }

            // Swap the element at indexToSwap with arr[i]
            swap(arr[i], arr[indexToSwap]);
            shuffledIndices.insert(indexToSwap);
            i++;
        }

        // Place the pivot in its final position
        swap(arr[low], arr[i - 1]);

        // Recursively shuffle both halves in parallel
        #pragma omp task
        LloydiusShuffle(arr, low, i - 2); // Exclude the pivot's position
        #pragma omp task
        LloydiusShuffle(arr, i, high);
        #pragma omp taskwait
    }
}

int main() {
    // Example usage
    srand(time(NULL));

    // Generate a random array
    vector<int> arr = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
    int size = arr.size();

    // Perform combined Lloydius Shuffling
    #pragma omp parallel
    {
        #pragma omp single
        LloydiusShuffle(arr, 0, size - 1);
    }

    // Print the shuffled array
    for (int num : arr) {
        cout << num << " ";
    }
    cout << endl;

    return 0;
}
