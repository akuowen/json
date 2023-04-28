//
// Created by akuowen on 23-4-23.
//
#include "json.h"
#include "malloc.h"
#include "string.h"
#include "stdlib.h"

static void *(*json_malloc)(size_t sz) = malloc;

char *json_parse(const char *json_str) {
    json *json_item = create_json();
    do_json_parse(json_item, json_str);
    return NULL;
};

char *do_json_parse(json *json_item, const char *json_str) {
    if (!strncmp(json_str, "false", 5)) {
        json_item->value_type = bool_type;
        json_item->int_value = 0;
        json_str += 5;
        return json_str;

    }

    if (!strncmp(json_str, "true", 4)) {
        json_item->value_type = bool_type;
        json_item->int_value = 1;
        json_str += 4;
        return json_str;

    }

    if (!strncmp(json_str, "null", 4)) {
        json_item->value_type = null_type;
        json_str += 4;
        return json_str;
    }
    //字符串
    if ((*json_str) == '\"') {
        return json_parse_str(json_item, json_str);

    }
    // 数字
    if (((*json_str) == '-') || (*json_str >= '0' && *json_str <= '9')) {
        return json_parse_num(json_item, json_str);
    }
    //实体
    if ((*json_str) == '{') {
        return json_parse_obj(json_item, json_str);
    }
    //数组
    //实体
    if ((*json_str) == '[') {
        return json_parse_array(json_item, json_str);
    }

    if (*json_str == ',') {
        return do_json_parse(json_item, json_str);
    }
    return json_str;
};

char *json_to_str(json *json_obj) {};

void delete_json(json *json_obj) {};

json *create_json() {
    json_malloc(sizeof(json));
};

const char *skip(const char *json_str) {
    while (json_str && *json_malloc && ((unsigned char) *json_str <= 32)) {
        json_str++;
    }
    return json_str;
};

json *create_json_str() {};

json *create_json_number(double num) {};

json *create_json_array() {};

json *create_json_boolean(int type) {};

json *create_json_boolean_true() {};

json *create_json_boolean_false() {};

json *create_json_null() {};

json *create_json_obj() {};


const char *json_parse_str(json *json_item, const char *json_str) {
    if (*json_str != '\"') {
        return NULL;
    }
    const char *str_ptr = json_str + 1;
    //字符串长度
    int str_len = 0;
    char *out;

    while (*str_ptr != '\"' && *str_ptr) {
        str_ptr++;
        if (*str_ptr != '\\') { str_len++; }
    }
    //申请空间  拷贝字符
    out = json_malloc(str_len + 1);
    if (!out) return NULL;
    str_ptr = json_str + 1;
    for (int i = 0; i < str_len; i++) {
        if (*str_ptr != '\\') {
            *(out + i) = *str_ptr;
            str_ptr++;
        } else {
            str_ptr++;
        }
    };
    *(out + str_len) = '\0';
    json_item->value_type = str_type;
    json_item->str_value = out;
    return json_str + str_len + 2;
};

const char *json_parse_num(json *json_item, const char *json_str) {

    unsigned char *after_end = NULL;

    const char *num_str = json_str;

    int length = 0;
    int index = 0;
    while ((*(num_str + index) == '-' || (*(num_str + index) >= '0' && *(num_str + index) <= '9') ||
            (*(num_str + index) == 'e' || *(num_str + index) == 'E'))) {
        length++;
        index++;
    }
    char *num_str_result = malloc(length + 1);
    for (int i = 0; i < length; ++i) {
        *(num_str_result + i) = *(num_str + i);
    }

    *(num_str_result + length + 1) = '\0';
    double double_value = strtod(num_str, (char **) &after_end);
    printf("转换完毕的的double_value为 %f\n", double_value);

    json_item->value_type = num_type;
    json_item->double_Value = double_value;
    json_item->int_value =
            double_value >= INT_MAX ? INT_MAX : double_value <= (double) INT_MIN ? INT_MIN : (int) double_value;
    return json_str + length ;

};

const char *json_parse_obj(json *json_item, const char *json_str) {
    json *child;
    if (*json_str != '{') {
        return NULL;
    }
    json_item->value_type = object_type;
    json_str = skip(json_str + 1);
    if (*json_str == '}')return json_str + 1;
    json_item->child = child = create_json();
    json_str = json_parse_str(child, skip(json_str));
    child->name = child->str_value;
    child->str_value = NULL;
    if (*json_str != ':') {
        return NULL;
    }
    json_str = skip(do_json_parse(child, skip(json_str + 1)));

    if (json_str == NULL) { return NULL; }
//    json_str = skip(do_json_parse(child, skip(json_str)));
//    if (*json_str == ',') {
//        json *new_json_item = create_json();
//        child->next = new_json_item;
//        new_json_item->pre = child;
//        child = new_json_item;
//        json_str = skip(json_parse_str(child, skip(json_str + 1)));
//        if (!json_str) return NULL;
//        child->name = child->str_value;
//        child->str_value = NULL;
//        if (*json_str != ':') {
//            return NULL;
//        }
//        json_str = skip(do_json_parse(child, skip(json_str + 1)));
//        if (!json_str) return NULL;
    while (*json_str == ',') {
        json *new_json_item = create_json();
        child->next = new_json_item;
        new_json_item->pre = child;
        child = new_json_item;
        json_str = skip(json_parse_str(child, skip(json_str + 1)));
        if (!json_str) return NULL;
        child->name = child->str_value;
        child->str_value = NULL;
        if (*json_str != ':') {
            return NULL;
        }
        json_str = skip(do_json_parse(child, skip(json_str + 1)));
        if (!json_str) return NULL;

    }
//    }
    if (*json_str == '}') return json_str + 1;
};

const char *json_parse_array(json *json_item, const char *json_str) {
    if (*json_str != '[') {
        return NULL;
    }
    json_item->value_type = array_type;
    json_str = skip(json_str);
    if (*json_str == ']') return json_str + 1;
    json *child = create_json();
    json_item->child = (struct json *) child;
    json_str = skip(do_json_parse(child, skip(json_str + 1)));
    if (!json_str) return NULL;
    while (*json_str == ',') {
        json *new_json_item = create_json();
        child->next = (struct json *) new_json_item;
        new_json_item->pre = (struct json *) child;
        child = new_json_item;
//        json_item->child = (struct json *) child;
        json_str = skip(do_json_parse(child, skip(json_str + 1)));
        if (!json_str) return NULL;
    }
    if (*json_str == ']') return (json_str + 1);
};
