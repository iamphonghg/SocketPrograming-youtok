#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>

void createUsersTable(MYSQL *);
void createVideosTable(MYSQL *);
void seedUser(MYSQL *);
void seedVideo(MYSQL *);

int main(int argc, char **argv) {
  MYSQL *conn;
  MYSQL_RES *res;
  MYSQL_ROW row;

  char *server = "localhost";
  char *user = "root";
  char *password = "root";
  char *database = "youtok";

  conn = mysql_init(NULL);

  /* Connect to database */
  if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0))
  {
    fprintf(stderr, "%s\n", mysql_error(conn));
    exit(1);
  }

  createUsersTable(conn);
  createVideosTable(conn);
  seedUser(conn);
  seedVideo(conn);

  mysql_close(conn);
}

void createUsersTable(MYSQL *conn)
{
  if (mysql_query(
    conn,
    "CREATE TABLE `users` ( `id` int(11) PRIMARY KEY AUTO_INCREMENT, `full_name` varchar(30) NOT NULL, `username` varchar(30) NOT NULL, `password` varchar(30) NOT NULL, `created_at` timestamp NOT NULL DEFAULT current_timestamp() )"))
  {
    fprintf(stderr, "%s\n", mysql_error(conn));
    exit(1);
  }
  else
  {
    printf("Successfully added table users\n");
  }
}

void createVideosTable(MYSQL *conn)
{
  if (mysql_query(
    conn,
    "CREATE TABLE `videos` ( `id` int(11) PRIMARY KEY AUTO_INCREMENT, `user_id` int(11)  NOT NULL, `title` varchar(255) NOT NULL, `description` varchar(255) NOT NULL, `privacy` varchar(255) NOT NULL, `filename` varchar(255) NOT NULL, `content_type` varchar(255) NOT NULL, `byte_size` int(11) NOT NULL, `created_at` timestamp NULL DEFAULT current_timestamp() )"))
  {
    fprintf(stderr, "%s\n", mysql_error(conn));
    exit(1);
  }
  else
  {
    printf("Successfully added table videos\n");
  }
}

void addForeignKey(MYSQL *conn)
{
  if (mysql_query(
    conn,
    "ALTER TABLE `videos` ADD CONSTRAINT `videos_fk0` FOREIGN KEY (`user_id`) REFERENCES `users`(`id`)"))
  {
    fprintf(stderr, "%s\n", mysql_error(conn));
    exit(1);
  }
  else
  {
    printf("Successfully added foreign key\n");
  }
}

void seedUser(MYSQL *conn)
{
  if (mysql_query(
    conn,
    "INSERT INTO `users` (`full_name`, `username`, `password`) VALUES ('Nguyen Van Admin', 'admin', 'admin')"))
  {
    fprintf(stderr, "%s\n", mysql_error(conn));
    exit(1);
  }
  else
  {
    printf("Successfully added user\n");
  }
}

void seedVideo(MYSQL *conn)
{
  if (mysql_query(conn, "INSERT INTO videos (user_id, title, description, privacy, filename, content_type, byte_size) VALUES (1, '3 ngay lam chu mon Lap trinh mang', 'Huong dan vuot qua mon Lap trinh mang cua thay giao Hoang Trong Nghien', 'public', 'Lamchulaptrinhmang.mp4', 'mp4', 2048)"))
  {
    fprintf(stderr, "%s\n", mysql_error(conn));
    exit(1);
  }
  else
  {
    printf("Successfully added video\n");
  }
  if (mysql_query(conn, "INSERT INTO videos (user_id, title, description, privacy, filename, content_type, byte_size) VALUES (1, 'Thuat toan khong kho', 'Video day thuat toan cua thay giao Pham Tuan Duc', 'public', 'thuattoankhongkho.mp4', 'mp4', 4096)"))
  {
    fprintf(stderr, "%s\n", mysql_error(conn));
    exit(1);
  }
  else
  {
    printf("Successfully added video\n");
  }

  if (mysql_query(conn, "INSERT INTO videos (user_id, title, description, privacy, filename, content_type, byte_size) VALUES (1, 'Huy diet giai tich', 'Qua mon giai tich de dang cung thay giao Hoang Gia Phong, voi 5 nam kinh nghiem huy diet giai tich bach khoa', 'public', 'huydietgiaitch.mp4', 'mp4', 10248)"))
  {
    fprintf(stderr, "%s\n", mysql_error(conn));
    exit(1);
  }
  else
  {
    printf("Successfully added video\n");
  }

  if (mysql_query(conn, "INSERT INTO videos (user_id, title, description, privacy, filename, content_type, byte_size) VALUES (2, 'Lam giau khong kho', 'Bi kip lam giau cua thay Huan Hoa Hong', 'public', 'lamgiaukhongkho.mp4', 'mp4', 10248)"))
  {
    fprintf(stderr, "%s\n", mysql_error(conn));
    exit(1);
  }
  else
  {
    printf("Successfully added video\n");
  }

  if (mysql_query(conn, "INSERT INTO videos (user_id, title, description, privacy, filename, content_type, byte_size) VALUES (2, 'Day con lam giau', 'Cach day con lam giau cua doanh nhan Hoang Trong Nghien', 'public', 'dayconlamgiau.mp4', 'mp4', 10248)"))
  {
    fprintf(stderr, "%s\n", mysql_error(conn));
    exit(1);
  }
  else
  {
    printf("Successfully added video\n");
  }
}
