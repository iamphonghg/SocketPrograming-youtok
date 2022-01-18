#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <json-c/json.h>
#include <mysql/mysql.h>

const char *server = "localhost";
const char *user = "root";
const char *password = "";
const char *database = "youtok";

int main() {
  MYSQL *conn;
  MYSQL_RES *res;
  MYSQL_ROW row;

  struct json_object *fetch_all_videos_response = json_object_new_object();

  conn = mysql_init(NULL);

  char *query_string = "select * from videos";

  if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0))
  {
    fprintf(stderr, "%s\n", mysql_error(conn));
    exit(1);
  }

  if (mysql_query(conn, query_string))
  {
    fprintf(stderr, "%s\n", mysql_error(conn));
    exit(1);
  }

  res = mysql_store_result(conn);

  struct json_object *videos_list = json_object_new_array();

  if (mysql_num_rows(res) > 0)
  {
    printf("Fetch successfully!\n");
    while ((row = mysql_fetch_row(res)) != NULL)
    {
      char id[10];
      char title[255];
      char description[255];
      char privacy[30];
      char filename[255];

      strcpy(id, row[0]);
      strcpy(title, row[2]);
      strcpy(description, row[3]);
      strcpy(privacy, row[4]);
      strcpy(filename, row[5]);

      struct json_object *video = json_object_new_object();

      json_object_object_add(video, "id", json_object_new_string(id));
      json_object_object_add(video, "title", json_object_new_string(title));
      json_object_object_add(video, "description", json_object_new_string(description));
      json_object_object_add(video, "privacy", json_object_new_string(privacy));
      json_object_object_add(video, "filename", json_object_new_string(filename));

      json_object_array_add(videos_list, video);

    }

  }
  else
  {
    printf("Fetch failed!\n");
    mysql_free_result(res);
    mysql_close(conn);
  }

  json_object_object_add(fetch_all_videos_response, "head", json_object_new_string("fetch_all_videos_response"));

  struct json_object *body_object = json_object_new_object();

  json_object_object_add(body_object, "videos_list", videos_list);

  json_object_object_add(fetch_all_videos_response, "body", body_object);

  printf("%s\n", json_object_to_json_string(fetch_all_videos_response));

  mysql_free_result(res);
  mysql_close(conn);

  return 0;
}


// int main() {
//   struct json_object *fetch_response = json_object_new_object();

//   struct json_object *videos_list = json_object_new_array();

//   json_object_array_add(videos_list, json_object_new_string("phong"));
//   json_object_array_add(videos_list, json_object_new_string("hoa"));
//   json_object_array_add(videos_list, json_object_new_string("chi"));
//   json_object_object_add(fetch_response, "videos_list", videos_list);

//   printf("%s\n", json_object_to_json_string(fetch_response));

// }
