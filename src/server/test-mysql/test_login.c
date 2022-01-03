#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{


  char *server = "localhost";
  char *user = "root";
  char *password = "";
  char *database = "youtok";

  char query_string[147];

  strcpy(query_string, "select * from users where username=\"");
  strcat(query_string, user);
  strcat(query_string, "\" and password=\"");
  strcat(query_string, password);
  strcat(query_string, "\" limit 1");


  printf("%s\n", query_string);

  MYSQL *conn;
  MYSQL_RES *res;
  MYSQL_ROW row;

  conn = mysql_init(NULL);

  if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
    fprintf(stderr, "%s\n", mysql_error(conn));
    exit(1);
  }

  if (mysql_query(conn, "select * from users where username=\"admin\" and password=\"123123\" limit 1")) {
    fprintf(stderr, "%s\n", mysql_error(conn));
    exit(1);
  }

  // res = mysql_use_result(conn);

  // if (res) {
  //   printf("Log in successfully!\n");
  // } else {
  //   printf("Log in failed!\n");
  // }
  // row = mysql_fetch_row(res);
  // printf("%s\n", row[1]);


  // mysql_free_result(res);
  // mysql_close(conn);
}
