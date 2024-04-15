/** Caution for Mac OSX: there's a known compiler bug on the
    reduce method used in example 5 below. Try compiling the
    code in any case, as should be fixed quickly: javac Streams.java
*/

import java.util.Arrays;
import java.util.List;
import java.util.ArrayList;
import java.util.Map;
import java.util.HashMap;
import java.util.stream.Stream;
import java.util.stream.Collectors;
 
public class Streams {
    public static void main(String[ ] args) {	
	List<Integer> list = new ArrayList<Integer>();  // collection to serve as a stream's source
	for (int i = 0; i < 16; i++) list.add(i);       // populate the collection

	/** example 1 **/
	list                                                                                                       
	    .stream()                         // streamify the list: values are made available only 'on demand'
	    .map(n -> n + 1)                  // map each value to its successor (new stream created)
	    .forEach(System.out::println);    // print each of the successor values
	System.out.println();

	/** example 2 **/
	list                                                                     
	    .stream()
	    .map(n -> Arrays.asList(n - 1, n, n + 1))   // a more complicated mapping
	    .forEach(System.out::println);
	System.out.println();

	/** example 3 **/
	list
	    .stream()        // non-parallel stream
	    .map(n -> n + 1)
	    .forEach(n -> System.out.format("%d %s\n", n, Thread.currentThread().getName()));
	System.out.println();

	/** example 4 **/
	list
	    .parallelStream() //*** parallel stream
	    .map(n -> n + 1)
	    .forEach(n -> System.out.format("%d %s\n", n, Thread.currentThread().getName()));
	System.out.println();

	/** example 5 **/
	int intSum = 
	    list
	    .parallelStream()
	    .map(n -> n + 1)
	    // the 3 arguments are: 0 is the 'identity value' == the initial value of 
	    //   the reduction and the default when no more stream values are available
	    // (sum, n) -> sum + n is the 'accumulator', 
	    // and (sum1, sum2) -> sum1 + sum2 is the 'combiner' function
	    .reduce(0, (sum, n) -> sum += n, (sum1, sum2) -> sum1 + sum2);
	System.out.println("\n### The sum: " + intSum);
	System.out.println();
	
	/** example 6 **/
	intSum = 
	    list
	    .parallelStream()
	    .map(n -> n + 1)
	    // the 3 arguments are: 0 is the 'identity value' == the initial value of 
	    //   the reduction and the default when no more stream values are available
	    // (sum, n) -> sum + n is the 'accumulator', 
	    // and (sum1, sum2) -> sum1 + sum2 is the 'combiner' function
	    .reduce(0, 
		    (sum, n) -> { System.out.format("Accumulte: n (%d) and sum (%d)\n", n, sum); 
				  return sum += n; }, 
		    (sum1, sum2) -> { System.out.format("Combine: sum1 (%d) and sum2 (%d)\n", sum1, sum2);
				      return sum1 + sum2; });
    }

}
