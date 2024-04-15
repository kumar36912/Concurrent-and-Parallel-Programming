/** To compile with GNU or OMP-enabled Clang: 
    gcc -fopenmp basicOMP.c -o basicOMP

    Then execute in the usual way: ./basicOMP (on Windows, drop the './').

    This program illustrates the basics of OpenMP multithreading. By default
    OpenMP creates one thread per processor, but the number of threads also can be
    specified eplicitly.
**/

#include <omp.h>                              /** line 1 **/
#include <stdio.h>
#include <stdlib.h>

int main() {
  int t_count = 0, tid;                       /** line 2 **/

  /* Fork threads, each of which gets its own copy of the local variables. */
  #pragma omp parallel private(t_count, tid)  /** line 3 **/
  {
    /* Obtain thread ID. */
    tid = omp_get_thread_num();               /** line 4 **/
    printf("Thread %i\n", tid);
    
    /* The 'master' or 'main' thread has an ID of 0. */
    if (tid == 0) {
      t_count = omp_get_num_threads();        /** line 5 **/
      printf("Thread count on this machine is: %i\n\n", t_count);
    }
  }  /* implicit wait here */
  puts("Goodbye, cruel world!");
  return 0;
}
