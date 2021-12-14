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

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.InetAddress;
import java.net.URL;
import java.net.UnknownHostException;
import java.util.ResourceBundle;


public class MainController implements Initializable {
    @FXML
    private ImageView imageViewLogo;

    @FXML
    private Button buttonAvatar;

    @FXML
    private Button buttonLogin;

    @FXML
    private Button buttonSearch;

    @FXML
    private MenuButton menuButtonUser;

    @FXML
    private Button buttonUpload;

    @FXML
    private TextField inputSearch;

    @FXML
    private Label labelTest;

    @FXML
    private BorderPane borderPaneMain;

    @FXML
    private StackPane stackPaneMain;

    @FXML
    private BorderPane borderPaneHome;
    @FXML
    private BorderPane borderPaneLogin;

    @Override
    public void initialize(URL url, ResourceBundle resourceBundle) {
        // when the button is hidden, it won't take up the container anymore
        buttonUpload.managedProperty().bind(buttonUpload.visibleProperty());
        buttonLogin.managedProperty().bind(buttonLogin.visibleProperty());
        menuButtonUser.managedProperty().bind(menuButtonUser.visibleProperty());


        try {
            FXMLLoader fxmlLoaderTest = new FXMLLoader();
            fxmlLoaderTest.setLocation(getClass().getResource("../fxml/home.fxml"));
            borderPaneHome = fxmlLoaderTest.load();

            FXMLLoader fxmlLoaderLogin = new FXMLLoader();
            fxmlLoaderLogin.setLocation(getClass().getResource("../fxml/log-in.fxml"));
            borderPaneLogin = fxmlLoaderLogin.load();

            stackPaneMain.getChildren().add(borderPaneLogin);
            stackPaneMain.getChildren().add(borderPaneHome);

        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void handleMouseClicks(MouseEvent event) throws IOException {
        if (event.getSource() == buttonLogin) {
            borderPaneLogin.toFront();
        } else if (event.getSource() == imageViewLogo) {
            borderPaneHome.toFront();
        } else if (event.getSource() == buttonSearch) {
            requestSearch();
        }
    }

    private void requestSearch() throws IOException {
        InetAddress ip = InetAddress.getLocalHost();
        String requestMessage = "SEARCHVIDEO|...";
        byte[] requestBuffer = requestMessage.getBytes();
        byte[] responseBuffer = new byte[1024];

        DatagramPacket datagramPacketSend = new DatagramPacket(requestBuffer, requestBuffer.length, ip, 9999);
        UDPClient.socket.send(datagramPacketSend);

        DatagramPacket datagramPacketReceive = new DatagramPacket(responseBuffer, responseBuffer.length);
        UDPClient.socket.receive(datagramPacketReceive);
        String responseMessage = new String(responseBuffer, 0, datagramPacketReceive.getLength());
        System.out.println(responseMessage);
    }
}
