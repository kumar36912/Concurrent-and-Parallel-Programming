package acct;

public class Account {
    public static int balance = 0;                                             /** line 1 **/
}

class Miser extends Thread {       // deposit                                  /** line 2 **/
    Miser(int howMany) { this.howMany = howMany; }

    @Override
    public void run() {
	for (int i = 0; i < howMany; i++) 
	    Account.balance++;
    }
    private int howMany; // how many times to increment
}

class Spendthrift extends Thread { // withdraw                                 /** line 3 **/
    Spendthrift(int howMany) { this.howMany = howMany; }

    @Override
    public void run() {
	for (int i = 0; i < howMany; i++) 
	    Account.balance--;
    }
    private int howMany; // how many times to decrement
}

/************************************************************************/

package acct;
public class RaceCondition {                                                   /** line 4 **/
    public static void main(String[ ] args) {
	if (args.length < 1) {
	    System.err.println("RunCondition <times to iterate>");
	    return;
	}
	int n = Integer.parseInt(args[0]);
	Miser miser = new Miser(n);                                            /** line 5 **/
	Spendthrift spendthrift = new Spendthrift(n);                          /** line 6 **/
	miser.start();       // start Miser                                    /** line 7 **/
	spendthrift.start(); // start Spendthrift */                           /** line 8 **/
	try {                                                          
	    miser.join();       // wait for Miser to terminate                 /** line 9 **/
	    spendthrift.join(); // wait for Spendthrift to terminate           /** 1ine 10 **/
	} catch(Exception e) { System.err.println(e); }
	System.out.println("Final balance: " + Account.balance);     
    }
}
    
