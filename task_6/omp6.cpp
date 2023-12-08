#include <omp.h>
#include <stdio.h>
#include <stdlib.h> 
#include <ctime> 

int* generateRandomArray(int size) {
  int* a = new int[size];
    for (int i = 0; i < size; i++)
    {
        a[i] = rand() % 11 - 5;
    }
  return a;
}

void reduction(int threads) {
  printf("Threads = %d\n", threads);
  int arraySize = 100000;
  int* array = generateRandomArray(arraySize);
  int sum = 0;

  int chunkSize = arraySize/threads;
  omp_set_num_threads(threads);

  // Reducion
  double startTime = omp_get_wtime();
  #pragma omp parallel for schedule(dynamic, chunkSize) reduction(+: sum)
  for (int i = 0; i < arraySize; ++i)
  {
    sum += array[i];
  }
  double endTime = omp_get_wtime();
  double timeSpent = (endTime - startTime);

  printf("Sum = %d; Reduction spent time = %f\n", sum, timeSpent);
  sum = 0;

  // Atomic
  startTime = omp_get_wtime();
  #pragma omp parallel for shared(sum) schedule(dynamic, chunkSize)
  for (int i = 0; i < arraySize; ++i)
  {
    #pragma omp atomic
    sum += array[i];
  }
  endTime = omp_get_wtime();
  timeSpent = endTime - startTime;
  printf("Sum = %d; Atomic spent time = %f\n", sum, timeSpent);
  sum = 0;

  // Critical
  startTime = omp_get_wtime();
  #pragma omp parallel for shared(sum) schedule(dynamic, chunkSize)
  for (int i = 0; i < arraySize; ++i)
  {
    #pragma omp critical
    sum += array[i];
  }
  endTime = omp_get_wtime();
  timeSpent = endTime - startTime;
  printf("Sum = %d; Critical spent time = %f\n", sum, timeSpent);
  sum = 0;

  // Lock
  omp_lock_t lock;
  omp_init_lock(&lock);
  startTime = omp_get_wtime();
  #pragma omp parallel for shared(sum) schedule(dynamic, chunkSize)
  for (int i = 0; i < arraySize; ++i)
  {
    omp_set_lock(&lock);
    sum += array[i];
    omp_unset_lock(&lock);
  }
  endTime = omp_get_wtime();
  timeSpent = endTime - startTime;
  printf("Sum = %d; Lock spent time = %f\n", sum, timeSpent);
  sum = 0;
  printf("--------------------------------------------------\n");
}

int main()
{
  int threads = 1;

  for (int i = threads; i <= 10; i++){
    reduction(threads);
    threads += 1;
  }
}
