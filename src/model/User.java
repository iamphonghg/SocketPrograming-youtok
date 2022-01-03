package model;

public class User {
    private String fullName;
    private String id;

    public User(String fullName, String id) {
        this.fullName = fullName;
        this.id = id;
    }

    public String getFullName() {
        return fullName;
    }

    public String getId() {
        return id;
    }

    public void setFullName(String fullName) {
        this.fullName = fullName;
    }

    public void setId(String id) {
        this.id = id;
    }
}
