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
#include <gst/gst.h>
#include <gst/rtsp-server/rtsp-server.h>
#include <pthread.h>

#define PORT 1472
#define MAX_BUFFER 4096

const char *server = "localhost";
const char *user = "root";
const char *password = "root";
const char *database = "youtok";

const char *get_request_head(const char *request);
const char *create_login_response(const char *request);
const char *create_watch_video_response(const char *request);
const char *get_request_body(const char *request);
const char *get_user_json_string(const char *username, const char *user_password);
const char *get_filename_string(const char *body);
const char *create_fetch_all_videos_response();

guint exit_timeout_id = 0;

static gboolean
timeout(GstRTSPServer *server)
{
  GstRTSPSessionPool *pool;

  pool = gst_rtsp_server_get_session_pool(server);
  gst_rtsp_session_pool_cleanup(pool);
  g_object_unref(pool);

  return TRUE;
}

static GstRTSPFilterResult
client_filter(GstRTSPServer *server, GstRTSPClient *client,
              gpointer user_data)
{
  /* Simple filter that shuts down all clients. */
  return GST_RTSP_FILTER_REMOVE;
}

/* Timeout that runs 10 seconds after the first client connects and triggers
 * the shutdown of the server */
static gboolean
shutdown_timeout(GstRTSPServer *server)
{
  GstRTSPMountPoints *mounts;
  g_print("Time for everyone to go. Removing mount point\n");
  /* Remove the mount point to prevent new clients connecting */
  mounts = gst_rtsp_server_get_mount_points(server);
  gst_rtsp_mount_points_remove_factory(mounts, "/test");
  g_object_unref(mounts);

  /* Filter existing clients and remove them */
  g_print("Disconnecting existing clients\n");
  gst_rtsp_server_client_filter(server, client_filter, NULL);
  return FALSE;
}

static void
client_connected(GstRTSPServer *server, GstRTSPClient *client)
{
  if (exit_timeout_id == 0)
  {
    g_print("First Client connected. Disconnecting everyone in 10 seconds\n");
    exit_timeout_id =
        g_timeout_add_seconds(10, (GSourceFunc)shutdown_timeout, server);
  }
}

static gboolean
remove_sessions(GstRTSPServer *server)
{
  GstRTSPSessionPool *pool;

  pool = gst_rtsp_server_get_session_pool(server);
  guint removed = gst_rtsp_session_pool_cleanup(pool);
  g_object_unref(pool);
  g_print("Removed %d sessions\n", removed);

  return TRUE;
}

int start_stream(const char *filename)
{
  GMainLoop *loop;
  GstRTSPServer *server;
  GstRTSPMountPoints *mounts;
  GstRTSPMediaFactory *factory;
  char source[250];
  strcpy(source, "( filesrc location=\\");
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

  // g_signal_connect(server, "client-connected", (GCallback)client_connected, NULL);

  /* add a timeout for the session cleanup */
  g_timeout_add_seconds(2, (GSourceFunc)timeout, server);
  g_print("stream ready at rtsp://127.0.0.1:8554%s\n", mountPointStr);
  g_main_loop_run(loop);

  return 0;

/* ERRORS */
failed:
{
  g_print("failed to attach the server\n");
  return -1;
}
}

void *connection_handler(void *client_socket) {
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
  else if (strcmp(request_head, "fetch_all_videos") == 0)
  {
    response = create_fetch_all_videos_response();
    write(connfd, response, strlen(response));
  }
  else if (strcmp(request_head, "watch_video") == 0)
  {
    response = create_watch_video_response(request_body);
    write(connfd, response, strlen(response));
    start_stream(get_filename_string(request_body));
  }
  printf("%ld\n", strlen(response));

  printf("Response: %s\n", response);
}

int main(int argc, char *argv[])
{
  int sockfd, connfd;
  struct sockaddr_in servaddr, cliaddr;

  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    perror("socket creation failed\n");
    exit(0);
  }

  bzero(&cliaddr, sizeof(cliaddr));
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = INADDR_ANY;
  servaddr.sin_port = htons(PORT);

  if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1)
  {
    perror("bind failed");
    exit(0);
  }

  int len, n;

  len = sizeof(cliaddr);

  int no_threads = 0;
  pthread_t threads[50];

  for (;;)
  {
    if ((listen(sockfd, 50)) != 0)
    {
      printf("Listen failed...\n");
      exit(0);
    }
    else
      printf("Server listening..\n");
    len = sizeof(cliaddr);

    connfd = accept(sockfd, (struct sockaddr *)&cliaddr, &len);
    if (pthread_create(&threads[no_threads], NULL, connection_handler, &connfd) < 0)
    {
      perror("Could not create thread");
      return 1;
    }
    if (connfd < 0)
    {
      printf("server accept failed...\n");
      exit(0);
    }
    else
      printf("server accept the client...\n");
    puts("Handler assigned");
    no_threads++;
  }
  int k = 0;
  for (k = 0; k < 50; k++)
  {
    pthread_join(threads[k], NULL);
  }
  close(sockfd);
  return 0;
}


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

const char *get_filename_string(const char *body)
{
  struct json_object *parsed_body_json;
  struct json_object *filename;

  parsed_body_json = json_tokener_parse(body);

  json_object_object_get_ex(parsed_body_json, "filename", &filename);
  printf("%s\n", json_object_to_json_string(filename));

  return json_object_get_string(filename);
}

const char *create_watch_video_response(const char *body)
{
  struct json_object *parsed_body_json;
  struct json_object *filename;

  parsed_body_json = json_tokener_parse(body);

  json_object_object_get_ex(parsed_body_json, "filename", &filename);

  struct json_object *response_json = json_object_new_object();
  json_object_object_add(response_json, "head", json_object_new_string("watch_video_response"));

  struct json_object *response_body_json = json_object_new_object();
  json_object_object_add(response_body_json, "filename", filename);
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

const char *create_fetch_all_videos_response()
{
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
    return NULL;
  }

  json_object_object_add(fetch_all_videos_response, "head", json_object_new_string("fetch_all_videos_response"));

  struct json_object *body_object = json_object_new_object();

  json_object_object_add(body_object, "videos_list", videos_list);

  json_object_object_add(fetch_all_videos_response, "body", body_object);

  mysql_free_result(res);
  mysql_close(conn);

  return json_object_to_json_string(fetch_all_videos_response);
}
