package client.main.Trash;

import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;

public class TCPServer {
    public static void main(String[] args) throws IOException {
        Socket socket = new Socket("127.0.0.1", 1472);
        DataOutputStream dataOutputStream = new DataOutputStream(socket.getOutputStream());
        dataOutputStream.write("Hoho".getBytes());
        dataOutputStream.flush();
        dataOutputStream.close();
        socket.close();

    }
}
