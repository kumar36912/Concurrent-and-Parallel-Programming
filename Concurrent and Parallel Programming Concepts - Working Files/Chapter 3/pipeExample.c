/** A first multiprocessing example: pipes in C 
    To compile (Unix-like system): gcc -o pipeExample pipeExample.c
    For Windows, the GNU compiler: https://gcc.gnu.org/
**/
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define ReadEnd  0                                                  /** line 1 **/            
#define WriteEnd 1                                                

void report_and_die(const char* msg) {                              /** line 2 **/
  perror(msg);
  exit(-1);    /** failure **/
}

int main() {
  int pipeFDs[2];                                                   /** line 3 **/
  char buf;         /* 1-byte buffer */
  const char* msg = "This is the winter of our discontent\n";

  if (pipe(pipeFDs) < 0) report_and_die("pipe(...)");               /** line 4 **/

  pid_t cpid = fork();                                              /** line 5 **/
  if (cpid < 0) report_and_die("fork()");                           /** line 6 **/

  /*** child ***/
  if (0 == cpid) {                                                  /** line 7 **/
    close(pipeFDs[WriteEnd]);                                       /** line 8 **/
    while (read(pipeFDs[ReadEnd], &buf, 1) > 0)                     /** line 9 **/
      write(STDOUT_FILENO, &buf, sizeof(buf));                      /** 1ine 10 **/
    close(pipeFDs[ReadEnd]);                                        /** line 11 **/
    _exit(0); /* signal parent of immediate exit **/                /** line 12 **/
  } 
  /*** parent ***/
  else {                                                            /** line 13 **/ 
    close(pipeFDs[ReadEnd]);                                   
    write(pipeFDs[WriteEnd], msg, strlen(msg));                     /** line 14 **/
    close(pipeFDs[WriteEnd]); /* generates an EOF */                /** line 15 **/
    wait(0); /*** wait for child to exit ***/                       /** line 16 **/  
    exit(0); /* exit normally */                                    /** line 17 **/
  }
  return 0;
}
