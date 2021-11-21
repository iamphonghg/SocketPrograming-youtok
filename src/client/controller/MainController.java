package client.controller;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.TextField;
import javafx.scene.layout.BorderPane;

import java.net.URL;
import java.util.ResourceBundle;


public class MainController implements Initializable {
    @FXML
    private Button buttonAvatar;

    @FXML
    private Button buttonLogin;

    @FXML
    private Button buttonSearch;

    @FXML
    private TextField inputSearch;

    @FXML
    private Label labelTest;

    @FXML
    private BorderPane paneMain;

    @Override
    public void initialize(URL url, ResourceBundle resourceBundle) {

    }

    public void handleClicks(ActionEvent event) {
        if (event.getSource() == buttonSearch) {
            labelTest.setText(inputSearch.getText());
        }
    }
}
