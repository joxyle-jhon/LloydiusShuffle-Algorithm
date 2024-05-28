#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <random>
#include <chrono>
#include <omp.h>
#include <fstream>
#include <numeric> // for iota

using namespace std;

// Fisher-Yates shuffle function
void FisherYatesShuffle(vector<int>& arr, int low, int high)
{
    for (int i = high; i > low; --i)
    {
        int j = low + rand() % (i - low + 1);
        swap(arr[i], arr[j]);
    }
}

// Combined Lloydius Shuffle function
void LloydiusShuffle(vector<int>& arr, int low, int high)
{
    if (low < high)
    {
        // Step 1: Apply Fisher-Yates shuffle
        for (int i = high; i > low; --i)
        {
            int j = low + rand() % (i - low + 1);
            swap(arr[i], arr[j]);
        }

        // Step 2: Apply Lloydius Shuffle with divide-and-conquer
        unordered_set<int> shuffledIndices; // To store shuffled indices

        // Choose a random pivot index
        int pivotIndex = low + rand() % (high - low + 1);
        int pivotValue = arr[pivotIndex]; // Store the value of the pivot

        // Swap pivot with the first element
        swap(arr[low], arr[pivotIndex]);

        int i = low + 1; // Index to start shuffling from

        // Shuffle elements around the pivot (excluding the pivot itself)
        for (int j = low + 1; j <= high; j++)
        {
            if (j == pivotIndex)
                continue; // Skip the pivot
            int indexToSwap = j;

            // Randomly choose an index to swap with, ensuring it hasn't been chosen before
            while (shuffledIndices.find(indexToSwap) != shuffledIndices.end())
            {
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

void measureShuffleTime(void (*shuffleFunc)(vector<int>&, int, int), size_t size, bool parallel, ofstream& file, const string& shuffleName)
{
    vector<int> data;
    try
    {
        data.resize(size);
    }
    catch (const std::bad_alloc& e)
    {
        cerr << "Memory allocation failed: " << e.what() << endl;
        return;
    }

    iota(data.begin(), data.end(), 0);

    auto start = chrono::high_resolution_clock::now();

    if (parallel)
    {
        #pragma omp parallel
        {
            #pragma omp single
            shuffleFunc(data, 0, size - 1);
        }
    }
    else
    {
        shuffleFunc(data, 0, size - 1);
    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    string type = parallel ? "Parallel" : "Sequential";
    cout << "Shuffle: " << shuffleName << " Size: " << size << " Time: " << elapsed.count() << " seconds" << (parallel ? " (Parallel)" : " (Sequential)") << endl;
    file << shuffleName << "," << size << "," << elapsed.count() << "," << type << "\n";
}

int main()
{
    // Example usage
    ofstream file("shuffle_times.csv");
    file << "shuffle,size,time,type\n";

    vector<size_t> sizes = {100, 1000, 10000};
    for (size_t size : sizes)
    {
        measureShuffleTime(FisherYatesShuffle, size, false, file, "FisherYates"); // Sequential Fisher-Yates
        measureShuffleTime(FisherYatesShuffle, size, true, file, "FisherYates");  // Parallel Lloydius
        measureShuffleTime(LloydiusShuffle, size, false, file, "Lloydius"); // Sequential Lloydius
        measureShuffleTime(LloydiusShuffle, size, true, file, "Lloydius");  // Parallel Lloydius
    }

    file.close();
    return 0;
}