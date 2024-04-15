#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>

void report_and_die(const char* msg) {
  perror(msg);
  exit(-1);
}

char* setup(key_t key, int byte_count, int flags) {
  /* Get the shared memory and its identifier. */
  int mem_id = shmget(key,        /* user-supplied key */
		      byte_count, /* how many bytes */
		      flags);     /* flags, including access rights */
  if (mem_id < 0) report_and_die("failed on shmget");
  
  /* Attach memory segment to putter's address space. */
  char* mem_ptr = shmat(mem_id, /* identifier */
			0,      /* let the system pick the address (NULL) */
			0);     /* optional flags, none in this case */
  if (mem_ptr == (void*) -1) report_and_die("failed on shmat");
  return mem_ptr;
}

int main() {
  int len = 14;  /* byte count of "Hello, world!" with '\0' included */
  key_t key = 9876;  /* user-supplied key */

  /* Attach memory segment to getter's address space. */
  char* mem_ptr = setup(key, len, 0666);
  *mem_ptr = 'h';   /* change the 'H' to 'h' as a signal to the putter */
  puts(mem_ptr);    /* print the string to confirm */
  return 0;
}
