#include <omp.h>
#include <stdio.h>
#include <stdlib.h> 
#include <ctime> 

int** generateRibbonMatrix(int size) {
  int bandWidth = 10;
  int** matrix = new int*[size];
  for (int i = 0; i < size; i++)
  {
    int* a = new int[size];
    for (int j = 0; j < size; j++)
    {
        if (std::abs(i - j) <= bandWidth) {
          a[j] = rand() % 11 - 5;
        } else {
          a[j] = 0;
        }
    }
    matrix[i] = a;
  }
  return matrix;
}

int** generateTriangularMatrix(int size) {
  int** matrix = new int*[size];
  for (int i = 0; i < size; i++)
  {
    int* a = new int[size];
    for (int j = i; j < size; j++)
    {
        a[i] = rand() % 11 - 5;
    }
    matrix[i] = a;
  }
  return matrix;
}

void calcMaxMinMatrix(int matrixSize, int** matrix, int threads) {

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
  int matrixSize = 100;

  printf("Ribbon Matrix\n");
  for (int i = threads; i <= 10; i++){
    calcMaxMinMatrix(
      matrixSize,
      generateRibbonMatrix(matrixSize),
      threads
    );
    threads += 1;
  }
  printf("------------------------------------\n");
  printf("Triangular Matrix\n");
  threads = 1;
  for (int j = threads; j <= 10; j++){
    calcMaxMinMatrix(
      matrixSize,
      generateTriangularMatrix(matrixSize),
      threads
    );
    threads += 1;
  }
}
