package client.controller;

import client.main.Main;
import client.model.MyVideo;
import client.model.UserSession;
import client.model.Video;
import client.model.VideoButton;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.ScrollPane;
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

public class MyVideoController implements Initializable {

  @FXML
  private ScrollPane myVideoScrollPane;

  private static VBox myVideoVBox;

  @Override
  public void initialize(URL url, ResourceBundle resourceBundle) {
    myVideoVBox = new VBox();
    myVideoScrollPane.setContent(myVideoVBox);
  }

  public static void updateMyVideoList() throws IOException {
    myVideoVBox.getChildren().clear();
    List<Video> videos = requestGetMyVideo();
    if (videos.size() > 0) {
      for (Video video : videos) {
        MyVideo videoButton = new MyVideo(
          video.getId(),
          video.getTitle(),
          video.getAuthorName(),
          video.getFilename(),
          video.getDescription(),
          video.getPrivacy()
        );
        System.out.println(video.getFilename());
        myVideoVBox.getChildren().add(videoButton);
      }
    }
  }

  public static List<Video> requestGetMyVideo() throws IOException {
    Socket socket = new Socket(Main.SERVER_IP, 1472);
    DataOutputStream dataOutputStream = new DataOutputStream(
      socket.getOutputStream()
    );

    byte[] bufferSend = null;
    byte[] bufferReceive = new byte[4096];

    JSONObject request = new JSONObject();

    request.put("head", "my_video");
    JSONObject requestBody = new JSONObject();
    requestBody.put("user_id", UserSession.getUserSession().getUser().getId());

    request.put("body", requestBody);

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

    String currentUserFullname = UserSession.getUserSession().getUser().getFullName();

    for (Object video : videosListArray) {
      String id = ((JSONObject) video).get("id").toString();
      String title = ((JSONObject) video).get("title").toString();
      String description =
        ((JSONObject) video)
          .get("description")
          .toString();
      String privacy = ((JSONObject) video).get("privacy").toString();
      String filename = ((JSONObject) video).get("filename").toString();
      Video newVideo = new Video(
        id,
        title,
        description,
        privacy,
        filename,
        currentUserFullname
      );
      videos.add(newVideo);
    }

    return videos;
  }
}
