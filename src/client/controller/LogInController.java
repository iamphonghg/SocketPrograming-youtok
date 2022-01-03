package client.controller;

import client.main.Main;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.fxml.Initializable;
import javafx.scene.Node;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.PasswordField;
import javafx.scene.control.TextField;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.Pane;
import javafx.stage.Stage;
import model.User;
import model.UserSession;
import org.json.simple.JSONObject;
import org.json.simple.JSONValue;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.*;
import java.nio.charset.StandardCharsets;
import java.util.Objects;
import java.util.ResourceBundle;

public class LogInController implements Initializable {

  @FXML
  private Button logInButton;
  @FXML
  private Button createAccountButton;
  @FXML
  private PasswordField passwordInput;
  @FXML
  private TextField usernameInput;
  @FXML
  private Label resultLabel;
  @FXML
  private Label passwordLabel;
  @FXML
  private Label usernameLabel;
  @FXML
  private Label titleLabel;

  private Node[] allNodes;

  @Override
  public void initialize(URL url, ResourceBundle resourceBundle) {
    allNodes = new Node[] {
      resultLabel,
      titleLabel,
      usernameLabel,
      usernameInput,
      passwordLabel,
      passwordInput,
      createAccountButton,
      logInButton
    };
    resultLabel.setVisible(false);
    initialAllNodes(allNodes);
  }

  public void initialAllNodes(Node ...nodes) {
    for (Node node : nodes) {
      node.managedProperty().bind(node.visibleProperty());
    }
  }



  public void changeSceneAfterLogin() {
    setNodeVisible(false, allNodes);
    resultLabel.setText("Logged in successfully!");
    resultLabel.setVisible(true);
  }

  public void setNodeVisible(boolean trueOrFalse, Node ...nodes) {
    for (Node node : nodes) {
      node.setVisible(trueOrFalse);
    }
  }

  public void handleMouseClicks(MouseEvent event) throws IOException {
    if (event.getSource() == logInButton) {
      String username = usernameInput.getText();
      String password = passwordInput.getText();
      User responseUser = requestLogin(username, password);
      if (responseUser == null) {
        resultLabel.setText("Result: login failed");
      } else {
        resultLabel.setText(
          "Result: login successfully, user name: "
            + responseUser.getFullName() +
            ", user id: " + responseUser.getId()
        );
        UserSession.getUserSession().setUser(responseUser);
        Main.mainController.handleAfterLogin();
        changeSceneAfterLogin();
      }
    }
  }

//  private User requestLogin(String username, String password) throws IOException {
//    User user = null;
//
//    DatagramSocket socket = new DatagramSocket();
//    InetAddress ip = InetAddress.getLocalHost();
//    byte[] bufferSend = null;
//    byte[] bufferReceive = new byte[1024];
//
//    JSONObject request = new JSONObject();
//    request.put("head", "login");
//    JSONObject requestBody = new JSONObject();
//    requestBody.put("username", username);
//    requestBody.put("password", password);
//    request.put("body", requestBody);
//    bufferSend = request.toJSONString().getBytes();
//
//    DatagramPacket datagramPacketSend = new DatagramPacket(bufferSend, bufferSend.length, ip, 9999);
//    socket.send(datagramPacketSend);
//
//    DatagramPacket datagramPacketReceive = new DatagramPacket(bufferReceive, bufferReceive.length);
//    socket.receive(datagramPacketReceive);
//    String response = new String(bufferReceive, 0, datagramPacketReceive.getLength());
//    JSONObject jsonObject = (JSONObject) JSONValue.parse(response);
//    String responseBody = jsonObject.get("body").toString();
//
//    JSONObject jsonObjectBody = (JSONObject) JSONValue.parse(responseBody);
//    String result = jsonObjectBody.get("status").toString();
//    if (Objects.equals(result, "false")) {
//      return null;
//    } else {
//      String userInfo = jsonObjectBody.get("user").toString();
//      JSONObject jsonObjectUser = (JSONObject) JSONValue.parse(userInfo);
//      String fullName = jsonObjectUser.get("full_name").toString();
//      String id = (String) jsonObjectUser.get("id");
//      user = new User(fullName, id);
//    }
//
//    return user;
//  }

  private User requestLogin(String username, String password) throws IOException {
    User user = null;
    Socket socket = new Socket("127.0.0.1", 1472);
    DataOutputStream dataOutputStream = new DataOutputStream(
      socket.getOutputStream()
    );

    byte[] bufferSend = null;
    byte[] bufferReceive = new byte[4096];

    JSONObject request = new JSONObject();
    request.put("head", "login");
    JSONObject requestBody = new JSONObject();
    requestBody.put("username", username);
    requestBody.put("password", password);
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

    JSONObject jsonObject = (JSONObject) JSONValue.parse(response);
    String responseBody = jsonObject.get("body").toString();

    JSONObject jsonObjectBody = (JSONObject) JSONValue.parse(responseBody);
    String result = jsonObjectBody.get("status").toString();
    if (Objects.equals(result, "false")) {
      return null;
    } else {
      String userInfo = jsonObjectBody.get("user").toString();
      JSONObject jsonObjectUser = (JSONObject) JSONValue.parse(userInfo);
      String fullName = jsonObjectUser.get("full_name").toString();
      String id = (String) jsonObjectUser.get("id");
      user = new User(fullName, id);
    }

    return user;
  }

}
