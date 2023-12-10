#include <omp.h>
#include <stdio.h>
#include <stdlib.h> 
#include <ctime> 

int f(int x) {
  return 1 * x^2 + 2 * x + 3;
}

void calcIntergralParallel(int threads) {
  omp_set_num_threads(threads);
  
  // Init params
  int N = 1000000;
  float a = -100;
  float b = 100;
  float h = (b - a) / N;

  float sum = 0;

  double time_start = omp_get_wtime();

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

  printf("time: %f threads = %d\n", delta, threads);
}

int main()
{
  int threads = 1;

  for (int i = threads; i <= 7; i++){
    calcIntergralParallel(threads);
    threads += 1;
  }
}