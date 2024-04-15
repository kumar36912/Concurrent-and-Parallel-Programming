/**
   gcc -fopenmp hello.c -o hello
*/
#include <stdio.h>  
#include <omp.h>   /** header file for OpenMP library **/

/** This is the race-condition version of the msOMP program, with 'ms' for miser/spendthrift.
    There's multithreading and a race condition.
**/
static float balance = 0.0f;

int main() {
  const int how_many = 8000000;  /** 8,000,000 **/
  unsigned i;

  #pragma omp parallel for                /** enable multhreading on the loop **/
  for (i = 0; i < how_many; i++) {
    balance++; /** miser **/
    balance--; /** spendthrift **/
  } /** end of parallel section: implicit wait **/
  
  printf("After %i deposits and %i withdrawals, the balance is: %f.\n",
	 how_many, how_many, balance); /** balance is zero **/
  return 0;
}
