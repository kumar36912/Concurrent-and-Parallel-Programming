/** javac StackTS.java
    java MainTS

    Kill at the command-line with Control-C.
*/

/** A thread-safe producer/consumer application with thread
    coordination and cooperation.
*/

import java.util.Random;

public class StackTS {
    private static final int capacity = 8;                      
    private int top = -1; // empty stack when top == -1
    private int[ ] stack = new int[capacity];

    // When an entire method is synchronized, the implicit lock is the current object--'this' in Java.
    // Hence, methods push and pop are effectively locked together: only one may be accessed at a time.
    public synchronized void push(Integer n) {             /** line 1 **/
	while ((top + 1) == capacity) { // full?           /** line 2 **/
	    try {
		wait(); // if so, wait for a pop           /** line 3 **/
	    } catch(InterruptedException e) { }
	}
	log(n + " pushed at " + (top + 1));                /** line 4 **/
	stack[++top] = n;                                  /** line 5 **/
	notifyAll();                                       /** line 6 **/
    }

    public synchronized void pop() {                       /** line 7 **/
	while (top < 0) { // empty?                        /** line 8 **/
	    try {
		wait();                                    /** line 9 **/
	    } catch(InterruptedException e) { }
	}
	log(stack[top] + " popped at " + top);             /** line 10 **/
	top--;                                             /** line 11 **/
	notifyAll();                                       /** line 12 **/
    }

    private void log(String msg) {
	System.out.println(msg);
    }
}

class Pusher extends Thread {
    private Random rand = new Random();
    private StackTS stack;

    Pusher(StackTS stack) { this.stack = stack; }           /** line 12 **/
    
    @Override 
    public void run() {
	while (true) {                                      /** line 13 **/
	    stack.push(rand.nextInt(100)); // 0 through 99
	    try {
		Thread.sleep(rand.nextInt(200)); // sleep 0 to 199 milliseconds
	    }  catch(InterruptedException e) { }
	}
    }
}

class Popper extends Thread {
    private Random rand = new Random();
    private StackTS stack;
    
    Popper(StackTS stack) { this.stack = stack; }            /** line 14 **/
    
    @Override
    public void run() {
	while (true) {
	    stack.pop();                                     /** line 15 **/
	    try {
		Thread.sleep(rand.nextInt(100));
	    } catch(InterruptedException e) { }
	}
    }
}

class MainTS {
    public static void main(String[ ] args) {
	StackTS stack = new StackTS();                       /** line 16 **/
	new Popper(stack).start();                           /** line 17 **/
	new Pusher(stack).start();                           /** line 18 **/
	new Pusher(stack).start();                           /** line 19 **/
    }
}
