package client.main;

import java.io.IOException;
import java.net.*;
import java.nio.charset.StandardCharsets;
import java.util.Arrays;
import java.util.Scanner;

public class UDPBaseClient {
    public static DatagramSocket socket;

    public UDPBaseClient() throws SocketException {
        socket = new DatagramSocket();

    }

    public static void main(String[] args) throws IOException {
        Scanner scanner = new Scanner(System.in);

        InetAddress ip = InetAddress.getLocalHost();
        byte[] bufferSend = null;
        byte[] bufferReceive = new byte[1024];
        while (true) {
            String input = scanner.nextLine();
            bufferSend = input.getBytes();

            DatagramPacket datagramPacketSend = new DatagramPacket(bufferSend, bufferSend.length, ip, 9999);
            socket.send(datagramPacketSend);
            if (input.equals("bye"))
                break;

            DatagramPacket datagramPacketReceive = new DatagramPacket(bufferReceive, bufferReceive.length);
            socket.receive(datagramPacketReceive);
            String response = new String(bufferReceive, 0, datagramPacketReceive.getLength());
            System.out.println(response);
            System.out.println(input);

        }
    }
}
