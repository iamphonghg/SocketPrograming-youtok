package client.main;

import java.io.IOException;
import java.net.*;
import java.nio.charset.StandardCharsets;
import java.util.Scanner;

public class UDPBaseClient {
    public static void main(String[] args) throws IOException {
        Scanner scanner = new Scanner(System.in);
        DatagramSocket datagramSocket = new DatagramSocket();

        InetAddress ip = InetAddress.getLocalHost();
        byte buff[] = null;

        while (true) {
            String inp = scanner.nextLine();

            buff = inp.getBytes();

            DatagramPacket DpSend = new DatagramPacket(buff, buff.length, ip, 9999);
            datagramSocket.send(DpSend);

            if (inp.equals("bye"))
                break;
        }
    }
}
