#include <stdio.h>  

/** This is the base version of the msOMP program, with 'ms' for miser/spendthrift.
    There's no race condition because there's no multithreading. Indeed, there's no use of OpenMP.
    This file is compiled and run in the usual way:
    gcc -o msOMP1 msOMP1.c
    ./msOMP1  ## on Windows: msOMP1
**/
static float balance = 0.0f;

int main() {
  const int how_many = 8000000;  /** 8,000,000 **/
  unsigned i;
  for (i = 0; i < how_many; i++) {
    balance++; /** miser **/
    balance--; /** spendthrift **/
  }
  printf("After %i deposits and %i withdrawals, the balance is: %f.\n",
	 how_many, how_many, balance); /** balance is zero **/
  return 0;
}
