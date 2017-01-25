import java.io.*;
import java.net.*;
import java.nio.*;
import java.nio.channels.*;

class tcpclient{
    public static void main(String args[]){
	try{
	    SocketChannel sc = Socketchannel.open();
	    sc.connect(new InetSocketAddress("127.0.0.1", 4456));
	    Console  cons = System.console();
	    string n = cons.readline("Enter your message. ");
	    Bytebuffer buf = ByteBuffer.wrap(m.getbuf);
	    sc.write(buf);
	    sc.close();
	}catch(IOException e){
	    System.out.println("got an IO Exception");
	}
    }
}
