package model;

public final class UserSession {
    private User user;
    private final static UserSession USER_SESSION = new UserSession();

    private UserSession() {}

    public static UserSession getUserSession() {
        return USER_SESSION;
    }

    public void setUser(User user) {
        this.user = user;
    }

    public User getUser() {
        return this.user;
    }
}
