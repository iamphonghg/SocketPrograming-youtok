package client.controller;

import client.main.Main;
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

public class SearchController implements Initializable {

  @FXML
  private ScrollPane resultVideoListScrollPane;

  private static VBox resultVideoListVBox;
  @Override
  public void initialize(URL url, ResourceBundle resourceBundle) {
    resultVideoListVBox = new VBox();
    resultVideoListScrollPane.setContent(resultVideoListVBox);

  }

  public static void updateSearchResultList(String searchKey) throws IOException {
    resultVideoListVBox.getChildren().clear();
    List<Video> videos = requestSearchVideo(searchKey);
    if (videos.size() > 0) {
      for (Video video : videos) {
        VideoButton videoButton = new VideoButton(
          video.getTitle(),
          video.getAuthorName(),
          video.getFilename()
        );
        System.out.println(video.getFilename());
        resultVideoListVBox.getChildren().add(videoButton);
      }
    }

  }

  public static List<Video> requestSearchVideo(String searchKey) throws IOException {
    Socket socket = new Socket(Main.SERVER_IP, 1472);
    DataOutputStream dataOutputStream = new DataOutputStream(
      socket.getOutputStream()
    );

    byte[] bufferSend = null;
    byte[] bufferReceive = new byte[4096];

    JSONObject request = new JSONObject();

    if (UserSession.getUserSession().getUser() == null) {
      request.put("head", "search_video_no_login");
    } else {
      request.put("head", "search_video_has_login");
    }

    JSONObject requestBody = new JSONObject();
    requestBody.put("search_key", searchKey);

    if (UserSession.getUserSession().getUser() == null) {
      requestBody.put("body", null);
    } else {
      requestBody.put(
        "user_id",
        UserSession.getUserSession().getUser().getId()
      );
    }
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
    for (Object video : videosListArray) {
      String id = ((JSONObject) video).get("id").toString();
      String title = ((JSONObject) video).get("title").toString();
      String description =
        ((JSONObject) video)
          .get("description")
          .toString();
      String privacy = ((JSONObject) video).get("privacy").toString();
      String filename = ((JSONObject) video).get("filename").toString();
      String authorName = ((JSONObject) video).get("author_name").toString();
      Video newVideo = new Video(
        id,
        title,
        description,
        privacy,
        filename,
        authorName
      );
      videos.add(newVideo);
    }

    return videos;
  }
}
