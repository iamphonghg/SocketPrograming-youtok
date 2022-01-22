package client.model;

public class Video {
  private String id;
  private String title;
  private String description;
  private String privacy;
  private String filename;
  private String authorName;
  public Video() {
  }

  public Video(
    String id,
    String title,
    String description,
    String privacy,
    String filename,
    String authorName
  ) {
    this.id = id;
    this.title = title;
    this.description = description;
    this.privacy = privacy;
    this.filename = filename;
    this.authorName = authorName;
  }

  public String getId() {
    return id;
  }

  public void setId(String id) {
    this.id = id;
  }

  public String getTitle() {
    return title;
  }

  public void setTitle(String title) {
    this.title = title;
  }

  public String getDescription() {
    return description;
  }

  public void setDescription(String description) {
    this.description = description;
  }

  public String getPrivacy() {
    return privacy;
  }

  public void setPrivacy(String privacy) {
    this.privacy = privacy;
  }

  public String getFilename() {
    return filename;
  }

  public void setFilename(String filename) {
    this.filename = filename;
  }

  public String getAuthorName() {
    return authorName;
  }

  public void setAuthorName(String authorName) {
    this.authorName = authorName;
  }
}
