//
// Created by akuowen on 23-4-23.
//

#ifndef JSON_JSON_H
#define JSON_JSON_H
#define INT_MAX __INT_MAX__
#define INT_MIN (-INT_MAX - 1)
enum type {
    num_type = 0,
    object_type,
    array_type,
    str_type,
    bool_type,
    null_type
};
typedef struct {
    struct json *pre;
    struct json *next;
    struct json *child;
    enum type value_type;
    char *name;
    char *str_value;
    int int_value;
    double double_Value;

} json;

/**
 * 字符串转json对象主入口
 * @param json_str
 * @return
 */
char *json_parse(const char *json_str);
char *do_json_parse(json *json_item,const char *json_str);

/**
 * 实体转字符串
 * @param json_obj
 * @return
 */
char *json_to_str(json *json_obj);

void delete_json(json *json_obj);

json *create_json();

const char *skip(const char *json_str);

json *create_json_str();

json *create_json_number(double num);

json *create_json_array();

json *create_json_boolean(int type);

json *create_json_boolean_true();

json *create_json_boolean_false();

json *create_json_null();

json *create_json_obj();

const char* json_parse_str(json *json_item,const char *json_str);








const char* json_parse_num(json *json_item,const char *json_str);
const char* json_parse_obj(json *json_item,const char *json_str);
const char* json_parse_array(json *json_item,const char *json_str);

#endif //JSON_JSON_H
