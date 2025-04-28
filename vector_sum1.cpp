#include <iostream>
#include <cstdlib>
#include <chrono>

using namespace std;
using namespace chrono;

// Function to fill vector with random numbers between 0 and 99
void randomVector(int vector[], int size) {
    for (int i = 0; i < size; i++) {
        vector[i] = rand() % 100;
    }
}

int main() {
    const int size = 1000000; // Size of arrays
    int *v1 = new int[size];
    int *v2 = new int[size];
    int *v3 = new int[size];
    int total_atomic = 0;       // Total using atomic
    int total_reduction = 0;    // Total using reduction

    // Fill vectors with random values
    randomVector(v1, size);
    randomVector(v2, size);

    auto start = high_resolution_clock::now(); // Start timer

    // Add v1 and v2, store in v3 using parallel for loop
    #pragma omp parallel for default(none) shared(v1, v2, v3, size)
    for (int i = 0; i < size; i++) {
        v3[i] = v1[i] + v2[i];
    }

    // Calculate total using atomic
    #pragma omp parallel for default(none) shared(v3, size, total_atomic)
    for (int i = 0; i < size; i++) {
        #pragma omp atomic
        total_atomic += v3[i];
    }

    cout << "Total sum using atomic: " << total_atomic << endl;

    // Calculate total using reduction clause (much better for performance)
    #pragma omp parallel for default(none) shared(v3, size) reduction(+:total_reduction)
    for (int i = 0; i < size; i++) {
        total_reduction += v3[i];
    }

    cout << "Total sum using reduction: " << total_reduction << endl;

    auto end = high_resolution_clock::now(); // End timer
    duration<double> time = end - start;

    cout << "Execution time: " << time.count() << " seconds" << endl;

    // Free memory
    delete[] v1;
    delete[] v2;
    delete[] v3;

    return 0;
}
