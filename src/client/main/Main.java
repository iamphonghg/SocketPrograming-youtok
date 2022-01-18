package client.main;

import client.controller.MainController;
import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;
import org.json.simple.JSONObject;
import org.json.simple.JSONValue;

public class Main extends Application {
  public static MainController mainController;
  public static Stage mainStage;

  @Override
  public void start(Stage stage) throws Exception {
    mainStage = stage;

    FXMLLoader loader = new FXMLLoader(getClass().getResource("../fxml/main.fxml"));
    Parent root = loader.load();
    mainController = loader.getController();
    stage.setTitle("Youtok");
    Scene scene = new Scene(root);
    stage.setScene(scene);
    stage.show();
  }

  public static void main(String[] args) {
    launch(args);

  }

}
