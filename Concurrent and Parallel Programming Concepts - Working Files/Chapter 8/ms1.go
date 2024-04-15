/** To compile/run from command-line:

   go run ms1.go
*/

/* Synchronization through a mutex to protect access to a shared variable. 

  ## 100 misers, 100 spendthrifts -- each doing its thing 1K times.
  ## Increments and decrements to the balance are by randomly generated values; hence,
     the balance need not (indeed, probably does not) wind up as zero.
*/
package main

import (
   "fmt"
   "math/rand"
   "runtime"
   "sync"
   "sync/atomic"
)

func report(s string, n int32) {
   fmt.Println(s, n)
}

func main() {
   var balance int32 = 0        // shared memory across goroutines                         /** line 1 **/
   var lock = &sync.Mutex{}     // mutex to syncrhonize access to shared memory            /** line 2 **/
   var increments int32 = 0     // tracks deposits done on the balance
   var decrements int32 = 0     // tracks withdrawals done on the balance
   var goroutineCount int = 100 // goroutines to deposit and withdraw                      /** line 3 **/
   var maxAmt int32 = 5         // max value for a randomly generated integer value
   var iterations int = 1000    // per goroutine
   var wg sync.WaitGroup        // wait group to ensure goroutine coordination             /** line 4 **/
   
   // Miser goroutines: they increment the balance
   for i := 0; i < goroutineCount; i++ {
      wg.Add(1)           // increment WaitGroup counter
      go func() {                                                                           /** line 5 **/
         defer wg.Done()  // invoke Done on the WaitGroup when finished                     /** line 6 **/
         for j := 0; j < iterations ; j++ {
            incr := rand.Int31n(maxAmt) + 1
	    
            lock.Lock()                             // lock access                          /** line 7 **/
            balance += incr                         
            lock.Unlock()                           // unlock                               /** line 8 **/
	    
	         // report("Despositing  ", incr)        // report on current value
            atomic.AddInt32(&increments, incr)      // under-the-hood locking               /** line 9 **/
            runtime.Gosched()                       // yield to other goroutines, resume automatically
         }
      }()
   }
   
   // Spendthrift goroutines: they decrement the balanace
   for i := 0; i < goroutineCount; i++ {
      wg.Add(1)           // increment WaitGroup counter
      go func() {
         defer wg.Done()  // invoked Done on the WaitGroup when finished
         for j := 0; j < iterations; j++ {
            decr := rand.Int31n(maxAmt) + 1
	    
            lock.Lock()                             // lock access
            balance -= decr
            lock.Unlock()                           // unlock
	    
	         // report("Withdrawing  ", decr)
            atomic.AddInt32(&decrements, decr)      // under-the-hood locking
            runtime.Gosched()                       // play nice--yield to others
         }
      }()
   }

   wg.Wait()  // await completion of miser/spendthrift goroutines                           /** line 10 **/
   report("Final balance:               ", balance)
   report("Total increments:            ", increments)
   report("Total decrements:            ", decrements)
   report("Difference in incr and decr: ", increments - decrements) // should match final balance
}
