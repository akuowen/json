//
// Created by akuowen on 23-4-23.
//
#include "json.h"
#include "malloc.h"
#include "string.h"
#include "stdlib.h"

static void *(*json_malloc)(size_t sz) = malloc;

static void (*json_free)(void *prt) = free;

const char *json_parse(const char *json_str) {
    json *json_item = create_json();
    do_json_parse(json_item, json_str);
    return NULL;
};

const char *do_json_parse(json *json_item, const char *json_str) {
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

char *json_to_str(json *json_obj) {
    return do_json_to_str(json_obj, 0, 1);
};

char *do_json_to_str(json *json_obj, int deep, int fmt) {
    char *out;
    if (!json_obj) { return NULL; }
    switch (json_obj->value_type) {
        case null_type:
            break;
        case object_type:
            break;
        case array_type:
            break;
        case str_type:
            break;
        case bool_type:
            break;
        default:
            break;
    }
};

/**
 * 字符串转json
 * @param json_item
 * @return
 */
char *str_to_json(json *json_item, int is_num) {
    char *str = json_item->str_value;
    if (is_num) {
        char *name = json_item->name;
        unsigned long len = strlen(name);
        char *result = json_malloc(strlen(name) + 4);
        *result = '\"';
        strcpy(result + 1, name);
        *(result + len + 1) = '\"';
        *(result + len + 2) = ':';
        return result;
    }
    char *str_json;
    if (!str) {
        str_json = json_malloc(3);
        strcpy(str_json, "\"\"");
        return str_json;
    }
    unsigned long length = strlen(str);
    str_json = json_malloc(length + 3);
    *str_json = '\"';
    strcpy(str_json + 1, str);
    str_json[length + 1] = '\"';
    str_json[length + 2] = '\0';
    return str_json;
};

char *num_to_json(json *json_item) {};

char *obj_to_json(json *json_item) {};

char *array_to_json(json *json_item, int deep, int fmt) {
    //子结点的指针
    char **entities = NULL;
    char *out = NULL;
    //  [ ]  \0
    unsigned long length = 3, node_num = 0, i = 0;
    json *json_child = (json *) json_item->child;

    //广度优先  看他有几个子节点
    while (json_child) {
        json_child = (json *) json_child->next;
        node_num++;
    }
    //没有子节点
    if (!node_num) {
        out = json_malloc(length);
        strcpy(out, "[]");
        return out;
    }
    //申请空间
    entities = json_malloc(sizeof(char **) * node_num);
    //更换到头部
    json_child = (json *) json_item->child;

    while (json_child) {
        char *result = str_to_json(json_child, json_child->name ? 1 : 0);
        if (result) {
            entities[i++] = result;
            if (json_child->next) {
                length += strlen(result) + 1;
            } else {
                length += strlen(result);
            }
        }
        json_child = (json *) json_child->next;

    }

    out = json_malloc(length + 1);
    unsigned long pre_len = 0;
    *out = '[';
    for (int i = 0; i < node_num; ++i) {
        char *word = entities[i];
        unsigned long len = strlen(word);

        strcpy(out + 1 + pre_len, word);
        pre_len += len + 1;
        if (i != node_num - 1) {
            *(out + pre_len) = ',';
        }
    }
    *(out + length - 2) = ']';
    *(out + length - 1) = '\0';
    for (int i = 0; i < node_num; ++i) {
        json_free(entities[i]);
    };
    return out;

};

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
    return json_str + length;

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
