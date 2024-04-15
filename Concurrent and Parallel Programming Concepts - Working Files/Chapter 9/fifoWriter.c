/*            
              write ints  +------+  read ints
   fifoWriter------------>| fifo |<-----------epoller
                          +------+
    To compile and run:

     gcc -o fifoWriter fifoWriter.c
     ./fifoWriter ## on Windows: fifoWriter
  
   Run the fifoWriter first, then the epoller.
 */

#include <fcntl.h> 
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#define MaxWrites        3000  /* max times to write */
#define ChunkSize           4  /* how many ints written each time */
#define MaxZs              10  /* max microseconds to sleep */

int main() {
  srand(time(0));  /* seed the random-number generator */

  const char* pipeName = "./pipe1";
  mkfifo(pipeName, 0666);  /* read/write for user/group/others */
  int fd = open(pipeName, O_CREAT | O_WRONLY); /* open as write-only */

  /* Do MaxWrites, ChunkSize ints per write: sleep a bit in-between. */
  int i;
  for (i = 0; i < MaxWrites; i++) { 
    int j;
    for (j = 0; j < ChunkSize; j++) {
      int n = rand();             
      write(fd, &n, sizeof(int)); 
    }
    usleep((rand() % MaxZs) + 1);  
  }
  /* Clean up by closing and then disposing of the underlying file. */
  close(fd);
  unlink(pipeName);  
  return 0;
}
