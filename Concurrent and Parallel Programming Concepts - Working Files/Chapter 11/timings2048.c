/** A timing example to contrast serial and parallel execution on typical matrix/vector operations.
    The program uses 'SSE intrinsics', the interface for which is the header file 'xmmintrin.h'.
    
    Some of the code is obscure, a good sign of what's involved as you get ever closer to the metal.

    The program adds two vectors, and then multiples the rows of the sum vector by a third vector.
**/

#include <stdio.h>
#include <xmmintrin.h>
#include <stdint.h>

typedef void (*func)();  /** a function that takes no arguments and returns void **/

#define Length      (2048)
#define AlignedVals    (4)

 /** Ensure 16-byte alignment of vector elements: 16 bytes is 4 ints or 4 floats,
     which comes to 128 bits--the size of an %xmm register. **/
float v1[Length] __attribute__ ((aligned(16))); 
float v2[Length] __attribute__ ((aligned(16)));
float v3[Length] __attribute__ ((aligned(16)));
float r1[Length] __attribute__ ((aligned(16)));
float r2[Length] __attribute__ ((aligned(16)));

/** Initialize with small random values. **/
void init() {
  unsigned i;
  for (i = 0; i < Length; i++) {
    v1[i] = (float) (rand() / RAND_MAX); 
    v2[i] = (float) (rand() / RAND_MAX);  
    v3[i] = (float) (rand() / RAND_MAX); 
  }
}

void in_serial() {
  unsigned i;
  for (i = 0; i < Length; i++) r1[i] = (v1[i] + v2[i]) * v3[i]; /** the math is shown here **/
}

void in_parallel() {
  /** set up for the special vector instructions **/
  __m128* mm_v1 = (__m128*) v1; 
  __m128* mm_v2 = (__m128*) v2;
  __m128* mm_v3 = (__m128*) v3;
  __m128* mm_rv = (__m128*) r2;

  /** Multiply a vector row by a vector: the outer instruction is _mm_mul_ps. 
      _mm_add_ps adds vector elements.
   **/
  unsigned i;
  unsigned n = Length / AlignedVals; /** in this case, n == 512 **/
  for( i = 0; i < n; i++) mm_rv[i] = _mm_mul_ps(_mm_add_ps(mm_v1[i], mm_v2[i]), mm_v3[i]);
}

/** from the Intel developer's guide: in-line assembly for reasonably good timings **/
uint64_t rdtsc() {
    uint32_t lo, hi;
    __asm__ __volatile__ (
      "xorl %%eax, %%eax\n"
      "cpuid\n"
      "rdtsc\n"
      : "=a" (lo), "=d" (hi)
      :
      : "%ebx", "%ecx");
    return (uint64_t) hi << 32 | lo;
}

unsigned long long time_it(const char* msg, func func2run) {
  unsigned long long start, stop, diff;
  start = rdtsc();
  func2run();
  stop = rdtsc();
  diff = stop - start;
  printf("%s: %lld\n", msg, diff);
  return diff;
}

int main() {
  init();
  unsigned long long sdiff = time_it("in_serial()  ", in_serial);
  unsigned long long pdiff = time_it("in_parallel()", in_parallel);
  printf("Speedup for vector size %i: %.2f\n", Length, (double) sdiff / (double) pdiff);
  return 0;
}
