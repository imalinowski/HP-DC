#include <omp.h>
#include <stdio.h>
#include <stdlib.h> 
#include <ctime> 
#include <iostream>

void report_num_threads(int level){
 #pragma omp single
 {
    //printf("Level %d: threads - %d\n",level, omp_get_num_threads());
 }
}

void nestedExperiment(int threads){
  omp_set_num_threads(threads);
  omp_set_dynamic(0);

  double time_start = omp_get_wtime();
  #pragma omp parallel
  {
    report_num_threads(1);
    #pragma omp parallel
    {
      report_num_threads(2);
      #pragma omp parallel
      {
         report_num_threads(3);
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
    nestedExperiment(i);
    threads += 1;
  }
  
  printf("OMP_NESTED TRUE \n");
  omp_set_nested(1);

  threads = 1;
  for (int i = threads; i <= 7; i++){
    nestedExperiment(i);
    threads += 1;
  }
  return(0);
}
