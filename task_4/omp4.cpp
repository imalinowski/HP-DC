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

int* generateRandomArray(int size) {
  int* a = new int[size];
    for (int i = 0; i < size; i++)
    {
        a[i] = rand() % 11 - 5;
    }
  return a;
}

void calcMaxMinMatrix(int threads) {
  int matrixSize = 100;
  int** matrix = generateRandomMatrix(matrixSize);

  srand(static_cast<unsigned int>(time(0)));

  int chunkSize = matrixSize / threads;
  omp_set_num_threads(threads);

  printf("ChunkSize = %d, Threads = %d\n", chunkSize, threads);

  double time_start = omp_get_wtime();

  int maxValue = 0;

  #pragma omp parallel shared(matrix, maxValue)
  {
    #pragma omp for schedule(dynamic, chunkSize)
      for (int i = 0; i < matrixSize; ++i) {
        int minRowValue = 9999;

        for (int j = 0; j < matrixSize; ++j)
        {
          if (matrix[i][j] < minRowValue)
          {
            minRowValue = matrix[i][j];
          }
        }

        if (minRowValue > maxValue)
        {
          #pragma omp critical
          if (minRowValue > maxValue)
          {
            maxValue = minRowValue;
          }
        }
      }
  }

  double delta = (omp_get_wtime() - time_start);

  printf("time: %f\n", delta);
}

int main()
{
  int threads = 1;

  for (int i = threads; i <= 10; i++){
    calcMaxMinMatrix(threads);
    threads += 1;
  }
}
