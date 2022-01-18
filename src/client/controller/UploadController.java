package client.controller;

import client.main.Main;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.Button;
import javafx.scene.control.RadioButton;
import javafx.scene.control.ToggleGroup;
import javafx.scene.input.MouseEvent;
import javafx.stage.FileChooser;

import java.io.File;
import java.net.URL;
import java.util.ResourceBundle;

public class UploadController implements Initializable {
  @FXML
  private Button selectFileButton;

  final FileChooser fileChooser = new FileChooser();

  @Override
  public void initialize(URL url, ResourceBundle resourceBundle) {

  }

  public void handleMouseClicks(MouseEvent event) {
    if (event.getSource() == selectFileButton) {
      handleChooseFile();
    }
  }

  private void handleChooseFile() {
    File fileToUpload = fileChooser.showOpenDialog(Main.mainStage);
    if (fileToUpload != null) {
      MainController.uploadProcessController.fileToUpload = fileToUpload;
      String filename = fileToUpload.getName();
      String fileSize = MainController.uploadProcessController.humanReadableByteCountSI(
        fileToUpload.length()
      );
      MainController.uploadProcessController.setFileNameAndSizeLabel(
        filename, fileSize
      );
      Main.mainController.uploadProcessBorderPane.toFront();
    }
  }
}
