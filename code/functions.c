/*****************************************************************************/
/* Estimación del número Pi usando el método de Montecarlo                   */
/* Versión secuencial                                                        */
/* Autor: Danny Múnera                                                       */
/* Curso de programación paralela con OpenMP                                 */
/* 21/07/2020                                                                */
/*****************************************************************************/
#include "functions.h"

double montecarlo_seq (int p) {  
  int i, counter = 0;
  double x, y, z, pi;

  // Computation of pi
  for(i = 0; i < p; ++i) {
    x = (double)rand() / RAND_MAX;
    y = (double)rand() / RAND_MAX;
    z = x * x + y * y;
    if( z <= 1 ) counter++;
  }
  pi = (double) counter / p * 4;

  return pi;
}

double montecarlo_par (int p, int n_threads, unsigned int seed) {
  
  int i, local_p, local_counter, counter, my_rank, new_p;
  double x, y, z, pi;
  
  local_counter = 0;
  counter = 0;
  local_p = p / n_threads;
  new_p = local_p * n_threads;

  #pragma omp parallel \
    num_threads(n_threads) \
    shared(counter) \
    private (x, y, z, i, my_rank) \
    firstprivate(local_counter, local_p)
  {
    my_rank = omp_get_thread_num();
    unsigned int local_seed = seed * (my_rank+1);
    
    for(i = 0; i < local_p; ++i) {
      x = (double)rand_r(&local_seed) / RAND_MAX;
      y = (double)rand_r(&local_seed) / RAND_MAX;
      z = x * x + y * y;
      if( z <= 1 )
        local_counter++;
    }
    counter += local_counter;
  }
  pi = (double) counter / new_p * 4;
  
  return pi;
}
