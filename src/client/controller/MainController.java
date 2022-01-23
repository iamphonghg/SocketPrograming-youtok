package client.controller;

import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.fxml.Initializable;
import javafx.scene.control.Button;
import javafx.scene.control.MenuButton;
import javafx.scene.control.MenuItem;
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
  private BorderPane searchBorderPane;
  @FXML
  private BorderPane uploadBorderPane;
  @FXML
  public BorderPane watchVideoBorderPane;
  public BorderPane uploadProcessBorderPane;
  public BorderPane myVideoBorderPane;

  public static UploadProcessController uploadProcessController;
  public static HomeController homeController;
  public static WatchVideoController watchVideoController;
  public static SearchController searchController;
  public static MyVideoController myVideoController;

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

      FXMLLoader searchLoader = new FXMLLoader();
      searchLoader.setLocation(getClass().getResource(
        "../fxml/result-search.fxml"
      ));
      searchBorderPane = searchLoader.load();
      searchController = searchLoader.getController();

      FXMLLoader myVideoLoader = new FXMLLoader();
      myVideoLoader.setLocation(getClass().getResource(
        "../fxml/my-video.fxml"
      ));
      myVideoBorderPane = myVideoLoader.load();
      myVideoController = myVideoLoader.getController();

      mainStackPane.getChildren().add(myVideoBorderPane);
      mainStackPane.getChildren().add(searchBorderPane);
      mainStackPane.getChildren().add(watchVideoBorderPane);
      mainStackPane.getChildren().add(uploadBorderPane);
      mainStackPane.getChildren().add(uploadProcessBorderPane);
      mainStackPane.getChildren().add(loginBorderPane);
      mainStackPane.getChildren().add(homeBorderPane);

      uploadButton.setVisible(false);
      userMenuButton.setVisible(false);

      MenuItem myVideoChoice = new MenuItem("My videos");
      MenuItem logOutChoice = new MenuItem("Log out");
      userMenuButton.getItems().clear();
      userMenuButton.getItems().add(myVideoChoice);
      userMenuButton.getItems().add(logOutChoice);

      myVideoChoice.setOnAction(new EventHandler<ActionEvent>() {
        @Override
        public void handle(ActionEvent actionEvent) {
          try {
            MyVideoController.updateMyVideoList();
          } catch (IOException e) {
            e.printStackTrace();
          }
          myVideoBorderPane.toFront();
        }
      });

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
      SearchController.updateSearchResultList(this.searchInput.getText());
      searchBorderPane.toFront();
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


}
