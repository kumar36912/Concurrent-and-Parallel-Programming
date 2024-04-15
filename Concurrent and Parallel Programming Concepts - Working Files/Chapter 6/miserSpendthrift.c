/** To compile: gcc -o miserSpendthrift miserSpendthrift.c -lpthread **/

/** The problem:

         increment  +---------+  decrement
   miser----------->| account |<-----------spendthrift
                    +---------+
 */

#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

static int balance = 0;  /** shared storage across the threads **/                      /** line 1 **/

void report_and_die(const char* msg) {
   perror(msg);
   exit(0);
}

void* deposit(void* n) {  /** miser code **/                                            /** line 2 **/
   int* ptr = (int*) n;
   int limit = *ptr, i;
   for (i = 0; i < limit; i++) balance++; /* add 1 to balance */                        /** line 3 **/
   return 0; /** nothing to return: NULL **/
} /** thread terminates when exiting deposit **/

void* withdraw(void* n) { /** spendthrift code **/                                      /** line 4 **/
   int* ptr = (int*) n;
   int limit = *ptr, i;
   for (i = 0; i < limit; i++) balance--; /* subtract 1 from balance */                 /** line 5 **/
   return 0; /** nothing to return: NULL **/
} /** thread terminates when exiting withdraw **/

int main(int argc, char* argv[]) {
  if (argc < 2) {                                                                       /** line 6 **/
      fprintf(stderr, "Usage: miserSpendthrift <number of operations apiece>\n");
      return 0;
   }
   int n = atoi(argv[1]); /** command-line argument conversion to integer **/
   
   pthread_t miser, spendthrift; /* miser increments, spendthrift decrements */         /** line 7 **/
   if (pthread_create(&miser,    /** address of identifier **/                          /** line 8 **/
		      0,         /** attributes? none in this case **/                  /** line 9 **/
		      deposit,   /** function for thread to execute: deposit **/        /** line 10 **/
		      &n) < 0)            
      report_and_die("pthread_create: miser");
   
   if (pthread_create(&spendthrift, 0, withdraw, &n) < 0) /* spendthrift: withdraw */ 
      report_and_die("pthread_create: spendthrift");

   /* Force main thread to wait for the other two to die. */
   pthread_join(miser,  0);                                                             /** line 11 **/
   pthread_join(spendthrift, 0);                                                        /** line 12 **/
   
   /* Print final balance. */
   printf("The final balance is: %16i\n", balance);                                     /** line 13 **/
   return 0;
}
