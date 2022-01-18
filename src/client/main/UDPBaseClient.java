package client.main;

import org.json.simple.JSONObject;
import org.json.simple.JSONValue;

import java.io.IOException;
import java.net.*;
import java.util.Scanner;

public class UDPBaseClient {
    public UDPBaseClient() throws SocketException {
    }

    public static void main(String[] args) throws IOException {
        Scanner scanner = new Scanner(System.in);
        DatagramSocket socket = new DatagramSocket();
        InetAddress ip = InetAddress.getLocalHost();
        byte[] bufferSend = null;
        byte[] bufferReceive = new byte[1024];
        while (true) {
            JSONObject request = new JSONObject();
            request.put("head", "login");
            JSONObject requestBody = new JSONObject();
            requestBody.put("username", "admin");
            requestBody.put("password", "123123");
            request.put("body", requestBody);
            bufferSend = request.toJSONString().getBytes();

            DatagramPacket datagramPacketSend = new DatagramPacket(bufferSend, bufferSend.length, ip, 9999);
            socket.send(datagramPacketSend);

            DatagramPacket datagramPacketReceive = new DatagramPacket(bufferReceive, bufferReceive.length);
            socket.receive(datagramPacketReceive);
            String response = new String(bufferReceive, 0, datagramPacketReceive.getLength());
            JSONObject jsonObject = (JSONObject) JSONValue.parse(response);
            String responseBody = jsonObject.get("body").toString();

            JSONObject jsonObjectBody = (JSONObject) JSONValue.parse(responseBody);
            String result = jsonObjectBody.get("status").toString();

            String userInfo = jsonObjectBody.get("user").toString();
            JSONObject jsonObjectUser = (JSONObject) JSONValue.parse(userInfo);
            String fullName = jsonObjectUser.get("full_name").toString();
            String id = (String) jsonObjectUser.get("id");
            System.out.println(fullName + id);
        }
    }


}
