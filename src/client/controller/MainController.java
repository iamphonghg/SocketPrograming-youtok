package client.controller;

import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.fxml.Initializable;
import javafx.scene.control.Button;
import javafx.scene.control.MenuButton;
import javafx.scene.control.TextField;
import javafx.scene.image.ImageView;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.StackPane;

import java.io.IOException;
import java.net.URL;
import java.util.ResourceBundle;


public class MainController implements Initializable {
  @FXML
  private ImageView logoImageView;
  @FXML
  public Button loginButton;
  @FXML
  private Button searchButton;
  @FXML
  private MenuButton userMenuButton;
  @FXML
  public Button uploadButton;
  @FXML
  private TextField searchInput;
  @FXML
  private BorderPane mainBorderPane;
  @FXML
  private StackPane mainStackPane;
  @FXML
  public BorderPane homeBorderPane;
  @FXML
  private BorderPane loginBorderPane;
  @FXML
  private BorderPane uploadBorderPane;
  @FXML
  public BorderPane watchVideoBorderPane;
  public BorderPane uploadProcessBorderPane;

  public static UploadProcessController uploadProcessController;
  public static HomeController homeController;
  public static WatchVideoController watchVideoController;

  @Override
  public void initialize(URL url, ResourceBundle resourceBundle) {

    // when the button is hidden, it won't take up the container anymore
    uploadButton.managedProperty().bind(uploadButton.visibleProperty());
    loginButton.managedProperty().bind(loginButton.visibleProperty());
    userMenuButton.managedProperty().bind(userMenuButton.visibleProperty());

    try {
      FXMLLoader watchVideoLoader = new FXMLLoader();
      watchVideoLoader.setLocation(getClass().getResource(
        "../fxml/watch-video.fxml"
      ));
      watchVideoBorderPane = watchVideoLoader.load();
      watchVideoController = watchVideoLoader.getController();

      FXMLLoader uploadLoader = new FXMLLoader();
      uploadLoader.setLocation(getClass().getResource(
        "../fxml/upload-video.fxml"
      ));
      uploadBorderPane = uploadLoader.load();

      FXMLLoader uploadProcessLoader = new FXMLLoader();
      uploadProcessLoader.setLocation(getClass().getResource(
        "../fxml/upload-process.fxml"
      ));
      uploadProcessBorderPane = uploadProcessLoader.load();
      uploadProcessController = uploadProcessLoader.getController();

      FXMLLoader homeLoader = new FXMLLoader();
      homeLoader.setLocation(getClass().getResource(
        "../fxml/home.fxml"
      ));
      homeBorderPane = homeLoader.load();
      homeController = homeLoader.getController();

      FXMLLoader loginLoader = new FXMLLoader();
      loginLoader.setLocation(getClass().getResource(
        "../fxml/log-in.fxml"
      ));
      loginBorderPane = loginLoader.load();


      mainStackPane.getChildren().add(watchVideoBorderPane);
      mainStackPane.getChildren().add(uploadBorderPane);
      mainStackPane.getChildren().add(uploadProcessBorderPane);
      mainStackPane.getChildren().add(loginBorderPane);
      mainStackPane.getChildren().add(homeBorderPane);

      uploadButton.setVisible(false);
      userMenuButton.setVisible(false);

    } catch (IOException e) {
      e.printStackTrace();
    }
  }

  public void handleMouseClicks(MouseEvent event) throws IOException {
    if (event.getSource() == loginButton) {
      loginBorderPane.toFront();
    } else if (event.getSource() == logoImageView) {
      HomeController.updateVideoList();
      homeBorderPane.toFront();
    } else if (event.getSource() == searchButton) {
      requestSearch();
    } else if (event.getSource() == uploadButton) {
      System.out.println("true");
      uploadBorderPane.toFront();
    }
  }

  public void handleAfterLogin() {
    loginButton.setVisible(false);
    uploadButton.setVisible(true);
    userMenuButton.setVisible(true);
  }

  private void requestSearch() throws IOException {
//        InetAddress ip = InetAddress.getLocalHost();
//        String requestMessage = "SEARCHVIDEO|...";
//        byte[] requestBuffer = requestMessage.getBytes();
//        byte[] responseBuffer = new byte[1024];
//
//        DatagramPacket datagramPacketSend = new DatagramPacket(requestBuffer, requestBuffer.length, ip, 9999);
//        UDPClient.socket.send(datagramPacketSend);
//
//        DatagramPacket datagramPacketReceive = new DatagramPacket(responseBuffer, responseBuffer.length);
//        UDPClient.socket.receive(datagramPacketReceive);
//        String responseMessage = new String(responseBuffer, 0, datagramPacketReceive.getLength());
//        System.out.println(responseMessage);
  }
}
