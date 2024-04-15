/** To compile and run the application:

    javac NonBlockingServer.java
    javac NonBlockingClient.java
    java NonBlockingServer
    java NonBlockingClient ## using a separate command-line prompt

    Server can be terminated with Control-C.
*/

import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.util.Iterator;
import java.util.Set;

public class NonBlockingServer {
    public static void main(String[] args) {
	new NonBlockingServer().runServer();

    }
    private void runServer() {
	try {
	    final int port = 9876;                                                               /** line  1 **/
	    ServerSocketChannel serverSocket = ServerSocketChannel.open();                       /** line  2 **/
	    serverSocket.configureBlocking(false);                                               /** line  3 **/
	    serverSocket.socket().bind(new InetSocketAddress(InetAddress.getByName("localhost"),
							     port));
	    System.out.println("Server accepting connections on port " + port);

	    // Await connections.
	    Selector selector = Selector.open();                                                 /** line  4 **/
	    serverSocket.register(selector, SelectionKey.OP_ACCEPT);                             /** line  5 **/
	    while (true) {                                                                       /** line  6 **/
		if (selector.select() <= 0) continue;                                            /** line  7 **/
		acceptConnections(selector.selectedKeys());                                      /** line  8 **/
	    }
	}
	catch(Exception e) { }
    }

    private void acceptConnections(Set keys) {
	try {
	    Iterator it = keys.iterator();
	    while (it.hasNext()) {
		SelectionKey key = (SelectionKey) it.next();                                     /** line  9 **/
		it.remove();                                                                     /** line 10 **/
		
		if (key.isAcceptable()) {                                                        /** line 11 **/
		    ServerSocketChannel serverSocket = (ServerSocketChannel) key.channel();      /** line 12 **/
		    SocketChannel rwChannel = (SocketChannel) serverSocket.accept();             /** line 13 **/
		    rwChannel.configureBlocking(false);                                          /** line 14 **/
		    rwChannel.register(key.selector(), SelectionKey.OP_READ);                    /** line 15 **/
		}
		
		if (key.isReadable()) {                                                          /** line 16 **/
		    String request = handleRead(key);
		    if (request.length() > 0) {
			SocketChannel rwChannel = (SocketChannel) key.channel();                 /** line 17 **/
			request = "Echoing: " + request;
			ByteBuffer buffer = ByteBuffer.wrap(request.getBytes());
			rwChannel.write(buffer);                                                 /** line 18 **/
			System.out.println(new String(buffer.array()));
		    }
		}
	    }
	}
	catch(Exception e) { }
    }

    private String handleRead(SelectionKey key) {
	try {
	    SocketChannel rwChannel = (SocketChannel) key.channel();                             /** line 20 **/
	    ByteBuffer buffer = ByteBuffer.allocate(1024);
	    int bytesCount = rwChannel.read(buffer);                                             /** line 21 **/
	    if (bytesCount > 0) {                                                                /** line 22 **/
		buffer.flip();
		return new String(buffer.array());
	    }
	}
	catch(Exception e) { }
	return "No request";
    }
}
