#include <iostream>
#include <cstdlib>
#include <chrono>

using namespace std;
using namespace chrono;

// Function to fill array with random values between 0 and 99
void randomVector(int vector[], int size) {
    for (int i = 0; i < size; i++) {
        vector[i] = rand() % 100;
    }
}

int main() {
    const int size = 1000000; // Size of the arrays

    // Dynamic memory allocation
    int *v1 = new int[size];
    int *v2 = new int[size];
    int *v3 = new int[size];

    // Variables to store sum using different methods
    int total_atomic = 0;
    int total_reduction = 0;
    int total_critical = 0;

    // Fill vectors with random values
    randomVector(v1, size);
    randomVector(v2, size);

    auto start = high_resolution_clock::now(); // Start time

    // Add v1 and v2 elements and store in v3 using parallel for loop
    #pragma omp parallel for default(none) shared(v1, v2, v3, size)
    for (int i = 0; i < size; i++) {
        v3[i] = v1[i] + v2[i];
    }

    // ------------------------------
    // 1. Sum using atomic directive
    // ------------------------------
    #pragma omp parallel for default(none) shared(v3, size, total_atomic)
    for (int i = 0; i < size; i++) {
        #pragma omp atomic
        total_atomic += v3[i];
    }
    cout << "Total sum using atomic: " << total_atomic << endl;

    // ------------------------------
    // 2. Sum using reduction clause
    // ------------------------------
    #pragma omp parallel for default(none) shared(v3, size) reduction(+:total_reduction)
    for (int i = 0; i < size; i++) {
        total_reduction += v3[i];
    }
    cout << "Total sum using reduction: " << total_reduction << endl;

    // ------------------------------------------------
    // 3. Sum using private local sum + critical section
    // ------------------------------------------------
    #pragma omp parallel default(none) shared(v3, size, total_critical)
    {
        int local_sum = 0;

        #pragma omp for
        for (int i = 0; i < size; i++) {
            local_sum += v3[i];
        }

        #pragma omp critical
        {
            total_critical += local_sum;
        }
    }
    cout << "Total sum using critical section: " << total_critical << endl;

    auto end = high_resolution_clock::now(); // End time
    duration<double> elapsed = end - start;

    cout << "Execution time: " << elapsed.count() << " seconds" << endl;

    // Free memory
    delete[] v1;
    delete[] v2;
    delete[] v3;

    return 0;
}
