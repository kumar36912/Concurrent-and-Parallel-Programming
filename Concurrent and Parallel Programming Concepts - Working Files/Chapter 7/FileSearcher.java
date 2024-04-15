/** javac FileSearcher.java
    java MainFS

    The program searches from the user's home directory for
    files extending with '.txt' as the extension. The list of
    such files is printed to the screen.
*/

import java.io.File;
import java.util.ArrayList;
import java.util.List;
import java.util.Collection;
import java.util.concurrent.RecursiveTask;                                           /** line 0 **/
 
public class FileSearcher extends RecursiveTask<List<String>> {                      /** line 1 **/
    private final String path;
    private final String extension;
 
    public FileSearcher(String path, String extension) {                             /** line 2 **/
	this.path = path;
	this.extension = extension;
    }
    
    @Override
    public List<String> compute() {                                                  /** line 3 **/
	List<String> listOfFileNames = new ArrayList<String>();
	File[ ] files = new File(path).listFiles();                                  /** line 4 **/
	if (files == null) return listOfFileNames; // base case                      /** line 5 **/

	List<FileSearcher> tasks = new ArrayList<FileSearcher>();                    /** line 6 **/
	for (File file : files) {                                                    /** line 7 **/
	    String absolutePath = file.getAbsolutePath();                            /** line 8 **/
	    if (file.isDirectory()) {                                                /** line 9 **/
		FileSearcher task = new FileSearcher(absolutePath, extension);       /** line 10 **/
		task.fork(); // recursive case                                       /** 1ine 11 **/
		tasks.add(task);                                                     /** line 12 **/
	    }
	    else if (file.getName().endsWith(extension))                             /** line 13 **/
		listOfFileNames.add(absolutePath);
	}
	assembleResults(listOfFileNames, tasks);                                     /** line 14 **/
	return listOfFileNames;                                                      /** line 15 **/
    }
    private void assembleResults(List<String> list, List<FileSearcher> tasks) {
	for (FileSearcher task : tasks) {
	    Collection<String> results = task.join();                                /** line 16 **/
	    list.addAll(results);                                                    /** line 17 **/
	}
    }
}
class MainFS {
    public static void main(String[ ] args) {
	List<String> list = 
	    new FileSearcher(System.getProperty("user.home"), ".txt").compute();     /** line 18 **/
	System.out.println("Files with extension .txt:");
	for (String string : list) System.out.println(string);                       /** line 19 **/
    }
}
