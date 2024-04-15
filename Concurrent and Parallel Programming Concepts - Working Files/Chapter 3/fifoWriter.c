#include <fcntl.h> 
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MaxWrites 256  

int main() {
  srand(time(0)); /* seed the random number generator */

  const char* fifoName = "./myPipe1";          
  mkfifo(fifoName, 0666); /* read/write for user/group/others */             /** line 1 **/
  int fd = open(fifoName, O_CREAT | O_WRONLY); /* open blocks on a FIFO */   /** line 2 **/

  int i;
  for (i = 0; i < MaxWrites; i++) { 
    int n = rand();             
    write(fd, &n, sizeof(int));  /* 4 bytes per int */                       /** line 3 **/
  }
  close(fd);         /* close the fifo */                                    /** line 4 **/
  unlink(fifoName);  /* dispose of underlying file */                        /** line 5 **/
  return 0;
}

