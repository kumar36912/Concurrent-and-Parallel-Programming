/** Execute after the NonBlockingServer has been started:
    
    java NonBlockingClient
*/

import java.io.BufferedReader;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.CharBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.SocketChannel;
import java.nio.charset.Charset;
import java.nio.charset.CharsetDecoder;
import java.util.Iterator;
import java.util.Set;
import java.util.Scanner;

public class NonBlockingClient {
    public static void main(String[ ] args) {
	new NonBlockingClient().runClient();
    }
    private void runClient() {
	try {
	    // Prepare to connect to the server with a non-blocking read/write socket.
	    InetSocketAddress server =
		new InetSocketAddress(InetAddress.getByName("localhost"), 9876);          /** line  1 **/
	    SocketChannel rwChannel = SocketChannel.open();
	    rwChannel.configureBlocking(false);                                           /** line  2 **/
	    rwChannel.connect(server);                                                    /** line  3 **/
	    
	    Selector selector = Selector.open();                                          /** line  4 **/
	    rwChannel.register(selector,                                                  /** line  5 **/
			     SelectionKey.OP_CONNECT | 
			     SelectionKey.OP_READ    | 
			     SelectionKey.OP_WRITE);
	    while (true) {
		if (selector.select() > 0)
		    processKeys(selector.selectedKeys());
	    }
	}
	catch(Exception e) { }
    }
    
    private void processKeys(Set keys) {
	Iterator it = keys.iterator();                                                    /** line  6 **/
	try {
	    while (it.hasNext()) {
		SelectionKey key = (SelectionKey) it.next();
		it.remove();

		// Complete the connection.
		if (key.isConnectable()) {                                                /** line  7 **/
		    SocketChannel rwChannel = (SocketChannel) key.channel();              /** line  8 **/
		    while (rwChannel.isConnectionPending()) {                             /** line  9 **/
			rwChannel.finishConnect();                                        /** line 10 **/
		    }
		}

		if (key.isReadable()) {                                                   /** line 11 **/
		    String response = getResponse(key);
		    System.out.println(response);
		}

		if (key.isWritable()) {                                                   /** line 12 **/
		    sendRequest(key);
		}
	    }
	}
	catch(Exception e) { }
    }

    private String getResponse(SelectionKey key) {
	Scanner scanner = new Scanner(System.in);
	try {
	    SocketChannel rwChannel = (SocketChannel) key.channel();
	    ByteBuffer buffer = ByteBuffer.allocate(1024);
	    int count = 0;
	    String response = "";
	    while ((count = rwChannel.read(buffer)) > 0) {                                /** line 13 **/
		buffer.flip();
		response += Charset.defaultCharset().decode(buffer);
	    }
	    return response;
	}
	catch(Exception e) { }
	return "No response from server";
    }

    private void sendRequest(SelectionKey key) {                                          /** line 14 **/
	Scanner scanner = new Scanner(System.in);
	try {
	    SocketChannel rwChannel = (SocketChannel) key.channel();                      /** line 15 **/

	    System.out.print("Request to send ('quit' to quit): ");
	    String request = scanner.nextLine();
	    if (request.equalsIgnoreCase("quit")) System.exit(0);                         /** line 16 **/

	    ByteBuffer buffer = ByteBuffer.wrap(request.getBytes());
	    while (buffer.hasRemaining())                                                 /** line 17 **/
		rwChannel.write(buffer);                                                  /** line 18 **/
	}
	catch(Exception e) { }
    }
}

