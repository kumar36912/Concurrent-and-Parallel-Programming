/** Data parallelism usimg OpenMP:

    The program converts, in parallel, N Fahrenheit temperatures in Centrigrade,
    where N is the number of processors on the host system. The temperatures to
    be converted are generated as random values in the range of 0.0 through 99.9.

   gcc -fopenmp tempConv.c -o tempConv
   ./tempConv  # on Windows: tempConv
*/

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N (100.0)

/* Generate values between 0.0 and 99.9. */
double my_rand() {
  return ((double) rand() / ((double) RAND_MAX + 1) * N);
}

void f2c(double f) {
  double c = (5.0 / 9.0) * (f - 32.0); 
  printf("%.2fF is %.2fC\n", f, c);
}

int main() {
  srand(time(0));

  #pragma omp parallel 
  {
    f2c(my_rand());
  }
  
  puts("That's all, folks!");  
  return 0;
}
/** Output from a sample run:

    13.01F is -10.55C
    22.61F is -5.22C
    96.56F is 35.87C
    82.59F is 28.10C
    39.75F is 4.31C
    7.27F is -13.74C
    80.45F is 26.92C
    43.93F is 6.63C
    That's all, folks!
*/
