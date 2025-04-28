#include <iostream>
#include <cstdlib>
#include <chrono>

using namespace std;
using namespace chrono;

// This function fills the array with random values from 0 to 99
void randomVector(int vector[], int size) {
    for (int i = 0; i < size; i++) {
        vector[i] = rand() % 100;
    }
}

int main() {
    const int size = 1000000; // size of the vectors
    int *v1 = new int[size];
    int *v2 = new int[size];
    int *v3 = new int[size];
    int total = 0; // shared variable for total sum

    // Fill v1 and v2 with random values
    randomVector(v1, size);
    randomVector(v2, size);

    auto start = high_resolution_clock::now(); // start timer

    // Parallel addition of v1 and v2 using OpenMP
    #pragma omp parallel for default(none) shared(v1, v2, v3, size)
    for (int i = 0; i < size; i++) {
        v3[i] = v1[i] + v2[i];
    }

    // Compute total sum using atomic update to avoid race condition
    #pragma omp parallel for default(none) shared(v3, size, total)
    for (int i = 0; i < size; i++) {
        #pragma omp atomic
        total += v3[i];
    }

    auto end = high_resolution_clock::now(); // end timer
    duration<double> time = end - start;

    cout << "Total sum using atomic: " << total << endl;
    cout << "Execution time: " << time.count() << " seconds" << endl;

    // Clean up memory
    delete[] v1;
    delete[] v2;
    delete[] v3;

    return 0;
}
