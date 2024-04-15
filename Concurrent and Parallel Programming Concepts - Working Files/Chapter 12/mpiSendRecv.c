/** To compile and run:

    mpicc -o mpiSendRecv mpiSendRecv.c
    mpirun -np N ./mpiSendRecv  ## try values for N such as 2, 4, 8, and 16

    This program illustrates message-sending and message-receiving in MPI.
    The messages flow in a 'round-robin' fashion: the 'master' process sends
    a message to worker1, which sends one to worker2,...,which sends a message
    back to the 'master' process. At this point, the program terminates.
*/

#include <stdio.h>    
#include <string.h>   
#include <mpi.h>      

#define MaxLength (64)

void report(char* title, int rank, char digitsIn[ ]) {
  printf("%s %2i received: %s\n", title, rank, digitsIn);
}

/** All processes execute the code in main and report: an if-else structure is
    is used to divide the labor between the 'master', on the one hand, and the 
    'workers', on the other hand.
*/
int main() {
  /** data buffers **/
  char digitsOut[MaxLength + 1]; /** string terminator **/
  char digitsIn[MaxLength + 1];
  memset(digitsOut, '\0', sizeof(digitsOut)); /** clear the buffer **/
  memset(digitsIn, '\0', sizeof(digitsIn));   /** ditto **/

  /** MPI setup **/
  int rank = -1, size = -1;
  MPI_Status status;

  MPI_Init(0, 0);                        /** traditionally &argc and &argv **/      
  MPI_Comm_size(MPI_COMM_WORLD, &size);  /** size == number of processes **/
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);  /** rank == MPI id (0,1,2,...,size - 1) per process **/

  /** 'master' process **/
  if (rank == 0) {                     /** the 'master' process: the rest are 'workers' **/
      if (size < 2) 
	puts("\nUsage: mpirun -np N ./mpiBasics for N > 1");

      sprintf(digitsOut, "%i", rank);           

      /** Blocking send: function blocks until the specified data arrive at the destination. **/
      MPI_Send(digitsOut,                /** data to send **/
	       strlen(digitsOut) + 1,    /** count of data items (+ 1 is for the '\0') **/
	       MPI_CHAR,                 /** data type of each item **/
	       rank + 1,                 /** destination process by 'rank' (id) **/
	       1,                        /** user-defined data **/
	       MPI_COMM_WORLD);          /** built-in communicator **/

      /** Blocking receive: function blocks until the specified number of items are in the buffer. **/
      MPI_Recv(digitsIn,                 /** buffer to receive data **/
	       MaxLength,                /** buffer size **/
	       MPI_CHAR,                 /** data type of each buffer element **/
	       size - 1,                 /** sending process **/
	       1,                        /** user-defined data **/
	       MPI_COMM_WORLD,           /** built-in communicator **/
	       &status);                 /** MPI meta-data **/
      report("Master process", rank, digitsIn);
  }
  /** 'worker' processes **/
  else {                    
    MPI_Recv(digitsIn,                      
	     MaxLength,       /** MaxLength because receiver doesn't know how many are coming. **/                      
	     MPI_CHAR,                   
	     MPI_ANY_SOURCE,  /** for generality--don't have to hardcode the source **/          
	     1,                         
	     MPI_COMM_WORLD,            
	     &status);                   
    
    report("Worker process", rank, digitsIn);
    
    sprintf(digitsOut, "%s %i", digitsIn, rank);
    MPI_Send(digitsOut,                    
	     strlen(digitsOut) + 1,        
	     MPI_CHAR,                      
	     (rank + 1) % size,  /** next worker in line, or master if there is none **/
	     1,                             
	     MPI_COMM_WORLD);               
  }
  
  MPI_Finalize();
  return 0;
}
