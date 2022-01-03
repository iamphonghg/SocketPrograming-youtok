package client.controller;

import client.main.UDPClient;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.fxml.Initializable;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.MenuButton;
import javafx.scene.control.TextField;
import javafx.scene.image.ImageView;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.StackPane;
import model.User;
import model.UserSession;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.InetAddress;
import java.net.URL;
import java.net.UnknownHostException;
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
  private BorderPane homeBorderPane;
  @FXML
  private BorderPane loginBorderPane;

  @Override
  public void initialize(URL url, ResourceBundle resourceBundle) {

    // when the button is hidden, it won't take up the container anymore
    uploadButton.managedProperty().bind(uploadButton.visibleProperty());
    loginButton.managedProperty().bind(loginButton.visibleProperty());
    userMenuButton.managedProperty().bind(userMenuButton.visibleProperty());

    try {
      FXMLLoader fxmlLoaderTest = new FXMLLoader();
      fxmlLoaderTest.setLocation(getClass().getResource("../fxml/home.fxml"));
      homeBorderPane = fxmlLoaderTest.load();

      FXMLLoader fxmlLoaderLogin = new FXMLLoader();
      fxmlLoaderLogin.setLocation(getClass().getResource("../fxml/log-in.fxml"));
      loginBorderPane = fxmlLoaderLogin.load();

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
      homeBorderPane.toFront();
    } else if (event.getSource() == searchButton) {
      requestSearch();
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
