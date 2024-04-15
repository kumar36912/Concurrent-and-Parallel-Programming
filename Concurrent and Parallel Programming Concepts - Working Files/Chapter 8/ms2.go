/** Miser/spendthrift coordination through channels:
    ## 100 misers, 100 spendthrifts each doing its thing 1K times.
    ## The balance is updated by randomly generated values.

    Program logic:

    The 'banker' goroutine receives requests from miser and
    spendthrift goroutines: only the banker has direct access to the
    account balance, with the miser and spendthrift goroutines sending
    deposit and withdrawal requests to the banker over a channel.

    The banker responds to each miser and spendthrift over a channel as well,
    reporting on the updated balance.

    go run ms2.go
*/
package main

import (
   "fmt"
   "math/rand"
   "runtime"
   "sync"
   "sync/atomic"
)

type miserOp struct {           // miser operation                                       /** line 1 **/
   increment int32              // amount
   response  chan int32         // channel
}

type spendthriftOp struct {     // spendthrift operation                                 /** line 2 **/
   decrement int32              // amount
   response  chan int32         // channel
}

func report(s string, n int32) {
   fmt.Println(s, n)
}

func main() {
	var reportOn bool = false
   var balance int32 = 0       // only the banker manipulates this variable
   var increments int32 = 0    // tracks increments on the shared memory
   var decrements int32 = 0    // tracks decrements on the shared memory
   var limit int = 100         // goroutines to deposit and withdraw
   var maxAmt int32 = 4
   var iterations int = 1000   // per goroutine
   var wg sync.WaitGroup       // wait group to ensure goroutine coordination
   var buffSize int = 2        // channels can be buffered for async behavior

   deposits    := make(chan* miserOp, buffSize)         // channel for deposits          /** line 3 **/
   withdrawals := make(chan* spendthriftOp, buffSize)   // channel for withdrawals       /** line 4 **/

   // The banker: handles all requests for deposits and withdrawals through a channel.
   go func() {
      for {   
         /* The select construct is inherently non-blocking:
              -- if there's something to read from a channel, handle it
              -- if not, fall through to the next case
            And so on.
          */
         select {
         case deposit := <-deposits:                                                     /** line 5 **/
            balance += deposit.increment    // add to the balance
            deposit.response <- balance     // respond with current balance              /** line 6 **/
					
         case withdrawal := <-withdrawals:                                               /** line 7 **/
            balance -= withdrawal.decrement // subtract from the balance
            withdrawal.response <- balance  // respond with updated value of shared      /** line 8 **/
         }
      }
   }()
   
   // misers
   for i := 0; i < limit; i++ {
      wg.Add(1)
      go func() {
         defer wg.Done()
         for j := 0; j < iterations; j++ {
            amt := rand.Int31n(maxAmt)
				
            deposit := &miserOp{increment: amt + 1, response: make(chan int32)}         /** line 9 **/
            deposits <- deposit                                                         /** line 10 **/
            newBalance := <-deposit.response                                            /** line 11 **/
				
				if reportOn {
					report("Deposit results in  ", newBalance)
				}
            atomic.AddInt32(&increments, amt + 1)                                       /** line 12 **/
            runtime.Gosched()
         }
      }()
   }
   
   // spendthrifts
   for i := 0; i < limit; i++ {
      wg.Add(1)
      go func() {
         defer wg.Done()
         for j := 0; j < iterations; j++ {
            amt := rand.Int31n(maxAmt)
				
            withdrawal := &spendthriftOp{decrement:  amt + 1, response: make(chan int32)} // writeOp instance
            withdrawals <- withdrawal                                                     // send to channel
            newBalance := <-withdrawal.response                                           // await response
				
            if reportOn {
					report("Withdrawal results in ", newBalance)
				}
            atomic.AddInt32(&decrements, amt + 1)                                         // update decrements
            runtime.Gosched();
         }
      }()
   }

   wg.Wait() // await completion of readers and writers
   report("Final balance:               ", balance)
   report("Total increments:            ", increments)
   report("Total decrements:            ", decrements)
   report("Difference in incr and decr: ", increments - decrements)
}

