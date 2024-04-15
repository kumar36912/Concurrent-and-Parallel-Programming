#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int main() {
  signal(SIGCHLD, SIG_IGN);  /* prevents zombies */  /** line 1 **/

  int n = 777;                                       /** line 2 **/
  printf("Initial value: %i\n", n);
  pid_t pid = fork();                                /** line 3 **/
  if (-1 == pid) {                                   /** line 4 **/
    perror("fork() error");
    exit(0);
  }
  /** parent and child code **/
  if (0 == pid) {                                    /** line 5 **/
    n = n + 10;              
    printf("Add 10:        %i\n", n);                    
  }
  else {                                             /** line 6 **/
    n = n - 10;
    printf("Sub 10:        %i\n", n);
  }
  return 0;
}
/**     
    Is the print order determined? That is, could the print statments
    be executed in arbitrary order? Explain.
 */
/** A more ambitious exercise:
    Convert the forking_server to a 'pre-forking' server by introducing
    two changes:
    1. Create a pool of worker processes at startup, putting them into a 'wait' state.
    2. As clients connection, awaken a process to handle the request, returning the 
       process to the 'wait' state once the request has been handled.

    Hint: Keep the waiting processes in a queue, and use a signal to awaken the
          process at the front of this queue.

 */
