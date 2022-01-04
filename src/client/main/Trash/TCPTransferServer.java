package client.main.Trash;

import java.io.*;
import java.net.Socket;

public class TCPTransferServer {
  public static void main(String[] args) throws IOException {
    Socket socket = new Socket("127.0.0.1", 1555);
    File fileToSend = new File(
      "/home/htg/Documents/Project/youtok/src/client/main/Trash/video.mp4"
    );
    byte[] bytes = new byte[8192];
    FileInputStream fileInputStream = new FileInputStream(fileToSend);
    BufferedInputStream bufferedInputStream = new BufferedInputStream(fileInputStream);

    OutputStream outputStream = socket.getOutputStream();

    int count, file_size;

    while ((count = bufferedInputStream.read(bytes)) > 0) {
      System.out.println(count);
      outputStream.write(bytes, 0, count);
    }

    outputStream.flush();
    outputStream.close();
    fileInputStream.close();
    bufferedInputStream.close();
  }
}
