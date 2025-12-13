#ifndef LIST_H
#define LIST_H

#include "type_and_struct.h"


list_type *initialization_void_list();

list_type *initialization_ready_list(data_type *array, int size);

void free_list(list_type *list);

void append(list_type *list, data_type value);

void del_last_el(list_type *list);

Node *find_node_byValue(list_type *list, int_or_str value, int num_atter);

void remove_el_byValue(list_type *list, int_or_str value, tupe_attr num_atter);

data_type *get_array_value(list_type *list);

list_type *find_All_el_byAttr(list_type *list, int num_atter, int_or_str value);

list_type *get_clone_list(list_type *list);

void append_id_in_list(list_type *list);

#endif