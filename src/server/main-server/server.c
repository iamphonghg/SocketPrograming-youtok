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

#define PORT 1472
#define MAX 4096

const char *server = "localhost";
const char *user = "root";
const char *password = "";
const char *database = "youtok";

const char *get_request_head(const char *request);
const char *create_login_response(const char *request);
const char *get_request_body(const char *request);
const char *get_user_json_string(const char *username, const char *user_password);

int main(int argc, char *argv[])
{
  int sockfd, connfd;
  struct sockaddr_in servaddr, cliaddr;
  char buffer[MAX];

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

  len = sizeof(cliaddr); //len is value/resuslt

  for (;;)
  {
    if ((listen(sockfd, 5)) != 0)
    {
      printf("Listen failed...\n");
      exit(0);
    }
    else
      printf("Server listening..\n");
    len = sizeof(cliaddr);

    connfd = accept(sockfd, (struct sockaddr *)&cliaddr, &len);
    if (connfd < 0)
    {
      printf("server accept failed...\n");
      exit(0);
    }
    else
      printf("server accept the client...\n");

    bzero(buffer, MAX);
    read(connfd, buffer, sizeof(buffer));

    printf("Client : %s\n", buffer);
    const char *request_head = get_request_head(buffer);
    const char *request_body = get_request_body(buffer);
    const char *response = NULL;
    if (strcmp(request_head, "login") == 0)
    {
      response = create_login_response(request_body);
    }
    else if (strcmp(request_head, "search") == 0)
    {
    }
    printf("%ld\n", strlen(response));

    write(connfd, response, strlen(response));

    printf("Response: %s\n", response);
  }

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
