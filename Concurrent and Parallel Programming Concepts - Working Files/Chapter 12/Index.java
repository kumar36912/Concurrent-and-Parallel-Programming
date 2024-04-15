import java.util.List;
import java.util.ArrayList;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.Arrays;
import java.util.stream.Collectors;

/** The program picks up the 'scatter/gather' midstream: the book's pages have been
    scattered behind the scenes to eight editors, each of whom submits an ordered list
    of the pages on which a specified word (in this example, 'concurrent') occurs.
    However, the lists of pages come back in arbitrary order, as reflected in the code below.
    The program now illustrates some additional aspects of the 'scatter/gather' pattern.

    To compile and run:

    javac Index.java      ## Should compile without issue even on a Mac (Java 8 or greater required)
    java IndexMain
*/
public class Index {
    private Map<String, List<Integer>> index;
    public Index() {
	index = new ConcurrentHashMap<String, List<Integer>>();
    }
    public void addEntry(final String word, final List<Integer> list) {
	List<Integer> tempList = null;
	if (index.containsKey(word)) {       // if there's already an entry, append the new list to the current one
	    tempList = index.get(word); 
	    tempList.addAll(list);           // add the new list's items
	}
	else
	    tempList = list;                 // otherwise, this is the 1st occurrence of the word/pages entry

	tempList.sort((n1, n2) -> n1.compareTo(n2)); // sort in ascending order
	index.put(word, tempList);                   // add the entry
    }
    public List<Integer> getEntry(final String word) {
	return index.get(word);
    }
}

class IndexMain {
    private void log(String msg) {
	System.out.println(msg);
    }

    private List<List<Integer>> init() {
	List<List<Integer>> lol = new ArrayList<List<Integer>>();
	lol.add(new ArrayList<Integer>(Arrays.asList(162,176,187,199)));          // editor 4
	lol.add(new ArrayList<Integer>(Arrays.asList(110,132,133)));              // editor 3
	lol.add(new ArrayList<Integer>(Arrays.asList(4,18,32,43,44,48)));         // editor 1
	lol.add(new ArrayList<Integer>(Arrays.asList(362,376,387,388,399)));      // editor 8
	lol.add(new ArrayList<Integer>(Arrays.asList(54,65,76,89,90)));           // editor 2
	lol.add(new ArrayList<Integer>(Arrays.asList(254,265,276,289,290)));      // editor 6   
	lol.add(new ArrayList<Integer>(Arrays.asList(310,311,332,333,345)));      // editor 7
	lol.add(new ArrayList<Integer>(Arrays.asList(201,203,207,219,227,231)));  // editor 5
	return lol;
    }

    // Reduce a list of lists such as [[1,2,3],[9,8,7,6],...] to [1,2,3,9,8,7,6,...]
    List<Integer> flatten(final List<List<Integer>> lol) {
	List<Integer> flatList = 
	    lol
	    .parallelStream()                  // parallelize the work across worker threads
	    .flatMap(n -> n.stream())          // 'flatten' a list-of-lists-of-ints to a list of ints
	    .collect(Collectors.toList());     // aggregate the pieces from the workers into a single list
	return flatList;
    }

    private void doIt() {
	List<List<Integer>> lol = init();    // looks like: [[7, 8, 9], [1, 2, 3],...]
	List<Integer> pages = flatten(lol);  // looks like: [7, 8, 9, 1, 2, 3,...]

	Index index = new Index();
	index.addEntry("concurrent", pages);
	log("concurrent ==> " + index.getEntry("concurrent"));

	// For good measure, add the stuff from the always late editor 9.
	index.addEntry("concurrent", Arrays.asList(401, 402, 417, 435));
	log("\nAfter new addition: " + index.getEntry("concurrent"));
    }

    public static void main(String[ ] args) {
	new IndexMain().doIt();
    }
}
