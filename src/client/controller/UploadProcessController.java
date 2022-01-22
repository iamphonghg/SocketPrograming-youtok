package client.controller;

import client.main.Main;
import client.model.UserSession;
import javafx.application.Platform;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.*;
import javafx.scene.input.MouseEvent;
import org.json.simple.JSONObject;

import java.io.*;
import java.net.Socket;
import java.net.URL;
import java.text.CharacterIterator;
import java.text.StringCharacterIterator;
import java.util.ResourceBundle;

public class UploadProcessController implements Initializable {
  @FXML
  private TextArea titleTextArea;
  @FXML
  private TextArea descriptionTextArea;
  @FXML
  private Label fileSizeLabel;
  @FXML
  private Label filenameLabel;
  @FXML
  private RadioButton privateRadioButton;
  @FXML
  private RadioButton publicRadioButton;
  @FXML
  private Label percentageLabel;
  @FXML
  private Button startUploadButton;
  @FXML
  private ProgressBar uploadProgressBar;
  @FXML
  private Label statusLabel;

  private ToggleGroup privacyGroup;

  public File fileToUpload;


  @Override
  public void initialize(URL url, ResourceBundle resourceBundle) {
    privacyGroup = new ToggleGroup();
    privateRadioButton.setToggleGroup(privacyGroup);
    publicRadioButton.setToggleGroup(privacyGroup);
  }

  public void handleMouseClicks(MouseEvent event) throws IOException, InterruptedException {
    if (event.getSource() == startUploadButton) {
      uploadFile(fileToUpload, fileToUpload.length());
    }
  }

  public void setFileNameAndSizeLabel(String filename, String fileSize) {
    filenameLabel.setText(filename);
    fileSizeLabel.setText(fileSize);
  }

  public String humanReadableByteCountSI(long bytes) {
    if (-1000 < bytes && bytes < 1000) {
      return bytes + " B";
    }
    CharacterIterator ci = new StringCharacterIterator("kMGTPE");
    while (bytes <= -999_950 || bytes >= 999_950) {
      bytes /= 1000;
      ci.next();
    }
    return String.format("%.1f %cB", bytes / 1000.0, ci.current());
  }

  public void uploadFile(File fileToUpload, long fileSize) throws IOException, InterruptedException {
    Socket socket = new Socket(Main.SERVER_IP, 1472);
    DataOutputStream dataOutputStream = new DataOutputStream(
      socket.getOutputStream()
    );

    byte[] bufferSend = null;

    JSONObject request = new JSONObject();
    request.put("head", "upload_new_video");
    JSONObject requestBody = new JSONObject();
    requestBody.put("user_id", UserSession.getUserSession().getUser().getId());
    requestBody.put("title", this.titleTextArea.getText());
    requestBody.put("description", this.descriptionTextArea.getText());
    if (this.privateRadioButton.isSelected()) {
      requestBody.put("privacy", "private");
    } else {
      requestBody.put("privacy", "public");
    }
    requestBody.put("byte_size", fileToUpload.length());
    requestBody.put("filename", fileToUpload.getName());
    request.put("body", requestBody);
    bufferSend = request.toJSONString().getBytes();

    dataOutputStream.write(bufferSend);
    dataOutputStream.flush();

    byte[] bytes = new byte[1024];

    FileInputStream fileInputStream = new FileInputStream(fileToUpload);
    BufferedInputStream bufferedInputStream = new BufferedInputStream(fileInputStream);

    OutputStream outputStream = socket.getOutputStream();


    int count = 0, total = 0;
    while (true) {
      if (!((count = bufferedInputStream.read(bytes)) > 0)) break;
      outputStream.write(bytes, 0, count);
      total += count;
      System.out.println(total);
      updateProgressBar((int) (total / fileSize) * 100);
    }
    statusLabel.setText("Upload successfully!");
    outputStream.flush();
    outputStream.close();
    fileInputStream.close();
    bufferedInputStream.close();
  }

  public void updateProgressBar(int percentageNumber) {
    percentageLabel.setText(String.valueOf(percentageNumber) + "%");
    uploadProgressBar.setProgress(percentageNumber / 100);
  }
}
