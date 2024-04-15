/** To compile and run from the command-line:
      javac Deadlock.java
      java Deadlock
*/
public class Deadlock {
    static Object lock1 = new Object(); // a single lock1                   /** line 1 **/
    static Object lock2 = new Object(); // a single lock2
    
    /** Each thread executes its run() method. **/
    public static void main(String args[]) {
	Thread thread1 = new Thread() {
		public void run() {                            
		    synchronized(lock1) {                                   /** line 2 **/
			print("thread1 holds lock1");
			try { Thread.sleep(2); } catch(Exception e) { }     /** line 3 **/
			print("thread1 waiting for lock2");
			print("\t(thread1 needs lock2 to release lock1...)");
			synchronized(lock2) {                               /** line 4 **/
			    print("thread1 holds lock1 and lock2");
			} // lock2 released here                            /** line 5 **/
		    } // lock1 released here                                /** line 6 **/
		} 
	    };
	Thread thread2 = new Thread() {
		public void run() {
		    synchronized(lock2) {                                   /** line 7 **/
			print("thread2 holds lock2");
			try { Thread.sleep(2); } catch(Exception e) { }
			print("thread2 waiting for lock1");
			print("\t(thread2 needs lock1 to release lock2...)");
			synchronized(lock1) {                               /** line 8 **/
			    print("thread2 holds lock2 and lock1");
			} // lock1 released here                            /** line 9 **/
		    } // lock2 released here                                /** line 10 **/
		}
	    };
	thread1.start();                                                    /** line 11 **/
	thread2.start();                                                    /** line 12 **/
    }
    private static void print(Object obj) { System.out.println(obj); }
}

