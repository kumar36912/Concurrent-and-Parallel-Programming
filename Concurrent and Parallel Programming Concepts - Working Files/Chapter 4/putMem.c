#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void report_and_die(const char* msg) {
  perror(msg);
  exit(-1);
}

char* setup(key_t key, int byte_count, int flags) {                                        /** line 1 **/
  /* Get the shared memory and its identifier. */
  int mem_id = shmget(key,        /* user-supplied key */                                  /** line 2 **/
		      byte_count, /* how many bytes */
		      flags);     /* flags, including access rights */
  if (mem_id < 0) report_and_die("failed on shmget");
  
  /* Attach memory segment to putter's address space. */
  char* mem_ptr = shmat(mem_id, /* identifier */                                           /** line 3 **/
			0,      /* let the system pick the address (NULL) */
			0);     /* optional flags, none in this case */
  if (mem_ptr == (void*) -1) report_and_die("failed on shmat");
  return mem_ptr;
}

int main() {
  const char* greeting = "Hello, world!";  /* placed in shared memory */
  int len = strlen(greeting) + 1;          /* + 1 for the '\0' string terminator */
  key_t key = 9876;                        /* user-supplied key */
 
  /* Attach memory segment to putter's address space (0666 == read/write but not execute) */
  char* mem_ptr = setup(key, len, IPC_CREAT | 0666);
  memcpy(mem_ptr, greeting, len); /* copy the msg to shared memory */

  /* Hang around until the other process accesses the shared memory. */
  while (*mem_ptr == 'H') /* other process will change 'H' to 'h' */
    sleep(1);
  printf("%s is new msg: putter is exiting...\n", mem_ptr);
  return 0;
}
