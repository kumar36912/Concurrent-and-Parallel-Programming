/** javac Semaphores.java
    java Semaphores

    This program uses an ExecutorService and Semaphores
    in a simulation of a ski rental service.
*/
import java.util.concurrent.Executors;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Semaphore;                                                 /** line 1 **/
import java.util.Random;

final class SkiPair {                                                                  /** line 2 **/
    private String name;
    private boolean inUse;

    public SkiPair(String name, boolean inUse) { 
	setName(name); 
	setInUse(inUse);
    }
    public String getName() { return this. name; }
    public void setName(String name) { this.name = name; }
    public void setInUse(boolean inUse) { this.inUse = inUse;  }
    public boolean isInUse() { return this.inUse; }
}

final class SkisForRent {
    public static final int MaxSkiPairs = 150;                                         /** line 3 **/
    private static final int MinInStock = 5;                                           /** line 4 **/
    private final Semaphore semaphore = new Semaphore(MaxSkiPairs - MinInStock);       /** line 5 **/
    private SkiPair[ ] inventory;

    SkisForRent() {
	inventory = new SkiPair[MaxSkiPairs];
	for (int i = 0; i < MaxSkiPairs; i++) inventory[i] = new SkiPair("skiPair-" + i, false);
    }

    public SkiPair rentSkiPair() throws Exception {
	semaphore.acquire();  // get the ticket (acquire() is thread-safe              /** line 6 **/
	return getSkiPair();  // use tick to retrieve pair                             /** line 7 **/
    }
    public void returnSkiPair(SkiPair sp) {
	if (isReturnable(sp)) semaphore.release(); // release() is thread-safe         /** line 8 **/
    }

    protected synchronized SkiPair getSkiPair() {                                      /** line 9 **/
	for (SkiPair sp : inventory) 
	    if (!sp.isInUse()) {
		sp.setInUse(true);
		return sp;
	    }
	return null; // out of luck
    }
    protected synchronized boolean isReturnable(SkiPair sp) {                          /** line 10 **/
	if (sp.isInUse()) {
	    sp.setInUse(false);
	    return true;
	}
	return false;
    }
}

public class Semaphores {
    public static void main(String[] args) {
	Random rand = new Random();
	final SkisForRent sfr = new SkisForRent();
	
	Runnable runnable = new Runnable() {                                           /** line 11 **/
		@Override
		public void run() {
		    String name = Thread.currentThread().getName();
		    try {
			while (true) {                                                 /** line 12 **/
			    SkiPair sp = sfr.rentSkiPair();
			    System.out.printf("%s renting %s%n", name, sp.getName());
			    Thread.sleep(rand.nextInt(2000));  // skiing
			    System.out.printf("%s returning %s%n", name, sp.getName());
			    sfr.returnSkiPair(sp);
			}
		    }
		    catch (Exception e) { System.err.println(e); }
		}
	    };
	ExecutorService[ ] executors =                                                 /** line 13 **/
	    new ExecutorService[SkisForRent.MaxSkiPairs + 1];                       
	for (ExecutorService executor : executors) {
	    executor = Executors.newSingleThreadExecutor();                            /** line 14 **/
	    executor.execute(runnable);                                                /** line 15 **/
	}
    }
}
