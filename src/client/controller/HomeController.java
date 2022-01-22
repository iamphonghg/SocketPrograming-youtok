package client.controller;

import client.main.Main;
import client.model.UserSession;
import client.model.Video;
import client.model.VideoButton;
import javafx.event.EventHandler;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.geometry.Insets;
import javafx.scene.Node;
import javafx.scene.control.Button;
import javafx.scene.control.ScrollPane;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.ColumnConstraints;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.Priority;
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
  private ScrollPane videoListScrollPane;
  private static GridPane videoListGridPane;

  @Override
  public void initialize(URL url, ResourceBundle resourceBundle) {
    videoListGridPane = new GridPane();
    videoListScrollPane.setContent(videoListGridPane);
    videoListGridPane.setPadding(new Insets(20));
    videoListGridPane.setHgap(20);
    videoListGridPane.setVgap(20);
    ColumnConstraints col1 = new ColumnConstraints();
    col1.setPercentWidth(25);
    videoListGridPane.getColumnConstraints().setAll(
      col1
    );

    try {
      updateVideoList();
    } catch (IOException e) {
      e.printStackTrace();
    }
  }

  public static void updateVideoList() throws IOException {
    List<Video> videos = fetchAllVideos();
//    for (Video video : videos) {
//      Button button = new Button(video.getTitle());
//      button.setOnMouseClicked(new EventHandler<MouseEvent>() {
//        @Override
//        public void handle(MouseEvent mouseEvent) {
////          Main.mainController.watchVideoBorderPane.toFront();
//          ProcessBuilder processBuilder = new ProcessBuilder();
//          processBuilder.command("bash", "-c", "ffplay rtsp://127.0.0.1:8554/test");
//          try {
//            Process process = processBuilder.start();
//          } catch (IOException e) {
//            e.printStackTrace();
//          }
//        }
//      });
//      videoListVBox.getChildren().add(button);
//    }


    List<VideoButton> buttonList = new ArrayList<>();
    for (Video video : videos) {
      VideoButton videoButton = new VideoButton(
        video.getTitle(),
        video.getAuthorName(),
        video.getFilename()
      );
      System.out.println(video.getFilename());
      buttonList.add(videoButton);
    }
    int COLS = 4;
    int videoNums = videos.size();
    int count = 0;
    for (int i = 0; i < videoNums / COLS + 1; i++) {
      for (int j = 0; j < COLS; j++) {
        if (count == videoNums)
          break;
        VideoButton tempButton = buttonList.get(count);
        videoListGridPane.add(tempButton, j, i);
        GridPane.setHgrow(tempButton, Priority.ALWAYS);

        count++;
      }
    }

  }

  public static List<Video> fetchAllVideos() throws IOException {
    Socket socket = new Socket(Main.SERVER_IP, 1472);
    DataOutputStream dataOutputStream = new DataOutputStream(
      socket.getOutputStream()
    );

    byte[] bufferSend = null;
    byte[] bufferReceive = new byte[4096];

    JSONObject request = new JSONObject();
    if (UserSession.getUserSession().getUser() == null) {
      request.put("head", "fetch_all_videos_no_login");
    } else {
      request.put("head", "fetch_all_videos_has_login");
    }
    JSONObject requestBody = new JSONObject();

    if (UserSession.getUserSession().getUser() == null) {
      requestBody.put("body", null);
    } else {
      JSONObject userId = new JSONObject();
      userId.put(
        "user_id",
        UserSession.getUserSession().getUser().getId()
      );
      request.put("body", userId);
    }
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
      Video newVideo = new Video(id, title, description, privacy, filename, authorName);
      videos.add(newVideo);
    }

    return videos;
  }
}
