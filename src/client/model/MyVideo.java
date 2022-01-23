package client.model;

import client.main.Main;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.geometry.Pos;
import javafx.scene.Node;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;
import org.json.simple.JSONObject;

import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;

public class MyVideo extends HBox {
  private VBox detailContainerVBox;
  private VBox buttonContainerVBox;
  private String videoTitle;
  private String videoId;
  private String videoFileName;
  private String privacy;
  private String authorName;
  private String description;
  private Button buttonView;
  private Button buttonChangePrivacy;
  public MyVideo(
    String videoId,
    String videoTitle,
    String authorName,
    String videoFileName,
    String description,
    String privacy
  ) {
    this.videoId = videoId;
    this.videoFileName = videoFileName;
    this.videoTitle = videoTitle;
    this.authorName = authorName;
    this.description = description;
    this.privacy = privacy;
    initComponent();
  }

  private void initComponent() {
    this.setMaxWidth(Double.MAX_VALUE);
    this.setPrefHeight(300);
    this.setAlignment(Pos.CENTER_LEFT);
    Label thumbnailLabel = new Label("Thumbnail here");
    this.getChildren().add(thumbnailLabel);

    detailContainerVBox = new VBox();
    Label titleLabel = new Label(this.videoTitle);
    Label descriptionLabel = new Label(this.description);
    detailContainerVBox.getChildren().add(titleLabel);
    detailContainerVBox.getChildren().add(descriptionLabel);

    this.getChildren().add(detailContainerVBox);

    buttonContainerVBox = new VBox();
    buttonView = new Button("View video");
    buttonChangePrivacy = new Button();
    if (this.getPrivacy().equals("public")) {
      buttonChangePrivacy.setText("Make video private");
    } else {
      buttonChangePrivacy.setText("Make video public");
    }
    buttonContainerVBox.getChildren().add(buttonView);
    buttonContainerVBox.getChildren().add(buttonChangePrivacy);
    this.getChildren().add(buttonContainerVBox);

    buttonChangePrivacy.setOnAction(new EventHandler<ActionEvent>() {
      @Override
      public void handle(ActionEvent actionEvent) {
        String newPrivacy = "";
        try {
          requestChangePrivacy();
        } catch (IOException e) {
          e.printStackTrace();
        }
      }
    });
  }

  private void requestChangePrivacy() throws IOException {
    String newPrivacy = "";
    if (this.getPrivacy().equals("public")) {
      newPrivacy = "private";
    } else {
      newPrivacy = "public";
    }
    Socket socket = new Socket(Main.SERVER_IP, 1472);
    DataOutputStream dataOutputStream = new DataOutputStream(
      socket.getOutputStream()
    );

    byte[] bufferSend = null;
    byte[] bufferReceive = new byte[4096];

    JSONObject request = new JSONObject();

    request.put("head", "update_privacy");

    JSONObject requestBody = new JSONObject();
    requestBody.put("video_id", this.getVideoId());
    requestBody.put("privacy", newPrivacy);

    request.put("body", requestBody);

    bufferSend = request.toJSONString().getBytes();

    dataOutputStream.write(bufferSend);
    dataOutputStream.flush();
  }

  public VBox getDetailContainerVBox() {
    return detailContainerVBox;
  }

  public void setDetailContainerVBox(VBox detailContainerVBox) {
    this.detailContainerVBox = detailContainerVBox;
  }

  public VBox getButtonContainerVBox() {
    return buttonContainerVBox;
  }

  public void setButtonContainerVBox(VBox buttonContainerVBox) {
    this.buttonContainerVBox = buttonContainerVBox;
  }

  public String getVideoTitle() {
    return videoTitle;
  }

  public void setVideoTitle(String videoTitle) {
    this.videoTitle = videoTitle;
  }

  public String getVideoFileName() {
    return videoFileName;
  }

  public void setVideoFileName(String videoFileName) {
    this.videoFileName = videoFileName;
  }

  public String getPrivacy() {
    return privacy;
  }

  public void setPrivacy(String privacy) {
    this.privacy = privacy;
  }

  public String getAuthorName() {
    return authorName;
  }

  public void setAuthorName(String authorName) {
    this.authorName = authorName;
  }

  public String getDescription() {
    return description;
  }

  public void setDescription(String description) {
    this.description = description;
  }

  public Button getButtonView() {
    return buttonView;
  }

  public void setButtonView(Button buttonView) {
    this.buttonView = buttonView;
  }

  public Button getButtonChangePrivacy() {
    return buttonChangePrivacy;
  }

  public void setButtonChangePrivacy(Button buttonChangePrivacy) {
    this.buttonChangePrivacy = buttonChangePrivacy;
  }

  public String getVideoId() {
    return videoId;
  }

  public void setVideoId(String videoId) {
    this.videoId = videoId;
  }
}
