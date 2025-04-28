#include <iostream>
#include <cstdlib>
#include <chrono>

using namespace std;
using namespace chrono;

// This function fills the vector with random numbers between 0 and 99
void randomVector(int vector[], int size) {
    for (int i = 0; i < size; i++) {
        vector[i] = rand() % 100;
    }
}

int main() {
    const int size = 100000000; // Big size to see performance difference
    int* v1 = new int[size];
    int* v2 = new int[size];
    int* v3 = new int[size];

    // Fill v1 and v2 with random numbers
    randomVector(v1, size);
    randomVector(v2, size);

    // Different chunk sizes for testing
    int chunk_sizes[] = {10, 100, 1000, 10000};

    // ---------- Static Scheduling ----------
    for (int chunk : chunk_sizes) {
        auto start = high_resolution_clock::now();

        // Using OpenMP with static schedule
        #pragma omp parallel for schedule(static, chunk) default(none) shared(v1, v2, v3, size, chunk)
        for (int i = 0; i < size; i++) {
            v3[i] = v1[i] + v2[i]; // Add elements
        }

        auto end = high_resolution_clock::now();
        duration<double> time = end - start;
        cout << "Static scheduling (chunk = " << chunk << ") time: " << time.count() << " seconds" << endl;
    }

    // ---------- Dynamic Scheduling ----------
    for (int chunk : chunk_sizes) {
        auto start = high_resolution_clock::now();

        // Using OpenMP with dynamic schedule
        #pragma omp parallel for schedule(dynamic, chunk) default(none) shared(v1, v2, v3, size, chunk)
        for (int i = 0; i < size; i++) {
            v3[i] = v1[i] + v2[i]; // Add elements
        }

        auto end = high_resolution_clock::now();
        duration<double> time = end - start;
        cout << "Dynamic scheduling (chunk = " << chunk << ") time: " << time.count() << " seconds" << endl;
    }

    // ---------- Guided Scheduling ----------
    for (int chunk : chunk_sizes) {
        auto start = high_resolution_clock::now();

        // Using OpenMP with guided schedule
        #pragma omp parallel for schedule(guided, chunk) default(none) shared(v1, v2, v3, size, chunk)
        for (int i = 0; i < size; i++) {
            v3[i] = v1[i] + v2[i]; // Add elements
        }

        auto end = high_resolution_clock::now();
        duration<double> time = end - start;
        cout << "Guided scheduling (chunk = " << chunk << ") time: " << time.count() << " seconds" << endl;
    }

    // ---------- Auto Scheduling ----------
    auto start = high_resolution_clock::now();

    // Let OpenMP decide the best scheduling
    #pragma omp parallel for schedule(auto) default(none) shared(v1, v2, v3, size)
    for (int i = 0; i < size; i++) {
        v3[i] = v1[i] + v2[i]; // Add elements
    }

    auto end = high_resolution_clock::now();
    duration<double> time = end - start;
    cout << "Auto scheduling time: " << time.count() << " seconds" << endl;

    // Free the memory
    delete[] v1;
    delete[] v2;
    delete[] v3;

    return 0;
}
