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

void findMinElementParallel(int VECTOR_SIZE, int threads) {
  int* a = generateRandomArray(VECTOR_SIZE);

  srand(static_cast<unsigned int>(time(0)));

  int chunkSize = VECTOR_SIZE / threads;
  omp_set_num_threads(threads);

  printf("VECTOR_SIZE = %d, Threads = %d\n", VECTOR_SIZE, threads);

  double time_start = omp_get_wtime();

  int min_el = 99999;

  #pragma omp parallel shared(a, VECTOR_SIZE)
  {
    #pragma omp for schedule(dynamic, chunkSize) reduction (min:min_el)
      for (int i = 0; i < VECTOR_SIZE; ++i)
        if (a[i] < min_el)
					  min_el = a[i];
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
    findMinElementParallel(VECTOR_SIZE, threads);
  }
}
