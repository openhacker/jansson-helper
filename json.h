#ifndef JSON_H
#define JSON_H
#include <stdbool.h>
#include "jansson.h"

void print_json(json_t *json);
void print_json_and_label(const char *label, json_t *json);
void show_json(char *buffer, int size, json_t *json);
int json_add_integer(json_t *object, const char *name, json_int_t value);
_Bool json_get_integer(json_t *object, const char *name, int *value);
int json_replace_integer(json_t *object, const char *name, int new_value);
int json_add_double(json_t *object, const char *name, double new_value);
int json_add_string(json_t *object, const char *name, const char *value);
int json_add_boolean(json_t *object, const char *name, _Bool value);
const char *json_get_string(json_t *object, const char *name);
json_t *json_construct_ipc(const char *command, int seq_num, _Bool response);
json_t *json_construct_ipc_with_id(const char *command, int id, _Bool response);

_Bool json_invert_boolean(json_t *object, const char *name);



#endif

