package client.controller;

import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.Button;
import javafx.scene.image.ImageView;
import javafx.scene.input.MouseEvent;
import javafx.scene.media.MediaView;
import javafx.util.Duration;

import java.net.URL;
import java.util.ResourceBundle;

public class WatchVideoController implements Initializable {
//  @FXML
//  private MediaView mediaView;
  @FXML
  private ImageView videoImageView;
  @FXML
  private Button pauseButton;
  @FXML
  private Button playButton;
  @FXML
  private Button seekBackButton;
  @FXML
  private Button seekForwardButton;



  @Override
  public void initialize(URL url, ResourceBundle resourceBundle) {



//    Media media = null;
//    try {
//      media = new Media(new File("/home/htg/Documents/stream/index.m3u8").toURI().toURL().toString());
//    } catch (MalformedURLException e) {
//      e.printStackTrace();
//    }
////    Media media = new Media(
////      "/home/htg/Documents/test/mp4"
////    );
//    mediaPlayer = new MediaPlayer(media);
//    mediaPlayer.setAutoPlay(false);
//    mediaView.setMediaPlayer(mediaPlayer);
  }

  public void handleMouseClicks(MouseEvent event) {
    if (event.getSource() == playButton) {
//      mediaPlayer.play();
    } else if (event.getSource() == pauseButton) {
//      mediaPlayer.pause();
    } else if (event.getSource() == seekBackButton) {
//      mediaPlayer.seek(Duration.millis(mediaPlayer.getCurrentTime().toMillis() - 1000));
    } else if (event.getSource() == seekForwardButton) {
//      mediaPlayer.seek(Duration.millis(mediaPlayer.getCurrentTime().toMillis() + 1000));
    }
  }


}
