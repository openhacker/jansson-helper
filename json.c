/*

This is a set of json helper utilities I found useful -- which goes with the janssen library
Marty Leisner    maleisner@gmail.com


This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

For more information, please refer to <http://unlicense.org>
*/

#include <stdio.h>
#include <stdbool.h>
#include "jansson.h"
#include "json.h"

#define log_message printf
#define log_file stderr

static void print_json_array(json_t *json)
{
	int size;
	json_t *element;
	int i;

	size = json_array_size(json);
	// fprintf(log_file, "array of size %d\n", size);
	for(i = 0; i < size; i++)  {
		element = json_array_get(json, i);
		print_json(element);
	}
}


void print_json_and_label(const char *label, json_t *json)
{
	log_message(label);
	print_json(json);
}

void print_json(json_t *json)
{
	/* obj is a JSON object */
	const char *key;
	json_t *value;
	
	void *iter = json_object_iter(json);

	while(iter) {
	    key = json_object_iter_key(iter);
	    value = json_object_iter_value(iter);
	    fprintf(log_file, "key = %s:", key);
	    switch(json_typeof(value)) {
		case JSON_OBJECT:
			fprintf(log_file, "\n");
			print_json(value);
			break;
		case JSON_ARRAY:
			print_json_array(value);
			break;
		case JSON_STRING:
			fprintf(log_file, "\"%s\"", json_string_value(value));
			break;			
		case JSON_INTEGER:
			fprintf(log_file, "%" JSON_INTEGER_FORMAT, json_integer_value(value));
			break;
		case JSON_TRUE:
			fprintf(log_file, "true");
			break;
		case JSON_FALSE:
			fprintf(log_file, "false");
			break;	
		case JSON_NULL:
			fprintf(log_file, "null");
			break;
		case JSON_REAL:
			fprintf(log_file, "%f", json_real_value(value));
			break;
		default:
			fprintf(log_file, "strange!!");
			break;
	   }
	   fprintf(log_file, "\n");

	   /* use key and value ... */
	   iter = json_object_iter_next(json, iter);
	}
	
}


const char *json_get_string(json_t *object, const char *name)
{
	json_t *string_obj;

	string_obj = json_object_get(object, name);
	if(!string_obj)
		return NULL;
	if(json_typeof(string_obj) != JSON_STRING)
		return NULL;
	return json_string_value(string_obj);
}

_Bool json_get_integer(json_t *object, const char *name, int *value) 
{
	json_t *int_obj;
	
	int_obj = json_object_get(object, name);
	if(!int_obj)
		return false;

	if(!json_is_integer(int_obj))
		return false;

	*value = json_integer_value(int_obj);
	return true;	
}

int json_replace_integer(json_t *object, const char *name, int new_value)
{
	json_t *json_int;
	int result;

	json_int = json_integer(new_value);
	if(!json_int) {
		log_message("json_dd_integer failed for %s: json_inter(%d)", name, new_value);
		return -1;
	}
	
	result = json_object_set(object, name, json_int);
	if(result != 0) {
		log_message("json_object_set failed for (%s,%d)", name, new_value);
	}
	return result;
}


int json_add_double(json_t *object, const char *name, double value)
{

	json_t *json_double;
	int result;

	json_double = json_real(value);
	if(!json_double) {
		log_message("json_real: json_add_double failed: %s %f", name, value);
		return -1;
	}


	result = json_object_set_new(object, name, json_double);
	if(result  != 0) {
		log_message("json_object_set_new failed for (%s,%f)", name, value);
	}
	return result;
	
}

int json_add_integer(json_t *object, const char *name, json_int_t value)
{
	json_t *json_int;
	int result;

	json_int = json_integer(value);
	if(!json_int) {
		log_message("json_dd_integer failed for %s: json_inter(%ld)", name, value);
		return -1;
	}


	result = json_object_set_new(object, name, json_int);
	if(result  != 0) {
		log_message("json_object_set_new failed for (%s,%ld)", name, value);
	}
	return result;
	
}

int json_add_string(json_t *object, const char *name, const char *value)
{
	json_t *json_string_object;
	int result;


	json_string_object = json_string(value);
	if(!json_string_object) {
		log_message("json_add_string failed, (%s,%s)", name, value);
	}

	result = json_object_set_new(object, name, json_string_object);

	return result;
}

int json_add_boolean(json_t *object, const char *name, _Bool value)
{
	json_t *json_bool_object;
	int result;

	if(true == value)
		json_bool_object = json_true();
	else	json_bool_object = json_false();
	
	if(!json_bool_object) {
		log_message("%s failed", __func__);
		return -1;
	}

	result = json_object_set_new(object, name, json_bool_object);
	return result;

}

/* return true if it works, false if not */
_Bool json_invert_boolean(json_t *object, const char *name)
{
	json_t *current;
	json_t *new_object;
	int result;

	current = json_object_get(object, name);
	if(!current) {
		log_message("cannot invert boolean: %s", name);
		print_json(object);
		return false;
	}

	if(json_is_true(current)) {
		new_object = json_false();
	} else if(json_is_false(current)) {
		new_object = json_true();
	} else {
		log_message("%s not true or false", name);
		return false;
	}
	result = json_object_set(object, name, new_object);
	if(result == -1) {
		log_message("cannot do json_object_set for %s", name);
		return false;
	} else return true;
}

