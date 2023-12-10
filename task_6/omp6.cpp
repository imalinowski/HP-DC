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

int arraySize = 100000; // 100_000
int* array = generateRandomArray(arraySize);

void reduction(int threads) {
  omp_set_num_threads(threads);

  int sum = 0;
  int chunkSize = arraySize/threads;

  printf("threads = %d\n", threads);

  // Reducion
  double startTime = omp_get_wtime();
  #pragma omp parallel for schedule(dynamic, chunkSize) reduction(+: sum)
  for (int i = 0; i < arraySize; ++i)
  {
    sum += array[i];
  }
  double delta = (omp_get_wtime() - startTime);
  printf("Reduction\n");
  printf("time = %f\n", delta);
  sum = 0;

  // Atomic
  startTime = omp_get_wtime();
  #pragma omp parallel for shared(sum) schedule(dynamic, chunkSize)
  for (int i = 0; i < arraySize; ++i)
  {
    #pragma omp atomic
    sum += array[i];
  }
  delta = (omp_get_wtime() - startTime);
  printf("Atomic\n");
  printf("time = %f\n", delta);
  sum = 0;

  // Critical
  startTime = omp_get_wtime();
  #pragma omp parallel for shared(sum) schedule(dynamic, chunkSize)
  for (int i = 0; i < arraySize; ++i)
  {
    #pragma omp critical
    sum += array[i];
  }
  delta = (omp_get_wtime() - startTime);
  printf("Critical\n");
  printf("time = %f\n", delta);
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
  delta = (omp_get_wtime() - startTime);
  printf("Lock\n");
  printf("time = %f\n", delta);
  sum = 0;

  printf("--------------------------------------------------\n");
}

int main()
{
  int threads = 1;

  for (int i = threads; i <= 7; i++){
    reduction(threads);
    threads += 1;
  }
}
