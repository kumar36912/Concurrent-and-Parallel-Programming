/*** Fixing the miser/spendthrift problem with explicit thread synchronization. ***/

package acct2;

public class Account {
    public static int balance = 0;                            /** line 1 **/
    public static final Object lock = new Object();           /** line 2 **/
}

class Miser extends Thread {       // deposit                 /** line 3 **/
    Miser(int howMany) { this.howMany = howMany; }

    @Override
    public void run() {
	for (int i = 0; i < howMany; i++) 
	    synchronized(Account.lock) {                      /** line 4 **/        
		Account.balance++;                            /** line 5 **/
	    }
    }
    private int howMany;    
}

class Spendthrift extends Thread { // withdraw                /** line 6 **/
    Spendthrift(int howMany) { this.howMany = howMany; }

    @Override
    public void run() {
	for (int i = 0; i < howMany; i++) 
	    synchronized(Account.lock) {                      /** line 7 **/
		Account.balance--;                            /** line 8 **/
	    }
    }
    private int howMany;
}

