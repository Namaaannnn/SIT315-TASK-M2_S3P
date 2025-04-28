#include <chrono>
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <pthread.h>

using namespace std::chrono;
using namespace std;

// Function to generate random values in the array
void generateRandomValues(int arr[], int length) {
  for (int i = 0; i < length; i++) arr[i] = rand() % 100;
}

// Sequential vector addition
void sequentialAdd(int a[], int b[], int c[], int length) {
  for (int i = 0; i < length; i++) c[i] = a[i] + b[i];
}

// Parallel vector addition using OpenMP
void parallelAddOpenMP(int a[], int b[], int c[], int length) {
  #pragma omp parallel for
  for (int i = 0; i < length; i++) c[i] = a[i] + b[i];
}

struct ThreadInfo { int *a, *b, *c, startIdx, endIdx; };

// Parallel vector addition using Pthreads
void* parallelAddPthreads(void* arg) {
  ThreadInfo* info = (ThreadInfo*)arg;
  for (int i = info->startIdx; i < info->endIdx; i++) info->c[i] = info->a[i] + info->b[i];
  return nullptr;
}

// Function to run Pthreads for parallel addition
void executePthreads(int a[], int b[], int c[], int length, int numThreads) {
  pthread_t threads[numThreads]; 
  ThreadInfo info[numThreads];
  int chunkSize = length / numThreads;

  for (int i = 0; i < numThreads; i++) {
    info[i] = {a, b, c, i * chunkSize, (i == numThreads - 1) ? length : (i + 1) * chunkSize};
    pthread_create(&threads[i], nullptr, parallelAddPthreads, &info[i]);
  }

  for (int i = 0; i < numThreads; i++) pthread_join(threads[i], nullptr);
}

// Main function
int main() {
  unsigned long dataSize = 100000000;
  srand(time(0));
  
  // Allocating memory for the vectors
  int *vec1 = (int *)malloc(dataSize * sizeof(int)), *vec2 = (int *)malloc(dataSize * sizeof(int)), *result = (int *)malloc(dataSize * sizeof(int));
  
  generateRandomValues(vec1, dataSize);
  generateRandomValues(vec2, dataSize);

  auto start = high_resolution_clock::now();
  sequentialAdd(vec1, vec2, result, dataSize);
  auto stop = high_resolution_clock::now();
  cout << "Sequential Add Time: " << duration_cast<microseconds>(stop - start).count() << " microseconds" << endl;

  start = high_resolution_clock::now();
  parallelAddOpenMP(vec1, vec2, result, dataSize);
  stop = high_resolution_clock::now();
  cout << "OpenMP Add Time: " << duration_cast<microseconds>(stop - start).count() << " microseconds" << endl;

  start = high_resolution_clock::now();
  executePthreads(vec1, vec2, result, dataSize, 8);
  stop = high_resolution_clock::now();
  cout << "Pthreads Add Time: " << duration_cast<microseconds>(stop - start).count() << " microseconds" << endl;

  free(vec1); 
  free(vec2); 
  free(result);
  
  return 0;
}