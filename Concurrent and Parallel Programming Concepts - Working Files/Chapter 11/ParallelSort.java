/** A Java program to illustrate the potential speedup of parallelism, in this
    case for sorting: the program uses the new (as of Java 8) Arrays.parallelSort
    method, and also the Arrays.parallelSetAll method for parallel initialization
    of the roughly 4M array of integers.

    As noted, Java 8 or greater is requied.
*/
import java.util.Random;
import java.util.Arrays;

public class ParallelSort {
    private int[ ] array2Sort;
    
    public static void main(String[ ] args) {
	new ParallelSort().doIt();
    }
    private void doIt() {
	final int arraySize = 1024 * 1024 * 4;  // about 4M: 4,194,304     /** line 1 **/

	//** serial sort
        init(arraySize);
	long serial = sortArray(array2Sort, false);                        /** line 2 **/

	//** parallel sort
	init(arraySize);
	long parallel = sortArray(array2Sort, true);                       /** line 3 **/

	//** report
	log("Serial sort of array of size:   " + arraySize);
	log("Elapsed time: " + serial);
	log("");

	log("Parallel sort of array of size: " + arraySize); 
	log("Elapsed time: " + parallel);

	log("");
	double ratio = (double) serial / (double) parallel;
	if (parallel < serial) 
	    log("Speedup:  " + ratio);
	else
	    log("Slowdown: " + ratio);
    }
    private void init(int size) {
	array2Sort = new int[size];
	/** parallel initialization of array elements **/
	Arrays.parallelSetAll(array2Sort,                                  /** line 4 **/ 
			      i->new Random().nextInt());                  /** line 5 **/
    }
    private long sortArray(int[ ] array, boolean parallel) {
	long start = System.currentTimeMillis();
	/** parallel version of merge-sort **/
	if (parallel) Arrays.parallelSort(array);                          /** line 6 **/
	/** variant of quicksort ('dual pivot') **/
	else Arrays.sort(array);                                           /** line 7 **/
	long stop = System.currentTimeMillis();
	return stop - start;
    }
    private void log(String msg) {
	System.out.println(msg);
    }
}
