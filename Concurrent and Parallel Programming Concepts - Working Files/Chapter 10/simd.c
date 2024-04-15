/** To compile and run:

    gcc -o simd simd.c
    ./simd  # On Windows: simd

 No explicit optimization is required. The GNU and Clang compilers
 do the optimization automatically. 
*/
  
#include <stdio.h>

/* A typedef in C supports 'aliasing' of data types. Here's a simple example:

     typedef unsigned boolean; ## 'unsigned' is short for 'unsigned int'

   This typedef allows 'boolean' to be used in place of 'unsigned', for example:

     boolean flag = 0;  ## 0 is 'false' in C

   The typedef below is more complicated, as it includes special information for the
   compiler, in particuar the '__attribute__' and 'vector_size' terms. Here's the upshot
   of the typedef:

   # 'intV8' aliases 'int', but as a vector (1-dimensional array) rather than as a scalar type.
   # The size of an 'intV8' vector is set to 32 == Length * sizeof(int) == 8 * 4. The size is in bytes.
*/
#define Length 8
typedef int intV8 __attribute__ ((vector_size (Length * sizeof(int)))); /** 32 bytes needed **/
    
int main() {
  intV8 dataV1 = {1, 2, 3, 4, 5, 6, 7, 8}; /* initialize vector dataV1: no array syntax here */                   
  intV8 dataV2 = {8, 7, 6, 5, 4, 3, 2, 1}; /* same for vector dataV2 */
  
  intV8 add = dataV1 + dataV2; /* 9  9  9  9  9  9  9 9 -- no array syntax */
  intV8 mul = dataV1 * dataV2; /* 8 14 18 20 20 18 14 8 -- no array syntax */
  intV8 div = dataV2 / dataV1; /* 8  3  2  1  0  0  0 0 -- no array syntax */
  int i;
  for (i = 0; i < Length; i++) printf("%i ", add[i]); /* array syntax */
  putchar('\n');
  return 0;
}
