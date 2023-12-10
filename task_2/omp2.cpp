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

int VECTOR_SIZE = 1000000; //1_000_000
int* a = generateRandomArray(VECTOR_SIZE);
int* b = generateRandomArray(VECTOR_SIZE);

void calcVectorsMultiply(int VECTOR_SIZE, int threads) {
  omp_set_num_threads(threads);

  int sum = 0;
  int chunkSize = VECTOR_SIZE / threads;

  double time_start = omp_get_wtime();

  #pragma omp parallel shared(a, b, VECTOR_SIZE)
  {
    #pragma omp for schedule(dynamic, chunkSize) reduction(+: sum)
      for (int i = 0; i < VECTOR_SIZE; ++i) {
        sum += a[i] * b[i];
      }
  }

  double delta = (omp_get_wtime() - time_start);

  printf("time: %f threads = %d\n", delta, threads);
}

int main()
{
  int threads = 1;

  for (int i = threads; i <= 10; i++){
    calcVectorsMultiply(VECTOR_SIZE, threads);
    threads += 1;
  }
}
