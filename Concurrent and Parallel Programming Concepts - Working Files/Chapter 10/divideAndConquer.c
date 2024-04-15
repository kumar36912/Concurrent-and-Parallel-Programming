/** 
    gcc -fopenmp divideAndConquer.c -o divideAndConquer
    ./divideAndConquer # on Windows: divideAndConquer

    This program uses OpenMP to divide a job into chunks, each of
    which is assigned to a worker thread for execution. OpenMP handles
    the distribution of chunks to threads to ensure that the entire job 
    is processed.
**/

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define JobSize   (100)                                                       /** line 1 **/
#define ChunkSize  (10)                                                       /** line 2 **/

int main () {
  int thread_count, tid, i, chunk = ChunkSize;
  float a[JobSize], b[JobSize], c[JobSize];                                   /** line 3 **/

  for (i = 0; i < JobSize; i++) a[i] = b[i] = i * 1.1f;                       /** line 4 **/

  #pragma omp parallel shared(a, b, c, thread_count, chunk) private(i, tid)   /** line 5 **/
  {
    tid = omp_get_thread_num();
    if (tid == 0) {
	thread_count = omp_get_num_threads();
	printf("\nThread count: %i\n", thread_count);
    }
    printf("worker thread %i getting work...\n", tid);
    
    #pragma omp for schedule(dynamic, chunk)                                  /** line 6 **/
    for (i = 0; i < JobSize; i++) {                                           /** line 7 **/
	c[i] = a[i] + b[i];
	printf("Worker thread %i: c[%i]= %f\n", tid, i, c[i]);
    }
  }  
  puts("Singled-thread execution again...");
  for (i = 0; i < JobSize; i++) printf("c[%i] == %f\n", i, c[i]);             /** line 8 **/
}
