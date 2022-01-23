#include "prototype.h"

/* for streaming */
static gboolean
timeout(GstRTSPServer *server)
{
  GstRTSPSessionPool *pool;

  pool = gst_rtsp_server_get_session_pool(server);
  gst_rtsp_session_pool_cleanup(pool);
  g_object_unref(pool);

  return TRUE;
}

void *start_stream(void *filenameInput)
{
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
  char *filename = (char *)filenameInput;

  GMainLoop *loop;
  GstRTSPServer *server;
  GstRTSPMountPoints *mounts;
  GstRTSPMediaFactory *factory;
  char source[250];
  strcpy(source, "( filesrc location=storage/");
  strcat(source, filename);
  strcat(source, " ! qtdemux ! h264parse ! rtph264pay name=pay0 pt=96 )");

  printf("%s\n", filename);
  printf("%s\n", source);

  gst_init(NULL, NULL);
  loop = g_main_loop_new(NULL, FALSE);

  server = gst_rtsp_server_new();

  mounts = gst_rtsp_server_get_mount_points(server);
  factory = gst_rtsp_media_factory_new();
  gst_rtsp_media_factory_set_launch(factory, source);

  char mountPointStr[100];
  strcpy(mountPointStr, "/");
  strcat(mountPointStr, filename);

  gst_rtsp_mount_points_add_factory(mounts, mountPointStr, factory);

  g_object_unref(mounts);
  if (gst_rtsp_server_attach(server, NULL) == 0)
    goto failed;

  g_timeout_add_seconds(2, (GSourceFunc)timeout, server);
  g_print("stream ready at rtsp://127.0.0.1:8554%s\n", mountPointStr);
  g_main_loop_run(loop);

  /* ERRORS */
  failed:
  {
    g_print("failed to attach the server\n");
  }
}

/* handle request from client */
void *handle_request(void *client_socket)
{
  int connfd = *(int *)client_socket;

  char buffer[MAX_BUFFER];

  bzero(buffer, MAX_BUFFER);
  read(connfd, buffer, sizeof(buffer));

  printf("Client: %s\n", buffer);
  const char *request_head = get_request_head(buffer);
  const char *request_body = get_request_body(buffer);
  const char *response = NULL;

  if (strcmp(request_head, "login") == 0)
  {
    response = create_login_response(request_body);
    write(connfd, response, strlen(response));
  }
  else if (strcmp(request_head, "fetch_all_videos_no_login") == 0)
  {
    response = create_fetch_all_videos_no_login_response();
    write(connfd, response, strlen(response));
  }
  else if (strcmp(request_head, "fetch_all_videos_has_login") == 0)
  {
    const char *user_id = get_current_user_id(request_body);
    response = create_fetch_all_videos_has_login_response(user_id);
    write(connfd, response, strlen(response));
  }
  else if (strcmp(request_head, "upload_new_video") == 0)
  {
    handle_upload_new_video(connfd, request_body);
  }
  else if (strcmp(request_head, "search_video_no_login") == 0) {
    const char *search_key = get_search_key(request_body);
    response = create_search_video_no_login_response(search_key);
    write(connfd, response, strlen(response));
  }
  else if (strcmp(request_head, "search_video_has_login") == 0) {
    const char *user_id = get_current_user_id(request_body);
    const char *search_key = get_search_key(request_body);
    response = create_search_video_has_login_response(user_id, search_key);
    write(connfd, response, strlen(response));
  }
  else if (strcmp(request_head, "my_video") == 0) {
    const char *user_id = get_current_user_id(request_body);
    response = create_my_video_response(user_id);
    write(connfd, response, strlen(response));
  }
  else if (strcmp(request_head, "update_privacy") == 0) {
    const char *video_id = get_video_id(request_body);
    const char *privacy = get_privacy(request_body);
    update_privacy(video_id, privacy);
  }
  else if (strcmp(request_head, "watch_video") == 0)
  {
    response = create_watch_video_response(request_body, 1);
    write(connfd, response, strlen(response));
    char filename[100];
    strcpy(filename, get_filename_string(request_body));
    start_stream(filename);
  }
  else if (strcmp(request_head, "cancel_stream") == 0)
  {
    // printf("%s\n", request_body);
    // int thread_no_to_kill = get_running_thread_no_to_kill(request_body);
    // pthread_cancel(threads[thread_no_to_kill]);
    // pthread_join(threads[thread_no_to_kill], NULL);
  }

  if (response != NULL)
  {
    printf("%ld\n", strlen(response));
    printf("Response: %s\n", response);
  }
}

const char *get_video_id(const char *request_body) {
  struct json_object *parsed_body_json;
  struct json_object *video_id;

  parsed_body_json = json_tokener_parse(request_body);

  json_object_object_get_ex(parsed_body_json, "video_id", &video_id);
  printf("%s\n", json_object_to_json_string(video_id));

  return json_object_get_string(video_id);
}

const char *get_privacy(const char *request_body) {
  struct json_object *parsed_body_json;
  struct json_object *privacy;

  parsed_body_json = json_tokener_parse(request_body);

  json_object_object_get_ex(parsed_body_json, "privacy", &privacy);
  printf("%s\n", json_object_to_json_string(privacy));

  return json_object_get_string(privacy);
}

void *update_privacy(const char *video_id, const char *privacy) {
  MYSQL *conn;
  MYSQL_RES *res;
  conn = mysql_init(NULL);

  char query_string[255];

  strcpy(query_string, "update youtok.videos set youtok.videos.privacy = \"");
  strcat(query_string, privacy);
  strcat(query_string, "\" where id = ");
  strcat(query_string, video_id);
  strcat(query_string, ";");

  printf("%s\n", query_string);

  if (!mysql_real_connect(conn, SERVER, USER, PASSWORD, DATABASE, 0, NULL, 0))
  {
    fprintf(stderr, "%s\n", mysql_error(conn));
    exit(1);
  }

  if (mysql_query(conn, query_string))
  {
    fprintf(stderr, "%s\n", mysql_error(conn));
  }
}

const char *create_my_video_response(const char *user_id)
{
  MYSQL *conn;
  MYSQL_RES *res;
  MYSQL_ROW row;

  struct json_object *my_video_response = json_object_new_object();

  conn = mysql_init(NULL);

  char query_string[255];
  strcpy(query_string, "select * from youtok.videos where youtok.videos.user_id = ");
  strcat(query_string, user_id);
  strcat(query_string, ";");

  if (!mysql_real_connect(conn, SERVER, USER, PASSWORD, DATABASE, 0, NULL, 0))
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
    printf("successfully!\n");
    while ((row = mysql_fetch_row(res)) != NULL)
    {
      char id[10];
      char user_id[10];
      char title[255];
      char description[255];
      char privacy[30];
      char filename[255];
      char content_type[10];
      char byte_size[10];
      char create_at[20];

      strcpy(id, row[0]);
      strcpy(user_id, row[1]);
      strcpy(title, row[2]);
      strcpy(description, row[3]);
      strcpy(privacy, row[4]);
      strcpy(filename, row[5]);
      strcpy(content_type, row[6]);
      strcpy(byte_size, row[7]);
      strcpy(create_at, row[8]);

      struct json_object *video = json_object_new_object();

      json_object_object_add(video, "id", json_object_new_string(id));
      json_object_object_add(video, "user_id", json_object_new_string(user_id));
      json_object_object_add(video, "title", json_object_new_string(title));
      json_object_object_add(video, "description", json_object_new_string(description));
      json_object_object_add(video, "privacy", json_object_new_string(privacy));
      json_object_object_add(video, "filename", json_object_new_string(filename));
      json_object_object_add(video, "content_type", json_object_new_string(content_type));
      json_object_object_add(video, "byte_size", json_object_new_string(byte_size));
      json_object_object_add(video, "create_at", json_object_new_string(create_at));

      json_object_array_add(videos_list, video);
    }
  }

  struct json_object *body_object = json_object_new_object();

  json_object_object_add(body_object, "videos_list", videos_list);

  json_object_object_add(my_video_response, "body", body_object);

  mysql_free_result(res);
  mysql_close(conn);

  return json_object_to_json_string(my_video_response);
}

const char *
rand_text(const char *file)
{
  char *result;
  result = (char *)malloc(sizeof(char *) * 11 + strlen(file));
  int i, rand_int;
  char char_set[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz&quot";

  for (i = 0; i < 9; i++)
  {
    result[i] = char_set[rand() % sizeof(char_set)];
  }
  result[9] = '-';
  strcat(result, file);
  return result;
}

const char *get_search_key(const char *request_body) {
  struct json_object *parsed_body_json;
  struct json_object *search_key;

  parsed_body_json = json_tokener_parse(request_body);

  json_object_object_get_ex(parsed_body_json, "search_key", &search_key);
  printf("%s\n", json_object_to_json_string(search_key));

  return json_object_get_string(search_key);
}

const char *create_search_video_has_login_response(const char *user_id, const char *search_key)
{
  MYSQL *conn;
  MYSQL_RES *res;
  MYSQL_ROW row;

  struct json_object *search_video_has_login_response = json_object_new_object();

  conn = mysql_init(NULL);

  char query_string[255];
  strcpy(query_string, "select videos.*,users.full_name FROM youtok.videos, youtok.users where youtok.videos.user_id = youtok.users.id and  youtok.videos.privacy = \"public\" and youtok.videos.title like \"%");
  strcat(query_string, search_key);
  strcat(query_string, "%\" union select videos.*,users.full_name FROM youtok.videos, youtok.users where youtok.videos.user_id = youtok.users.id and youtok.videos.user_id = ");
  strcat(query_string, user_id);
  strcat(query_string, " and youtok.videos.title like \"%");
  strcat(query_string, search_key);
  strcat(query_string, "%\";");

  printf("%s\n", query_string);

  if (!mysql_real_connect(conn, SERVER, USER, PASSWORD, DATABASE, 0, NULL, 0))
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
    printf("successfully!\n");
    while ((row = mysql_fetch_row(res)) != NULL)
    {
      char id[10];
      char user_id[10];
      char title[255];
      char description[255];
      char privacy[30];
      char filename[255];
      char content_type[10];
      char byte_size[10];
      char create_at[20];
      char author_name[50];

      strcpy(id, row[0]);
      strcpy(user_id, row[1]);
      strcpy(title, row[2]);
      strcpy(description, row[3]);
      strcpy(privacy, row[4]);
      strcpy(filename, row[5]);
      strcpy(content_type, row[6]);
      strcpy(byte_size, row[7]);
      strcpy(create_at, row[8]);
      strcpy(author_name, row[9]);

      struct json_object *video = json_object_new_object();

      json_object_object_add(video, "id", json_object_new_string(id));
      json_object_object_add(video, "user_id", json_object_new_string(user_id));
      json_object_object_add(video, "author_name", json_object_new_string(author_name));
      json_object_object_add(video, "title", json_object_new_string(title));
      json_object_object_add(video, "description", json_object_new_string(description));
      json_object_object_add(video, "privacy", json_object_new_string(privacy));
      json_object_object_add(video, "filename", json_object_new_string(filename));
      json_object_object_add(video, "content_type", json_object_new_string(content_type));
      json_object_object_add(video, "byte_size", json_object_new_string(byte_size));
      json_object_object_add(video, "create_at", json_object_new_string(create_at));

      json_object_array_add(videos_list, video);
    }
  }

  struct json_object *body_object = json_object_new_object();

  json_object_object_add(body_object, "videos_list", videos_list);

  json_object_object_add(search_video_has_login_response, "body", body_object);

  mysql_free_result(res);
  mysql_close(conn);

  return json_object_to_json_string(search_video_has_login_response);
}

const char *create_search_video_no_login_response(const char *search_key)
{
  MYSQL *conn;
  MYSQL_RES *res;
  MYSQL_ROW row;

  struct json_object *search_video_no_login_response = json_object_new_object();

  conn = mysql_init(NULL);

  char query_string[255];
  strcpy(query_string, "select videos.*,users.full_name FROM youtok.videos, youtok.users where youtok.videos.user_id = youtok.users.id and youtok.videos.privacy = \"public\" and youtok.videos.title like \"%");
  strcat(query_string, search_key);
  strcat(query_string, "%\";");
  printf("%s\n", query_string);

  if (!mysql_real_connect(conn, SERVER, USER, PASSWORD, DATABASE, 0, NULL, 0))
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
      char user_id[10];
      char title[255];
      char description[255];
      char privacy[30];
      char filename[255];
      char content_type[10];
      char byte_size[10];
      char create_at[20];
      char author_name[50];

      strcpy(id, row[0]);
      strcpy(user_id, row[1]);
      strcpy(title, row[2]);
      strcpy(description, row[3]);
      strcpy(privacy, row[4]);
      strcpy(filename, row[5]);
      strcpy(content_type, row[6]);
      strcpy(byte_size, row[7]);
      strcpy(create_at, row[8]);
      strcpy(author_name, row[9]);

      struct json_object *video = json_object_new_object();

      json_object_object_add(video, "id", json_object_new_string(id));
      json_object_object_add(video, "user_id", json_object_new_string(user_id));
      json_object_object_add(video, "author_name", json_object_new_string(author_name));
      json_object_object_add(video, "title", json_object_new_string(title));
      json_object_object_add(video, "description", json_object_new_string(description));
      json_object_object_add(video, "privacy", json_object_new_string(privacy));
      json_object_object_add(video, "filename", json_object_new_string(filename));
      json_object_object_add(video, "content_type", json_object_new_string(content_type));
      json_object_object_add(video, "byte_size", json_object_new_string(byte_size));
      json_object_object_add(video, "create_at", json_object_new_string(create_at));

      json_object_array_add(videos_list, video);
    }
  }

  json_object_object_add(search_video_no_login_response, "head", json_object_new_string("search_video_no_login_response"));

  struct json_object *body_object = json_object_new_object();

  json_object_object_add(body_object, "videos_list", videos_list);

  json_object_object_add(search_video_no_login_response, "body", body_object);

  mysql_free_result(res);
  mysql_close(conn);

  return json_object_to_json_string(search_video_no_login_response);
}

void receive_image(int connfd, const char *filename)
{
  char path[100];
  strcpy(path, "storage/");
  strcat(path, filename);
  FILE *newFile = fopen(path, "wb");

  ssize_t n;
  char buff[MAX_BUFFER] = {0};
  while ((n = recv(connfd, buff, MAX_BUFFER, 0)) > 0)
  {
    if (n == -1)
    {
      printf("Receive image failed!\n");
      exit(1);
    }

    if (fwrite(buff, sizeof(char), n, newFile) != n)
    {
      printf("Write file failed!\n");
      exit(1);
    }
    bzero(&buff, sizeof(buff));
  }
  printf("Successfully uploaded new video!\n");
  fclose(newFile);
}

void handle_upload_new_video(int connfd, const char *request_body)
{
  struct json_object *parsed_body_json;
  struct json_object *title;
  struct json_object *description;
  struct json_object *privacy;
  struct json_object *user_id;
  struct json_object *byte_size;
  struct json_object *filename;


  parsed_body_json = json_tokener_parse(request_body);

  json_object_object_get_ex(parsed_body_json, "user_id", &user_id);
  json_object_object_get_ex(parsed_body_json, "title", &title);
  json_object_object_get_ex(parsed_body_json, "description", &description);
  json_object_object_get_ex(parsed_body_json, "privacy", &privacy);
  json_object_object_get_ex(parsed_body_json, "filename", &filename);
  json_object_object_get_ex(parsed_body_json, "byte_size", &byte_size);

  srand(time(NULL));
  const char *file_name = rand_text(json_object_get_string(filename));
  receive_image(connfd, file_name);

  const char *response = create_upload_new_video_response(
    json_object_get_string(user_id),
    json_object_get_string(title),
    json_object_get_string(description),
    json_object_get_string(privacy),
    file_name,
    json_object_get_string(byte_size)
  );
  printf("%s\n", response);
}

const char *create_upload_new_video_response(
  const char *user_id,
  const char *title,
  const char *description,
  const char *privacy,
  const char *filename,
  const char *byte_size
) {
  MYSQL *conn;
  MYSQL_RES *res;

  struct json_object *response_json = json_object_new_object();
  json_object_object_add(response_json, "head", json_object_new_string("upload_new_video_response"));
  struct json_object *response_json_body = json_object_new_object();

  conn = mysql_init(NULL);

  srand(time(NULL));
  char query_string[1024];

  strcpy(query_string, "INSERT INTO videos (user_id, title, description, privacy, filename, content_type, byte_size) VALUES (");
  strcat(query_string, user_id);
  strcat(query_string, ",'");
  strcat(query_string, title);
  strcat(query_string, "','");
  strcat(query_string, description);
  strcat(query_string, "','");
  strcat(query_string, privacy);
  strcat(query_string, "','");
  strcat(query_string, filename);
  strcat(query_string, "','");
  strcat(query_string, "mp4");
  strcat(query_string, "',");
  strcat(query_string, byte_size);
  strcat(query_string, ");");
  printf("%s\n", query_string);

  if (!mysql_real_connect(conn, SERVER, USER, PASSWORD, DATABASE, 0, NULL, 0))
  {
    fprintf(stderr, "%s\n", mysql_error(conn));
    exit(1);
  }

  if (mysql_query(conn, query_string))
  {
    fprintf(stderr, "%s\n", mysql_error(conn));
    json_object_object_add(response_json_body, "status", json_object_new_string("false"));
    json_object_object_add(response_json, "body", response_json_body);
    mysql_close(conn);
    return json_object_to_json_string(response_json);
  }

  json_object_object_add(response_json_body, "status", json_object_new_string("true"));
  json_object_object_add(response_json, "body", response_json_body);
  mysql_close(conn);

  return json_object_to_json_string(response_json);
}

/* handle json */
const char *get_request_head(const char *request)
{
  struct json_object *head;
  struct json_object *parsed_json;

  parsed_json = json_tokener_parse(request);

  json_object_object_get_ex(parsed_json, "head", &head);

  return json_object_get_string(head);
}

const char *get_request_body(const char *request)
{
  struct json_object *body;
  struct json_object *parsed_json;

  parsed_json = json_tokener_parse(request);

  json_object_object_get_ex(parsed_json, "body", &body);

  return json_object_get_string(body);
}

int get_running_thread_no_to_kill(const char *request)
{
  struct json_object *parsed_body_json;
  struct json_object *thread_no;

  parsed_body_json = json_tokener_parse(request);

  json_object_object_get_ex(parsed_body_json, "thread_no", &thread_no);
  return json_object_get_int(thread_no);
}

const char *get_filename_string(const char *body)
{
  struct json_object *parsed_body_json;
  struct json_object *filename;

  parsed_body_json = json_tokener_parse(body);

  json_object_object_get_ex(parsed_body_json, "filename", &filename);
  printf("%s\n", json_object_to_json_string(filename));

  return json_object_get_string(filename);
}

const char *create_watch_video_response(const char *body, int streamThreadNo)
{
  struct json_object *parsed_body_json;
  struct json_object *filename;

  parsed_body_json = json_tokener_parse(body);

  json_object_object_get_ex(parsed_body_json, "filename", &filename);

  struct json_object *response_json = json_object_new_object();
  json_object_object_add(response_json, "head", json_object_new_string("watch_video_response"));

  struct json_object *response_body_json = json_object_new_object();
  json_object_object_add(response_body_json, "filename", filename);
  json_object_object_add(response_body_json, "thread_no", json_object_new_int64(streamThreadNo));
  json_object_object_add(response_json, "body", response_body_json);
  printf("%s\n", json_object_to_json_string(response_json));
  return json_object_to_json_string(response_json);
}

const char *create_login_response(const char *body)
{
  struct json_object *parsed_body_json;
  struct json_object *username;
  struct json_object *password;

  parsed_body_json = json_tokener_parse(body);

  json_object_object_get_ex(parsed_body_json, "username", &username);
  json_object_object_get_ex(parsed_body_json, "password", &password);

  const char *user_json_string = get_user_json_string(json_object_get_string(username), json_object_get_string(password));

  struct json_object *response_json = json_object_new_object();
  json_object_object_add(response_json, "head", json_object_new_string("login_response"));

  struct json_object *response_body_json = json_object_new_object();
  if (user_json_string)
  {
    json_object_object_add(response_body_json, "status", json_object_new_string("true"));
    json_object_object_add(response_body_json, "user", json_tokener_parse(user_json_string));
  }
  else
  {
    json_object_object_add(response_body_json, "status", json_object_new_string("false"));
  }
  json_object_object_add(response_json, "body", response_body_json);

  return json_object_to_json_string(response_json);
}

const char *get_user_json_string(const char *username, const char *user_password)
{
  MYSQL *conn;
  MYSQL_RES *res;
  MYSQL_ROW row;

  struct json_object *user_json = json_object_new_object();

  conn = mysql_init(NULL);

  char full_name[50];
  char id[8];
  char query_string[147];

  strcpy(query_string, "select * from users where username=\"");
  strcat(query_string, username);
  strcat(query_string, "\" and password=\"");
  strcat(query_string, user_password);
  strcat(query_string, "\" limit 1");

  if (!mysql_real_connect(conn, SERVER, USER, PASSWORD, DATABASE, 0, NULL, 0))
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
    printf("Log in successfully!\n");
    row = mysql_fetch_row(res);
    strcpy(full_name, row[1]);
    strcpy(id, row[0]);
  }
  else
  {
    printf("Log in failed!\n");
    mysql_free_result(res);
    mysql_close(conn);
    return NULL;
  }

  json_object_object_add(user_json, "full_name", json_object_new_string(full_name));
  json_object_object_add(user_json, "id", json_object_new_string(id));

  mysql_free_result(res);
  mysql_close(conn);

  return json_object_to_json_string(user_json);
}

const char *create_fetch_all_videos_no_login_response()
{
  MYSQL *conn;
  MYSQL_RES *res;
  MYSQL_ROW row;

  struct json_object *fetch_all_videos_no_login_response = json_object_new_object();

  conn = mysql_init(NULL);

  char *query_string = "select videos.*,users.full_name FROM youtok.videos, youtok.users where youtok.videos.user_id = youtok.users.id and youtok.videos.privacy = \"public\";";

  if (!mysql_real_connect(conn, SERVER, USER, PASSWORD, DATABASE, 0, NULL, 0))
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
      char user_id[10];
      char title[255];
      char description[255];
      char privacy[30];
      char filename[255];
      char content_type[10];
      char byte_size[10];
      char create_at[20];
      char author_name[50];

      strcpy(id, row[0]);
      strcpy(user_id, row[1]);
      strcpy(title, row[2]);
      strcpy(description, row[3]);
      strcpy(privacy, row[4]);
      strcpy(filename, row[5]);
      strcpy(content_type, row[6]);
      strcpy(byte_size, row[7]);
      strcpy(create_at, row[8]);
      strcpy(author_name, row[9]);

      struct json_object *video = json_object_new_object();

      json_object_object_add(video, "id", json_object_new_string(id));
      json_object_object_add(video, "user_id", json_object_new_string(user_id));
      json_object_object_add(video, "author_name", json_object_new_string(author_name));
      json_object_object_add(video, "title", json_object_new_string(title));
      json_object_object_add(video, "description", json_object_new_string(description));
      json_object_object_add(video, "privacy", json_object_new_string(privacy));
      json_object_object_add(video, "filename", json_object_new_string(filename));
      json_object_object_add(video, "content_type", json_object_new_string(content_type));
      json_object_object_add(video, "byte_size", json_object_new_string(byte_size));
      json_object_object_add(video, "create_at", json_object_new_string(create_at));

      json_object_array_add(videos_list, video);
    }
  }

  json_object_object_add(fetch_all_videos_no_login_response, "head", json_object_new_string("fetch_all_videos_no_login_response"));

  struct json_object *body_object = json_object_new_object();

  json_object_object_add(body_object, "videos_list", videos_list);

  json_object_object_add(fetch_all_videos_no_login_response, "body", body_object);

  mysql_free_result(res);
  mysql_close(conn);

  return json_object_to_json_string(fetch_all_videos_no_login_response);
}

const char *get_current_user_id(const char *request_body) {
  struct json_object *parsed_body_json;
  struct json_object *user_id;

  parsed_body_json = json_tokener_parse(request_body);

  json_object_object_get_ex(parsed_body_json, "user_id", &user_id);
  printf("%s\n", json_object_to_json_string(user_id));

  return json_object_get_string(user_id);
}

const char *create_fetch_all_videos_has_login_response(const char *user_id)
{
  MYSQL *conn;
  MYSQL_RES *res;
  MYSQL_ROW row;

  struct json_object *fetch_all_videos_has_login_response = json_object_new_object();

  conn = mysql_init(NULL);

  char query_string[255];
  strcpy(query_string, "select videos.*,users.full_name FROM youtok.videos, youtok.users where youtok.videos.user_id = youtok.users.id and  youtok.videos.privacy = \"public\" ");
  strcat(query_string, "union select videos.*,users.full_name FROM youtok.videos, youtok.users where youtok.videos.user_id = youtok.users.id and youtok.videos.user_id = ");
  strcat(query_string, user_id);
  strcat(query_string, ";");

  if (!mysql_real_connect(conn, SERVER, USER, PASSWORD, DATABASE, 0, NULL, 0))
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
      char user_id[10];
      char title[255];
      char description[255];
      char privacy[30];
      char filename[255];
      char content_type[10];
      char byte_size[10];
      char create_at[20];
      char author_name[50];

      strcpy(id, row[0]);
      strcpy(user_id, row[1]);
      strcpy(title, row[2]);
      strcpy(description, row[3]);
      strcpy(privacy, row[4]);
      strcpy(filename, row[5]);
      strcpy(content_type, row[6]);
      strcpy(byte_size, row[7]);
      strcpy(create_at, row[8]);
      strcpy(author_name, row[9]);

      struct json_object *video = json_object_new_object();

      json_object_object_add(video, "id", json_object_new_string(id));
      json_object_object_add(video, "user_id", json_object_new_string(user_id));
      json_object_object_add(video, "author_name", json_object_new_string(author_name));
      json_object_object_add(video, "title", json_object_new_string(title));
      json_object_object_add(video, "description", json_object_new_string(description));
      json_object_object_add(video, "privacy", json_object_new_string(privacy));
      json_object_object_add(video, "filename", json_object_new_string(filename));
      json_object_object_add(video, "content_type", json_object_new_string(content_type));
      json_object_object_add(video, "byte_size", json_object_new_string(byte_size));
      json_object_object_add(video, "create_at", json_object_new_string(create_at));

      json_object_array_add(videos_list, video);
    }
  }

  json_object_object_add(fetch_all_videos_has_login_response, "head", json_object_new_string("fetch_all_videos_has_login_response"));

  struct json_object *body_object = json_object_new_object();

  json_object_object_add(body_object, "videos_list", videos_list);

  json_object_object_add(fetch_all_videos_has_login_response, "body", body_object);

  mysql_free_result(res);
  mysql_close(conn);

  return json_object_to_json_string(fetch_all_videos_has_login_response);
}
