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

void calcVectorsMultiply(int VECTOR_SIZE, int threads) {
  int* a = generateRandomArray(VECTOR_SIZE);
  int* b = generateRandomArray(VECTOR_SIZE);

  srand(static_cast<unsigned int>(time(0)));

  int chunkSize = VECTOR_SIZE / threads;
  omp_set_num_threads(threads);

  printf("VectorSize = %d, Threads = %d\n", VECTOR_SIZE, threads);

  double time_start = omp_get_wtime();

  int sum = 0;

  #pragma omp parallel shared(a, b, VECTOR_SIZE)
  {
    #pragma omp for schedule(dynamic, chunkSize) reduction(+: sum)
      for (int i = 0; i < VECTOR_SIZE; ++i)
        sum += a[i] * b[i];
  }

  double delta = (omp_get_wtime() - time_start);

  printf("time: %f\n", delta);
}

int main()
{
  int VECTOR_SIZE = 10000;
  int threads = 1;

  for (int i = threads; i < 10; i++){
    threads += 1;
    calcVectorsMultiply(VECTOR_SIZE, threads);
  }
}
