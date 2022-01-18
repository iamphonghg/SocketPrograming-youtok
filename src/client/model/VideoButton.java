package client.model;

import client.main.Main;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.geometry.Pos;
import javafx.scene.Cursor;
import javafx.scene.Node;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.Alert;
import javafx.scene.control.Button;
import javafx.scene.control.DialogPane;
import javafx.scene.control.Label;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;
import javafx.scene.text.Font;
import javafx.stage.Modality;
import javafx.stage.Stage;

import java.io.IOException;

public class VideoButton extends Button {
  private String videoTitle;
  private VBox buttonLayoutVBox;
  public VideoButton(String videoTitle, String authorName) {
    this.setMinHeight(280);
    this.setMaxWidth(Double.MAX_VALUE);
    this.setCursor(Cursor.HAND);
    buttonLayoutVBox = new VBox();
    buttonLayoutVBox.setMaxWidth(Double.MAX_VALUE);
    buttonLayoutVBox.setMaxHeight(Double.MAX_VALUE);
    buttonLayoutVBox.setSpacing(5);

    Label thumbnailLabel = new Label("Thumbnail here");
    thumbnailLabel.setStyle("-fx-font-size: 30px; -fx-font-weight: bold; -fx-border-color: #fe2c55; -fx-font-family: \"Sofia Pro\"");
    thumbnailLabel.setAlignment(Pos.BASELINE_CENTER);
    thumbnailLabel.setMaxWidth(Double.MAX_VALUE);
    thumbnailLabel.setMinHeight(200);

    Label titleLabel = new Label(videoTitle);
    titleLabel.setStyle("-fx-font-size: 20px; -fx-font-family: \"Sofia Pro\"");

    Label authorLabel = new Label(authorName);
    authorLabel.setStyle("-fx-font-size: 18px; -fx-font-family: \"Sofia Pro\"");

    buttonLayoutVBox.getChildren().add(thumbnailLabel);
    buttonLayoutVBox.getChildren().add(titleLabel);
    buttonLayoutVBox.getChildren().add(authorLabel);
    this.setGraphic(buttonLayoutVBox);

    this.setOnAction(new EventHandler<ActionEvent>() {
      @Override
      public void handle(ActionEvent actionEvent) {
        DialogPane dialog = new DialogPane();

        VBox dialogVBox = new VBox();
        dialogVBox.setSpacing(20);
        Label confirmLabel = new Label("Video is ready, start to watch?");
        confirmLabel.setStyle("-fx-font-size: 20px; -fx-font-family: \"Sofia Pro\"");
        HBox buttonGroupHBox = new HBox();
        Button watchButton = new Button("Watch");
        Button cancelButton = new Button("Cancel");
        watchButton.setStyle("-fx-font-size: 20px; -fx-font-family: \"Sofia Pro\"");
        cancelButton.setStyle("-fx-font-size: 20px; -fx-font-family: \"Sofia Pro\"");
        buttonGroupHBox.getChildren().addAll(watchButton, cancelButton);
        buttonGroupHBox.setAlignment(Pos.BASELINE_RIGHT);
        buttonGroupHBox.setSpacing(5);

        dialogVBox.getChildren().add(confirmLabel);
        dialogVBox.getChildren().add(buttonGroupHBox);

        dialog.setContent(dialogVBox);

        VBox root = new VBox();
        root.getChildren().add(dialog);
        Stage stage = new Stage();
        stage.initModality(Modality.APPLICATION_MODAL);
        stage.initOwner(Main.mainStage.getScene().getWindow());
        stage.setScene(new Scene(root));
        stage.show();

        watchButton.setOnAction(new EventHandler<ActionEvent>() {
          @Override
          public void handle(ActionEvent actionEvent) {
            ProcessBuilder processBuilder = new ProcessBuilder();
            processBuilder.command("bash", "-c", "ffplay rtsp://127.0.0.1:8554/test");
            try {
              Process process = processBuilder.start();
            } catch (IOException e) {
              e.printStackTrace();
            }
          }
        });

        cancelButton.setOnAction(new EventHandler<ActionEvent>() {
          @Override
          public void handle(ActionEvent actionEvent) {
            stage.close();
          }
        });
      }
    });
  }

  public String getVideoTitle() {
    return videoTitle;
  }
  public void setVideoTitle(String videoTitle) {
    this.videoTitle = videoTitle;
  }
}
