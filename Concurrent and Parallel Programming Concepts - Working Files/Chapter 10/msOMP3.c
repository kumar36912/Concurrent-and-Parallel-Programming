#include <stdio.h>  
#include <omp.h>   /** header file for OpenMP library **/

/** This is the base version of the msOMP program, with 'ms' for miser/spendthrift.
    There's no race condition because there's no multithreading. 
**/
static float balance = 0.0f;

int main() {
  const int how_many = 8000000;  /** 8,000,000 **/
  unsigned i;
    
  #pragma omp parallel for                      /** line 1 **/
  for (i = 0; i < how_many; i++) {

     #pragma omp atomic                         /** line 2 **/
     balance++; /** miser **/

     #pragma omp atomic                         /** line 3 **/
     balance--; /** spendthrift **/
  } /** end of parallel section **/
  
  printf("After %i deposits and %i withdrawals, the balance is: %f.\n",
	 how_many, how_many, balance); /** balance is zero **/
    return 0;
}
