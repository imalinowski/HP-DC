#include <omp.h>
#include <stdio.h>
#include <stdlib.h> 
#include <ctime> 

int** generateRandomMatrix(int size) {
  int** matrix = new int*[size];
  for (int i = 0; i < size; i++)
  {
    int* a = new int[size];
    for (int i = 0; i < size; i++)
    {
        a[i] = rand() % 11 - 5;
    }
    matrix[i] = a;
  }
  return matrix;
}

int matrixSize = 10000; // 10_000

int findMinOfArray(int* array) {
  int minRowValue = 99999;
  for (int j = 0; j < matrixSize; ++j)
  {
    if (array[j] < minRowValue) {
      minRowValue = array[j];
    }
  }
  return minRowValue;
}

int** matrix = generateRandomMatrix(matrixSize);

void calcMaxMinMatrix(int threads) {
  omp_set_num_threads(threads);

  int maxValue = 0;
  int chunkSize = matrixSize / threads;

  double time_start = omp_get_wtime();

  #pragma omp parallel shared(matrix, maxValue)
  {
    #pragma omp for schedule(dynamic, chunkSize) reduction(max:maxValue)
      for (int i = 0; i < matrixSize; ++i) {
        int minRowValue = findMinOfArray(matrix[i]);
        maxValue = minRowValue;
      }
  }

  double delta = (omp_get_wtime() - time_start);

  printf("time: %f threads = %d\n", delta, threads);
}

void calcMaxMinMatrixDeepParalel(int threads) {
  omp_set_num_threads(threads);

  int maxValue = 0;
  int chunkSize = matrixSize / threads;

  double time_start = omp_get_wtime();

  #pragma omp parallel shared(matrix, maxValue)
  {
    #pragma omp for schedule(dynamic, chunkSize) reduction(max:maxValue)
      for (int i = 0; i < matrixSize; ++i) {

        int minRowValue = 99999;
        #pragma omp parallel for schedule(dynamic, chunkSize) reduction(min:minRowValue)
        for (int j = 0; j < matrixSize; ++j)
        {
          if (matrix[i][j] < minRowValue) {
            minRowValue = matrix[i][j];
          }
        }

        maxValue = minRowValue;
      }
  }

  double delta = (omp_get_wtime() - time_start);

  printf("time: %f threads = %d\n", delta, threads);
}

int main()
{
  int threads = 1;

  for (int i = threads; i <= 7; i++){
    calcMaxMinMatrix(threads);
    threads += 1;
  }
  threads = 1;

  printf("Nested Cycle Parallel\n");
  for (int i = threads; i <= 7; i++){
    calcMaxMinMatrixDeepParalel(threads);
    threads += 1;
  }
  threads = 1;
}
