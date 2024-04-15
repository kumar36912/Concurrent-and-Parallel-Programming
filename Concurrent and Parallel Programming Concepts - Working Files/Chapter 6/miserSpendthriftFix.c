/** To compile: gcc -o miserSpendthriftFix miserSpendthriftFix.c -lpthread **/

#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

static int balance = 0; /** shared storage across the threads **/                   /** line 1 **/
static pthread_mutex_t lock; /* named lock for clarity; any name would do */        /** line 2 **/

void report_and_die(const char* msg) {
   perror(msg);
   exit(0);
}

void* deposit(void* n) {  /** miser code **/                                          
   int* ptr = (int*) n;
   int limit = *ptr, i;
   for (i = 0; i < limit; i++) {
     if (pthread_mutex_lock(&lock) == 0) {                                               /** line 3 **/
       balance++; /* add 1 to balance */                                                 /** line 4 **/
       pthread_mutex_unlock(&lock);                                                      /** line 5 **/
     }
     else
       report_and_die("pthread_mutex_lock (deposit)");
   }
   return 0; /** nothing to return: NULL **/
}

void* withdraw(void* n) { /** spendthrift code **/                                    
   int* ptr = (int*) n;
   int limit = *ptr, i;
   for (i = 0; i < limit; i++) {
     if (pthread_mutex_lock(&lock) == 0) {
       balance--; /* subtract 1 from balance */
       pthread_mutex_unlock(&lock);
     }
     else
       report_and_die("pthread_mute_lock (withdraw)");
   }
   return 0; /** nothing to return: NULL **/
}

int main(int argc, char* argv[]) {
   if (argc < 2) {
      fprintf(stderr, "Usage: miserSpendthriftFix <number of operations apiece>\n");
      return 0;
   }
   int n = atoi(argv[1]); /** command-line argument conversion to integer **/
   
   pthread_t miser, spendthrift; /* miser increments, spendthrift decrements */          /** line 6 **/
   pthread_mutex_init(&lock, 0); /** initialize the lock **/
   
   if (pthread_create(&miser,  0, deposit,  &n) < 0)      /* miser: deposit */           /** line 7 **/
      report_and_die("pthread_create: miser");
   if (pthread_create(&spendthrift, 0, withdraw, &n) < 0) /* spendthrift: withdraw */ 
      report_and_die("pthread_create: spendthrift");

   /* Force main thread to wait for the other two to die. */
   pthread_join(miser,  0);                                                              /** line 8 **/
   pthread_join(spendthrift, 0);                                                         /** line 9 **/
   pthread_mutex_destroy(&lock); /* destroy the lock, no longer needed */                /** line 10 **/

   /* Print final balance. */
   printf("The final balance is: %i\n", balance);                                         /** line 11 **/
   return 0;
}
