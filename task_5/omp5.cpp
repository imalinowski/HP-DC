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

int matrixSize = 1000; // 1_000

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

int** ribbonMatrix = generateRibbonMatrix(matrixSize);
int** triangularMatrix = generateTriangularMatrix(matrixSize);
int** matrix = ribbonMatrix;

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

int main()
{
  int threads = 1;

  printf("Ribbon Matrix\n");
  matrix = ribbonMatrix;
  for (int i = threads; i <= 7; i++){
    calcMaxMinMatrix(threads);
    threads += 1;
  }

  printf("------------------------------------\n");

  printf("Triangular Matrix\n");
  threads = 1;
  matrix = triangularMatrix;
  for (int j = threads; j <= 7; j++){
    calcMaxMinMatrix(threads);
    threads += 1;
  }
}
