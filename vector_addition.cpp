#include <chrono>
#include <cstdlib>
#include <iostream>
#include <ctime>

using namespace std::chrono;
using namespace std;

void generateRandomValues(int* array, int length) {
  // Fill the array with random values between 0 and 99
  for (int idx = 0; idx < length; idx++) {
    array[idx] = rand() % 100;
  }
}

int main() {
  // Set the size of the vectors
  unsigned long vectorSize = 100000000;

  // Seed the random number generator with the current time
  srand(static_cast<unsigned>(time(0)));

  // Declare pointers for the vectors and result
  int *firstVector, *secondVector, *resultVector;

  // Start measuring time for the operation
  auto startTime = high_resolution_clock::now();
  
  // Allocate memory for the vectors
  firstVector = (int *)malloc(vectorSize * sizeof(int));
  secondVector = (int *)malloc(vectorSize * sizeof(int));
  resultVector = (int *)malloc(vectorSize * sizeof(int));

  // Generate random values for both input vectors
  generateRandomValues(firstVector, vectorSize);
  generateRandomValues(secondVector, vectorSize);
  
  // Perform vector addition in parallel using OpenMP
  #pragma omp parallel for
  for (int idx = 0; idx < vectorSize; idx++) {
    resultVector[idx] = firstVector[idx] + secondVector[idx];
  }

  // Stop measuring time after the operation
  auto endTime = high_resolution_clock::now();

  // Calculate the duration of the operation in microseconds
  auto elapsedTime = duration_cast<microseconds>(endTime - startTime);
  
  // Output the time taken for the vector addition
  cout << "Execution time: " << elapsedTime.count() << " microseconds" << endl;

  // Free the allocated memory to avoid memory leaks
  free(firstVector);
  free(secondVector);
  free(resultVector);

  return 0;
}