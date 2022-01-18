package client.controller;

import client.main.Main;
import client.model.Video;
import javafx.event.EventHandler;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.Button;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.VBox;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.JSONValue;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;
import java.net.URL;
import java.util.ArrayList;
import java.util.List;
import java.util.ResourceBundle;

public class HomeController implements Initializable {
  @FXML
  private VBox videoListVBox;

  @Override
  public void initialize(URL url, ResourceBundle resourceBundle) {
    try {
      updateVideoList();
    } catch (IOException e) {
      e.printStackTrace();
    }
  }

  public void updateVideoList() throws IOException {
    List<Video> videos = fetchAllVideos();
    for (Video video : videos) {
      Button button = new Button(video.getTitle());
      button.setOnMouseClicked(new EventHandler<MouseEvent>() {
        @Override
        public void handle(MouseEvent mouseEvent) {
//          Main.mainController.watchVideoBorderPane.toFront();
          ProcessBuilder processBuilder = new ProcessBuilder();
          processBuilder.command("bash", "-c", "ffplay rtsp://127.0.0.1:8554/test");
          try {
            Process process = processBuilder.start();
          } catch (IOException e) {
            e.printStackTrace();
          }
        }
      });
      videoListVBox.getChildren().add(button);
    }
  }

  public List<Video> fetchAllVideos() throws IOException {
    Socket socket = new Socket("127.0.0.1", 1472);
    DataOutputStream dataOutputStream = new DataOutputStream(
      socket.getOutputStream()
    );

    byte[] bufferSend = null;
    byte[] bufferReceive = new byte[4096];

    JSONObject request = new JSONObject();
    request.put("head", "fetch_all_videos");
    JSONObject requestBody = new JSONObject();
    request.put("body", null);
    bufferSend = request.toJSONString().getBytes();

    dataOutputStream.write(bufferSend);
    dataOutputStream.flush();

    DataInputStream dataInputStream = new DataInputStream(
      socket.getInputStream()
    );

    String response = new String(
      bufferReceive,
      0,
      dataInputStream.read(bufferReceive)
    );

    List<Video> videos = new ArrayList<>();

    JSONObject jsonObject = (JSONObject) JSONValue.parse(response);
    String responseBody = jsonObject.get("body").toString();
    JSONObject videosListJsonObject = (JSONObject) JSONValue.parse(
      responseBody
    );
    JSONArray videosListArray = (JSONArray) videosListJsonObject.get(
      "videos_list"
    );
    for (Object video : videosListArray) {
      String id = ((JSONObject) video).get("id").toString();
      String title = ((JSONObject) video).get("title").toString();
      String description =
        ((JSONObject) video)
          .get("description")
          .toString();
      String privacy = ((JSONObject) video).get("privacy").toString();
      String filename = ((JSONObject) video).get("filename").toString();

      Video newVideo = new Video(id, title, description, privacy, filename);
      videos.add(newVideo);
    }

    return videos;
  }
}
