//package client.main.Trash;
//import com.sun.jna.Native;
//import com.sun.jna.NativeLibrary;
//import javafx.application.Application;
//import javafx.scene.Scene;
//import javafx.scene.image.ImageView;
//import javafx.scene.layout.BorderPane;
//import javafx.scene.paint.Color;
//import javafx.stage.Stage;
//import uk.co.caprica.vlcj.binding.LibVlc;
//import uk.co.caprica.vlcj.binding.RuntimeUtil;
//import uk.co.caprica.vlcj.factory.MediaPlayerFactory;
//import uk.co.caprica.vlcj.factory.discovery.NativeDiscovery;
//import uk.co.caprica.vlcj.player.base.MediaPlayer;
//import uk.co.caprica.vlcj.player.base.MediaPlayerEventAdapter;
//import uk.co.caprica.vlcj.player.embedded.EmbeddedMediaPlayer;
//
//import java.io.File;
//import java.net.URISyntaxException;
//import java.util.List;
//
//import static uk.co.caprica.vlcj.javafx.videosurface.ImageViewVideoSurfaceFactory.videoSurfaceForImageView;
//
///**
// *
// */
//public class TestVlc extends Application {
//
//  private final MediaPlayerFactory mediaPlayerFactory;
//
//  private final EmbeddedMediaPlayer embeddedMediaPlayer;
//
//  private ImageView videoImageView;
//
//  public TestVlc() {
//    this.mediaPlayerFactory = new MediaPlayerFactory();
//    this.embeddedMediaPlayer = mediaPlayerFactory.mediaPlayers().newEmbeddedMediaPlayer();
//    this.embeddedMediaPlayer.events().addMediaPlayerEventListener(new MediaPlayerEventAdapter() {
//      @Override
//      public void playing(MediaPlayer mediaPlayer) {
//      }
//
//      @Override
//      public void paused(MediaPlayer mediaPlayer) {
//      }
//
//      @Override
//      public void stopped(MediaPlayer mediaPlayer) {
//      }
//
//      @Override
//      public void timeChanged(MediaPlayer mediaPlayer, long newTime) {
//      }
//    });
//  }
//
//  @Override
//  public void init() {
//    this.videoImageView = new ImageView();
//    this.videoImageView.setPreserveRatio(true);
//
//    embeddedMediaPlayer.videoSurface().set(videoSurfaceForImageView(this.videoImageView));
//  }
//
//  @Override
//  public final void start(Stage primaryStage) throws Exception {
//
//    BorderPane root = new BorderPane();
//    root.setStyle("-fx-background-color: black;");
//
//    videoImageView.fitWidthProperty().bind(root.widthProperty());
//    videoImageView.fitHeightProperty().bind(root.heightProperty());
//
//    root.widthProperty().addListener((observableValue, oldValue, newValue) -> {
//      // If you need to know about resizes
//    });
//
//    root.heightProperty().addListener((observableValue, oldValue, newValue) -> {
//      // If you need to know about resizes
//    });
//
//    root.setCenter(videoImageView);
//
//    Scene scene = new Scene(root, 1200, 675, Color.BLACK);
//    primaryStage.setTitle("vlcj JavaFX");
//    primaryStage.setScene(scene);
//    primaryStage.show();
//
//    embeddedMediaPlayer.media().play("/home/htg/Documents/test.mp4");
//
//    embeddedMediaPlayer.controls().setPosition(0.4f);
//  }
//
//  @Override
//  public final void stop() {
//    embeddedMediaPlayer.controls().stop();
//    embeddedMediaPlayer.release();
//    mediaPlayerFactory.release();
//  }
//
//  private static final String NATIVE_LIBRARY_SEARCH_PATH = "/usr/lib/x86_64-linux-gnu";
//
//  public static void main(String[] args) throws URISyntaxException {
////    NativeLibrary.addSearchPath("vlc", "/usr/lib/x86_64-linux-gnu/");
//////    ClassLoader cl = TestVlc.class.getClassLoader();
//////    NativeLibrary.addSearchPath("vlc", "/usr/lib/x86_64-linux-gnu/");
//////    Native.loadLibrary(RuntimeUtil.getLibVlcLibraryName(), LibVlc.class);
//////    System.out.println(RuntimeUtil.getLibVlcLibraryName());
////    launch(args);
////    NativeLibrary.addSearchPath("libvlc", NATIVE_LIBRARY_SEARCH_PATH);
//    System.out.println(LibVlc.libvlc_get_version());
//    boolean found = new NativeDiscovery().discover();
//    System.out.println(found);
////    System.out.println(LibVlc.libvlc_get_version());
//  }
//}
