/** javac ProducerConsumerQueue.java
    java ProducerConsumerQueue
*/
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.SynchronousQueue;
import java.util.Random;

public class ProducerConsumerQueue {
    public static void main(String[ ] args) {
        BlockingQueue<String> dropbox = new SynchronousQueue<String>();  /** line 1 **/
        new Thread(new Producer(dropbox)).start();                       /** line 2 **/
	new Thread(new Consumer(dropbox, 12)).start();                   /** line 3 **/
	new Thread(new Producer(dropbox)).start();                       /** line 4 **/
    }
}

class Producer implements Runnable {
    private BlockingQueue<String> dropbox;

    Producer(BlockingQueue<String> dropbox) {
        this.dropbox = dropbox;
    }

    @Override
    public void run() {                                                  /** line 5 **/
        String importantInfo[ ] = {
	    "Managed holistic contingency will grow killer action-items.",
	    "Vision-oriented zero administration time-frame will generate back-end interfaces.",
	    "Triple-buffered scalable services will productize visionary infomediaries.",
	    "Reactive radical knowledge base will aggregate extensible vortals.",
	    "Face to face client-server pricing structure will whiteboard robust communities.",
	    "Future-proofed 5th generation protocols will strategize web-enabled networks."
	};
        try {
	    for (String info : importantInfo) dropbox.put(info);         /** line 6 **/
        } catch (InterruptedException e) {}
    }
}

class Consumer implements Runnable {
    private BlockingQueue<String> dropbox;
    private int maxTakes;

    Consumer(BlockingQueue<String> dropbox, int maxTakes) {
        this.dropbox = dropbox;
	this.maxTakes = maxTakes;
    }

    @Override
    public void run() {
        Random random = new Random();
        try {
            for (int i = 0; i < this.maxTakes; i++) {                    /** line 7 **/
		String message = dropbox.take();                         /** line 8 **/
                System.out.format("Message received: %s\n", message);
                Thread.sleep(random.nextInt(3000));                      /** line 9 **/
            }
        } catch (InterruptedException e) {}
    }
}

