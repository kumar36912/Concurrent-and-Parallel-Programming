/* To compile and run:

   gcc -o epoller epoller.c
   ./epoller  # on Windows: epoller

   Run this after starting the fifoWriter.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <fcntl.h>

#define MaxEvents    12
#define MaxReads  12000

/* the application logic is here */
unsigned is_prime(unsigned n) {                                /** line 1 **/
  if (n <= 3) return n > 1;
  if (0 == (n % 2) || 0 == (n % 3)) return 0;
  unsigned i;
  for (i = 5; (i * i) <= n; i += 6) 
    if (0 == (n % i) || 0 == (n % (i + 2))) return 0;
  printf("%i is prime.\n", n);
  return 1; /** true in C **/
}

void report_and_die(const char* msg) {
  perror(msg);
  exit(-1);
}

int create_nonblocking(const char* name) {                     /** line 2 **/
  int fd = open(name, O_RDONLY);                               /** line 3 **/
  if (fd < 0) return fd;     /** error **/
  int flag = fcntl(fd, F_SETFL, O_NONBLOCK);                   /** line 4 **/
  if (flag < 0) return flag; /** error **/
  return fd;
}
 
int main() {
  const char* file = "./pipe1";
  int fd = create_nonblocking(file);                           /** line 5 **/
  if (fd < 0) report_and_die("create_nonblocking(file)");

  int epollFD = epoll_create1(0);                              /** line 6 **/
  if (epollFD < 0) report_and_die("epoll_create1(0)");
  
  struct epoll_event toKernel;                                 /** line 7 **/
  struct epoll_event fromKernel[MaxEvents];                    /** line 8 **/

  toKernel.events = EPOLLIN;  /* incoming == read events */    /** line 9 **/
  toKernel.data.fd = fd;      /* listen for reads on FIFO */   /** line 10 **/
  if (epoll_ctl(epollFD, EPOLL_CTL_ADD, fd, &toKernel) < 0)    /** line 11 **/
    report_and_die("epoll_ctl(...)");
  
  unsigned howmany = 0;
  int count = 0;
  while (count++ < MaxReads) {
    int n = epoll_wait(epollFD, fromKernel, MaxEvents, -1);    /** line 12 **/
    if (n < 0) report_and_die("epoll_wait(...)");
    /* Handle the events with read operations. */
    int i;
    for (i = 0; i < n; i++) {
      int next = 0;
      int bytes_read = read(fromKernel[i].data.fd,             /** line 13 **/
			    &next, 
			    sizeof(int));                     
      if (sizeof(int) == bytes_read)   /* success */           /** line 14 **/
	if (is_prime(next)) howmany++; /* app logic */
    }
  }
  /* Clean up. */
  close(fd);
  close(epollFD);
  unlink(file);

  /* Report on the number of primes found. */
  printf("\n%u primes were found among the %i values read.\n", howmany, MaxReads);
  return 0;
}
