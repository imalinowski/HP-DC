#include <omp.h>
#include <stdio.h>
#include <stdlib.h> 
#include <ctime> 

int f(int x) {
  return x^2;
}

void calcIntergralParallel(int threads) {
  
  // Init params
  int N = 1000000;
  float a = -100;
  float b = 100;
  float h = (b - a) / N;

  omp_set_num_threads(threads);

  printf("Threads = %d\n", threads);

  double time_start = omp_get_wtime();

  float sum = 0;

  #pragma omp parallel shared(N, a, b, h)
  {
  #pragma omp for schedule(guided) reduction(+: sum)
    for (int i = 0; i < N; ++i)
    {
      float x = i*h;
      sum += h * f(x);
    }
  }

  double delta = (omp_get_wtime() - time_start);

  printf("Integral value = %f\n", sum);
  printf("time: %f\n", delta);
}

int main()
{
  int threads = 1;

  for (int i = threads; i <= 10; i++){
    calcIntergralParallel(threads);
    threads += 1;
  }
}