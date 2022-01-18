#include <stdio.h>
#include <json-c/json.h>

int main() {
  char *buffer = "{'head':'login','body':{'password':'123123','username':'admin'}}";

  struct json_object *parsed_json;
  struct json_object *head;
  struct json_object *body;
  struct json_object *username;
  struct json_object *password;

  parsed_json = json_tokener_parse(buffer);

  json_object_object_get_ex(parsed_json, "head", &head);
  json_object_object_get_ex(parsed_json, "body", &body);
  json_object_object_get_ex(parsed_json, "body", &body);

  const char *request_type = json_object_get_string(head);

  printf("Head: %s\n", request_type);

}
