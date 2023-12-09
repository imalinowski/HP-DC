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

int VECTOR_SIZE = 1000000;
int* a = generateRandomArray(VECTOR_SIZE);

void findMinElementParallel(int threads) {
  omp_set_num_threads(threads);

  int min_el = 99999;
  int chunkSize = VECTOR_SIZE / threads;

  double time_start = omp_get_wtime();

  #pragma omp parallel shared(a, VECTOR_SIZE)
  {
    #pragma omp for schedule(dynamic, chunkSize) reduction(min:min_el)
      for (int i = 0; i < VECTOR_SIZE; ++i)
        if (a[i] < min_el){
					  min_el = a[i];
        }
  }

  double delta = (omp_get_wtime() - time_start);

  printf("time: %f threads = %d\n", delta, threads);
}

int main()
{
  int threads = 1;

  for (int i = threads; i <= 7; i++){
    findMinElementParallel(threads);
    threads += 1;
  }
}
