#ifndef LIST_H
#define LIST_H

typedef struct Node {
    int value;
    struct Node *prev;
    struct Node *next;
} Node;

typedef struct {
    int size;
    Node *head;
    Node *tail; //указатель на конец
} list_type;

list_type *initialization_list(int *array, int size);

void append(list_type *list, int a);

void del_last_el(list_type *list);

Node *find_node_byValue(list_type *list, int value);

void remove_el_byValue(list_type *list, int value);

int *get_array_value(list_type *list);

#endif