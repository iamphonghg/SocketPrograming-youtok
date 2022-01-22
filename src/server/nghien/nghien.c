#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include <ctype.h>
#include <json-c/json.h>
#include <mysql/mysql.h>

const char *server = "localhost";
const char *user = "root";
const char *password = "ikeXUH83";
const char *database = "youtok";


const char *show_videos_after_user_login(const char *username, const char *user_password);
const char *create_search_videos_after_login_response(const char* user_id);
const char *search_video_after_user_login(const char* search_string);


int main()

{
     printf("%s",show_videos_after_user_login("nghienht","123"));
    return 0;
}

const char *show_videos_after_user_login(const char *username, const char *user_password)
{
  MYSQL *conn;
  MYSQL_RES *res;
  MYSQL_ROW row;

  struct json_object *user_json = json_object_new_object();

  conn = mysql_init(NULL);


  char user_id[8];
  char query_string[255];

  strcpy(query_string, "select * from users where username=\"");
  strcat(query_string, username);
  strcat(query_string, "\" and password=\"");
  strcat(query_string, user_password);
  strcat(query_string, "\" limit 1");

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

  if (mysql_num_rows(res) > 0)
  {

    row = mysql_fetch_row(res);
    //chay tiep hàm show ra

    strcpy(user_id, row[0]);
        // printf("%s", create_search_videos_after_login_response(user_id,""));

  }
  else
  {
    printf("Log in failed!\n");
    mysql_free_result(res);
    mysql_close(conn);
    return NULL;
  }


//   json_object_object_add(user_json, "id", json_object_new_string(user_id));

  mysql_free_result(res);
  mysql_close(conn);

  return create_search_videos_after_login_response(user_id);
}


//sau login
const char *create_search_videos_after_login_response(const char* user_id)
{
  MYSQL *conn;
  MYSQL_RES *res;
  MYSQL_ROW row;

  struct json_object *search_videos_after_login_response = json_object_new_object();

  conn = mysql_init(NULL);

  char id[8];
  char query_string[255];



  // char *query_string = "select * from videos";

  strcpy(query_string, "select * from videos where privacy = 'public' or user_id=\"");
  strcat(query_string, user_id);
  strcat(query_string, "\"");

  // strcat(query_string, "\" limit 1");


  if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0))
  {
    fprintf(stderr, "%s\n", mysql_error(conn));
    exit(1);
  }
//ketnoi
  if (mysql_query(conn, query_string))
  {
    fprintf(stderr, "%s\n", mysql_error(conn));
    exit(1);
  }

  res = mysql_store_result(conn);

  struct json_object *videos_list = json_object_new_array();

  if (mysql_num_rows(res) > 0)
  {
    printf("Search successfully!\n");
    while ((row = mysql_fetch_row(res)) != NULL)
    {
      char id[10];
      char user_id[10];
      char title[255];
      char description[255];
      char privacy[30];
      char filename[255];
      char content_type[255];
      char byte_size[10];
      char created_at[255];

      strcpy(id, row[0]);
      strcpy(user_id,row[1]);
      strcpy(title, row[2]);
      strcpy(description, row[3]);
      strcpy(privacy, row[4]);
      strcpy(filename, row[5]);
      strcpy(content_type, row[6]);
      strcpy(byte_size, row[7]);
      strcpy(created_at, row[8]);

      struct json_object *video = json_object_new_object();

      json_object_object_add(video, "id", json_object_new_string(id));
      json_object_object_add(video, "user_id", json_object_new_string(user_id));
      json_object_object_add(video, "title", json_object_new_string(title));
      json_object_object_add(video, "description", json_object_new_string(description));
      json_object_object_add(video, "privacy", json_object_new_string(privacy));
      json_object_object_add(video, "filename", json_object_new_string(filename));
      json_object_object_add(video, "content_type", json_object_new_string(content_type));
      json_object_object_add(video, "byte_site", json_object_new_string(byte_size));
      json_object_object_add(video, "create_at", json_object_new_string(created_at));
      json_object_array_add(videos_list, video);
    }
  }
  else
  {
    printf("Fetch failed!\n");
    mysql_free_result(res);
    mysql_close(conn);
    return NULL;
  }

  json_object_object_add(search_videos_after_login_response, "head", json_object_new_string("search_all_video_after_login"));

  struct json_object *body_object = json_object_new_object();

  json_object_object_add(body_object, "videos_list", videos_list);

  json_object_object_add(search_videos_after_login_response, "body", body_object);

  mysql_free_result(res);
  mysql_close(conn);

  return json_object_to_json_string(search_videos_after_login_response);
}


