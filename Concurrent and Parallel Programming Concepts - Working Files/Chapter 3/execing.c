/* To run: 

   -- compile this program (gcc -o execing execing.c) and fileStatus.c (gcc -o fileStatus fileStatus.c)
   -- create a subdirectory named tmp: mkdir tmp
   -- from the command line: ./execing

   This program 'execs' the fileStatus program, which outputs metadata about the tmp directory.
   Here's a depiction:

                   execv
   execing program------->fileStatus program
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
  char* const args[] = {"./fileStatus", "tmp", 0};  /* fileStatus program, tmp directory, 0 to terminate array */
  int ret = execv("./fileStatus", args);            /* "v" for "vector" */
  if (-1 == ret) {                                  /* check for failure */
    perror("execv(...) error");
    exit(-1);
  }
  else 
    printf("I'm here\n");                           /* never executes */
  return 0;
}

