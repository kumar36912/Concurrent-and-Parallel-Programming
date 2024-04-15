/*** Fixing the miser/spendthrift problem with a thread-safe data type **/

package acct3;
import java.util.concurrent.atomic.AtomicInteger;                     /** line 1 **/

public class Account {
    public static AtomicInteger balance = new AtomicInteger();        /** line 2 **/
}

class Miser extends Thread {       // deposit
    Miser(int howMany) { this.howMany = howMany; }
    @Override
    public void run() {
	for (int i = 0; i < howMany; i++)                  
	    Account.balance.incrementAndGet();                        /** line 3 **/
    }
    private int howMany;
}

class Spendthrift extends Thread { // withdraw
    Spendthrift(int howMany) { this.howMany = howMany; }
    @Override
    public void run() {
	for (int i = 0; i < howMany; i++) 
	    Account.balance.decrementAndGet();                        /** line 4 **/
    }
    private int howMany;
}
