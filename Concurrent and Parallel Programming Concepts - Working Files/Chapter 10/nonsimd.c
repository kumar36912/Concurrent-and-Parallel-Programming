/** To compile and run:

    gcc -o nonsimd nonsimd.c
    ./nonsimd  # On Windows: simd
*/
  
#include <stdio.h>

#define Length 8

int main() {
  int array1[ ] = {1, 2, 3, 4, 5, 6, 7, 8}; 
  int array2[ ] = {8, 7, 6, 5, 4, 3, 2, 1};
  int sum[Length];

  int i;
  for (i = 0; i < Length; i++) sum[i] = array1[i] + array2[i]; /** serial computation **/
  
  for (i = 0; i < Length; i++) printf("%i ", sum[i]); 
  putchar('\n');
  return 0;
}
/** A better way: SSE = Streaming SIMD Extensions = additional instruction set, more CPU registers

In the late 1990s, Intel introduced an SSE instruction set for their Pentium III
processors. This became known as the MMX instruction set. (Officially, MMX is not an
acronym.) AMD earlier introduced a comparable instruction set known as 3DNow!. In any case, 
the hardware support included new registers with names that start with 'xmm'. In the original
SSE, there was support only for integer operations.

SEE has progressed, with new versions: SSE2, SSE3, SSSE3, and SSE4. The newer versions bring
SIMD support for both integer and floating-point operations. At the same time, C/C++ compilers 
such as GNU and Clang provided support, eventually automatic, for SIMD-friendly code. Nowadays,
both compilers are capable of 'automatic vectorization' of approrpiately written source code.

The short version: elements in the arrays 'array1' and 'array2' should be added pairwise in
parallel in order to boost performance.
*/
