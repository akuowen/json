#include <stdio.h>
#include "stdlib.h"
#include "malloc.h"
#include "json.h"

void print_json_node(json *json_node) {
    if (!json_node) { return; }
    enum type node_type = json_node->value_type;
    json *json_node_next = (json *) json_node->next;
    printf("当前节点==当前节点name:%s  类型:%u, 节点str:%s,int_value:%d double_value:%f \n", json_node->name, node_type,
           json_node->str_value, json_node->int_value, json_node->double_Value);
    if (json_node_next) {
        printf("它的平行节点\n");
        print_json_node(json_node_next);

    }
    json *json_node_child = (json *) json_node->child;
    if (json_node_child) {
        printf("它的子节点\n");
        print_json_node(json_node_child);
    }


}

void test_array() {
    char str[] = "[\"one\",\"two\",\"three\",\"four\"]";
    json *json_node = create_json();
    do_json_parse(json_node, str);
    print_json_node(json_node);
}

void test_obj_1() {
    char str[] = "{\"a\":\"a\",\"b\":true,\"c\":1}";
    json *json_node = create_json();
    do_json_parse(json_node, str);
    print_json_node(json_node);
}

void test_obj_2() {
    char str[] = "{\n"
                 "    \"a\":\"a\",\n"
                 "    \"b\":true,\n"
                 "    \"c\":1,\n"
                 "    \"d\":{\n"
                 "        \"a\":\"a\",\n"
                 "        \"b\":true,\n"
                 "        \"c\":1\n"
                 "    },\n"
                 "    \"e\":[\n"
                 "        1,\n"
                 "        2,\n"
                 "        3,\n"
                 "        4,\n"
                 "        5,\n"
                 "        6,\n"
                 "        \"7\"\n"
                 "    ]\n"
                 "}";
    json *json_node = create_json();
    do_json_parse(json_node, str);
    print_json_node(json_node);
}

int main() {
    test_obj_2();
    return 0;
}

