#include <omp.h>
#include <stdio.h>
#include <stdlib.h> 
#include <ctime> 
#include <iostream>

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

int arraySize = 1000; // 1_000

void scalarMulty(int threads) {
  omp_set_num_threads(threads);

  int** arrays = new int*[arraySize];
  int* vectorMult = new int[arraySize];
  for( int i = 0; i < arraySize; ++i) {
    vectorMult[i] = 1;
  }
  int* scalarMult = 0;
  int currentGeneredLine = 0;

  omp_lock_t lock;
  omp_init_lock(&lock);
  double time_start = omp_get_wtime();
  #pragma omp parallel sections
    {
        #pragma omp section
        {    
            for (int i = 0; i < arraySize; ++i) {
              omp_set_lock(&lock);
              int* a = new int[arraySize];
              for (int j = 0; j < arraySize; ++j) {
                a[j] = i * j;
              }
              arrays[i] = a;
              currentGeneredLine = i;
              omp_unset_lock(&lock);
            }
        }
        #pragma omp section
        {    
            for (int i = 1; i < arraySize; ++i) {
              if(currentGeneredLine < i) {
                  omp_set_lock(&lock);
                  omp_unset_lock(&lock);
              }
              for (int j = 0; j < arraySize; ++j) {
                vectorMult[j] *= arrays[i][j];
              }
            }
            for (int i = 0; i < arraySize; ++i) {
              scalarMult += vectorMult[i];
            }
        }
    }

  double delta = (omp_get_wtime() - time_start);
  printf("time: %f threads = %d\n", delta, threads);
}

int main()
{
  int threads = 1;

  for (int i = threads; i <= 7; i++){
    scalarMulty(threads);
    threads += 1;
  }
}
