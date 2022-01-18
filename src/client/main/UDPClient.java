package client.main;

import java.net.DatagramSocket;
import java.net.SocketException;

public class UDPClient {
    public static DatagramSocket socket;

    static {
        try {
            socket = new DatagramSocket();
        } catch (SocketException e) {
            e.printStackTrace();
        }
    }
}
