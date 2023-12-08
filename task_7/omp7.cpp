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

void scalarMulty(int threads) {
  omp_set_num_threads(threads);

  int arraySize = 100;
  int** arrays = generateRandomMatrix(arraySize);
  int scalarVectorMult = 0;

  double time_start = omp_get_wtime();
  #pragma omp parallel sections reduction(+:scalarVectorMult)
    {
        #pragma omp section
        {    
            for (int i = 1; i < arraySize; ++i) {
              for (int j = 0; j < arraySize; ++j) {
                scalarVectorMult += arrays[i-1][j] * arrays[i][j];
              }
            }

        }
    }

  double delta = (omp_get_wtime() - time_start);
  printf("Threads = %d Time = %f \n", threads, delta);
}

int main()
{
  int threads = 1;

  for (int i = threads; i <= 10; i++){
    scalarMulty(threads);
    threads += 1;
  }
}
