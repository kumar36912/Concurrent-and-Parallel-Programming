/** To compile and run (with, of course, OpenMPI installed):
    
    mpicc -o mpiBasics mpiBasics.c
    mpirun -np 4 ./mpiBasics   ## on Windows, drop the './'

    This program illustrates basic MPI setup and process-level parallelism.
    Although the program is short, its output is not predictable: the output
    from the multiple processes (in this case, four) will differ in order if
    the program is run sufficiently many times.

    In this example, there's no IPC: the processes do their own thing in
    splendid isolation.
*/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>      

int main() {
  /** MPI setup **/
  int rank = -1, size = -1;    /** size == number of processes, rank == id of each process **/

  MPI_Init(0, 0);              /** traditionally &argc and &argv **/

  /** MPI_COMM_WORLD is a built-in MPI 'communicator' structure. **/
  MPI_Comm_size(MPI_COMM_WORLD, &size);  /** size == number of processes **/
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);  /** rank == MPI id (0,1,2,...,size - 1) per process **/

  /** 'master' process **/
  if (rank == 0) {
    if (size < 4) 
      puts("\nUsage: mpirun -np 4 ./mpiBasics\n");
    
    printf("%i makes me the 'master' process, now with %i 'worker processes'\n", rank, size - 1);
  }
  
  /** 'worker' processes **/
  else if (rank == 1 || rank == 2) 
    printf("Hello, world! from worker %i\n", rank);

  else if (rank == 3) {
    unsigned i, sum = 0;
    for (i = 1; i <= 1000; i++) sum += i;
    printf("Sum of the 1st 1000 positive integer is %i (courtesy of worker %i)\n",
	   sum, rank);
  }
  
  MPI_Finalize();
  return 0;
}

