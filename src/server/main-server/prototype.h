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
#include <time.h>

#define PORT 1472
#define MAX_BUFFER 4096

#define SERVER "localhost"
#define USER "root"
#define PASSWORD "root"
#define DATABASE "youtok"

const char *get_request_head(const char *request);
const char *create_login_response(const char *request);
const char *create_watch_video_response(const char *request, int streamThreadNo);
const char *get_request_body(const char *request);
const char *get_user_json_string(const char *username, const char *user_password);
const char *get_filename_string(const char *body);
const char *create_fetch_all_videos_no_login_response();

const char *get_current_user_id(const char *request_body);
const char *create_fetch_all_videos_has_login_response(const char *user_id);

int get_running_thread_no_to_kill(const char *request);

const char *rand_text(const char *file);
void handle_upload_new_video(int connfd, const char *request_body);
void receive_image(int sockfd, const char *filename);
const char *create_upload_new_video_response(
  const char *user_id,
  const char *title,
  const char *description,
  const char *privacy,
  const char *filename,
  const char *byte_size
);

const char *get_search_key(const char *request_body);
const char *create_search_video_no_login_response(const char *search_key);
const char *create_search_video_has_login_response(const char *user_id, const char *search_key);

const char *create_my_video_response(const char *user_id);

void *handle_request(void *client_socket);

void *update_privacy(const char *video_id, const char *privacy);
const char *get_privacy(const char *request_body);
const char *get_video_id(const char *request_body);
