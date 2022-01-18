# Youtok
- Video sharing application using Java for UI & C for socket server.
- Devoloped on Ubuntu 20.
## Tool Requirements
- Install OpenJDK 17 (ignore this if Java had been installed):
    ```
    sudo add-apt-repository ppa:linuxuprising/java -y
    ```
    ```
    sudo apt update
    ```
    ```
    sudo apt-get install oracle-java17-installer oracle-java17-set-default
    ```
- Download JavaFX runtime from gluonhq from: https://gluonhq.com/products/javafx/
    **Note**:

        - OS: Linux
        - Architecture: x64
        - Type: SDK

    After downloading, unzip, put child directory of unziped directory in a fixed directory. Example: /home/htg/Library/javafx-sdk...
- Install IntelliJ IDEA
    - Download tar.gz file from Jetbrains: https://www.jetbrains.com/idea/download/#section=linux
    - Unzip, move child directory of unziped directory to outside, then use this command to move it to /opt:
        ```
        sudo mv Downloads/idea... /opt
        ```
    - Change directory to bin directory of IntelliJ in /opt:
        ```
        cd /opt/idea.../bin
        ```
    - Run this command to open IntelliJ:
        ```
        bash idea.sh
        ```
    - Select **Tools** -> **Create Desktop Entry...** to create icon in **Show all applications**
- Install json C for server:
  ```
  sudo apt install libjson-c-dev
  ```

## Set up IDE

- Open project with IntelliJ.
- Set JDK
    - File -> Project Structure
    - In tab Project, select Project SDK from detected SDKs.
- Create a library
    - In tab Libraries, add Project Library -> Java
    - Point to lib directory of javafx-sdk downloaded above (/home/htg/Library/javafx-sdk.../lib)
- Add VM options:
    - Run -> Edit Configurations...
    - Modify options -> Add VM options, then paste this in VM options field:
    ```
    --module-path pathtolib --add-modules javafx.controls,javafx.fxml
    ```
    - Example: --module-path /home/htg/Library/javafx-sdk-17.0.1/lib --add-modules javafx.controls,javafx.fxml
