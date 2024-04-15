/** javac AsyncJob.java
    java MainAJ

    This program illlustrates the Callable interface and the Future interface, the
    latter of which represents the result of an asynchronous computation.
*/

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.Callable;                                 /** line 1 **/
import java.util.concurrent.Future;                                   /** line 2 **/
import java.util.concurrent.ExecutorService;                          /** line 3 **/
import java.util.concurrent.Executors;
                                   
// A 'callable' is like a 'runnable' except that the executed
// method is now 'call' instead of 'run'; a 'callable' can return
// a value of a specified type, in this case Long.  
public class AsyncJob implements Callable<Long> {                     /** line 4 **/
    @Override
    public Long call() throws Exception {                             /** line 5 **/
	long sum = 0;
	final int n = 1000;
	for (long i = 0; i < n; i++) sum += i;
	return sum;
    }
}

class MainAJ {
    public static void main(String[ ] args) {
	final int poolSize = 10;
	final int n = 20000; // 20,000
	
	ExecutorService executor =
	    Executors.newFixedThreadPool(poolSize);                   /** line 6 **/
	List<Future<Long>> jobList = new ArrayList<Future<Long>>();   /** line 7 **/

	for (int i = 0; i < n; i++) {
	    Callable<Long> job = new AsyncJob();                      /** line 8 **/
	    Future<Long> pendingJob = executor.submit(job);           /** line 9 **/
	    jobList.add(pendingJob);                                  /** line 10 **/
	}
	

	long sum = 0;
	// Try to retrieve the results.
	for (Future<Long> result : jobList) {                         /** line 11 **/
	    try {
		sum += result.get();                                  /** line 12 **/
	    } 
	    catch (Exception e) { e.printStackTrace(); } 
	}
	executor.shutdown();
	
	System.out.println("List size: " + jobList.size());      	
	System.out.println("Sum: " + sum);
    }
}
/* output (formatted for readability):

   List size:        20,000
   Sum:       9,990,000,000
*/
