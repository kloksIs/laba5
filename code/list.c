#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

list_type *initialization_void_list(){
    list_type *list = (list_type *) malloc(sizeof(list_type));
    if (!list) {
        printf("Ошибка выделения памяти\n");
        return NULL;
    }
    Node *head = (Node*) malloc(sizeof(Node));
        if (!head) {
            printf("Ошибка выделения памяти для узла\n");
            return NULL;
        }
    list->size = 1;
    list->head = head;
    list->tail = head;
    
    list->tail->prev = NULL;
    list->size--;
    free(list->tail);
    return list;
}

list_type *initialization_ready_list(data_type *array, int size) {
    //Проверка корректности вводимых данных
    if (!array){
        printf("Передан пустой массив\n");
        return NULL;
    }
    if (size <= 0) {
        printf("Размер массива равен или меньше 0\n");
        return NULL;;
    }
    

    //Выделяем память под массив указателей
    Node **node_array = (Node**) malloc(sizeof(Node*) * size);
    if (!node_array){
        printf("Ошибка выделения памяти\n");
        return NULL;
    }
    
    //Создаём узлы, со значениями
    for (int i = 0; i < size; i++) { 
        node_array[i] = (Node*) malloc(sizeof(Node));
        if (!node_array[i]) {
            printf("Ошибка выделения памяти для узла %d\n", i);
            return NULL;
        }
        node_array[i]->value = array[i];
        node_array[i]->prev = NULL;
        node_array[i]->next = NULL;
    }

    //Определяем связи между узлами
    for (int i = 0; i < size; i++) {
        if (i > 0) {
            node_array[i]->prev = node_array[i-1];
        }
        if (i < size - 1) {
            node_array[i]->next = node_array[i+1];
        }
    }
    
    //Инициализируем список и заполняем его
    list_type *list = (list_type *) malloc(sizeof(list_type));
    if (!list) {
        printf("Ошибка выделения памяти\n");
        return NULL;
    }
    list->size = size;
    list->head = node_array[0];
    list->tail = node_array[size-1];
    free(node_array);

    return list;
}

void free_list(list_type *list){
    if (list == NULL) {
        printf("Ошибка: Список не инициализирован.\n");
        return;
    }
    Node *now_node = list->head;
    Node *next_node;
    while (now_node != NULL) {
        next_node = now_node->next;
        free(now_node);
        now_node = next_node;   
    }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    
}

void append(list_type *list, data_type value){
    //Инициализация нового узла, запись данных
    Node *new_node = (Node *) malloc(sizeof(Node)); 
    if (!new_node){
        printf("Ошибка выделения памяти\n");
            exit(1);
    }
    new_node->value = value;

    //Присоединения нового узла в конец
    list->tail->next = new_node;
    new_node->prev = list->tail;
    new_node->next = NULL;
    list->tail = new_node;
    list->size++;
}

void del_last_el(list_type *list) {
    if (list->size < 2){
        printf("Нельзя удалить конец, т.к. это начало списка\n");
        return; //Если не работает вот последние изменение
    }
    
    list->tail = list->tail->prev;
    free(list->tail->next);
    list->tail->next = NULL;

    list->size--;
}

static int_or_str *find_value_byAttr(int_or_str *value_attr, int num_atter, Node *node){
    switch (num_atter) {
    case ID_Num:
        value_attr->num = node->value.id;
        break;
    case Serial_Number_Num:
        strcpy(value_attr->str, node->value.serial_number);
        break;
    case Status_Num:
        strcpy(value_attr->str, node->value.status);
        break;
    case Location_Num:
        strcpy(value_attr->str, node->value.location);
        break;
    case Battery_Num:
        value_attr->num = node->value.battery;
        break;
    case Condition_Num:
        strcpy(value_attr->str, node->value.condition);
        break;
    default:
        printf("Передан неверный номер аттребута\n");
        return NULL;
    }
    return value_attr;
}

Node *find_node_byValue(list_type *list, int_or_str value, int num_atter){
    Node *node = list->head;
    int_or_str value_attr;
    if (num_atter == ID_Num || num_atter == Battery_Num) {
        while (node != NULL) {
            if (!find_value_byAttr(&value_attr, num_atter, node)) {
                printf("Ошибка поиска по значению\n");
                return NULL;
            }

            if (value_attr.num != value.num) {
                node = node->next;
            } else break;
        }
    } else if (ID_Num <= num_atter && num_atter <= Condition_Num){
        while (node != NULL) {
            if (!find_value_byAttr(&value_attr, num_atter, node)) {
                printf("Ошибка поиска по значению\n");
                return NULL;
            }

            if (strcmp(value_attr.str, value.str) != 0) { //0 - если равны
                node = node->next;
            } else break;
        }
    } else {
        printf("Передан неверный номер аттребута\n");
        return NULL;
    }

    return node;
}

void remove_el_byValue(list_type *list, int_or_str value, tupe_attr num_atter){
    Node *node_remove = find_node_byValue(list, value, num_atter);
    if (!node_remove){
        //
        printf("Элемент для удаления не найден\n");
    } else {
        //Меняем узлы, отчищаем
        if (node_remove->prev != NULL){
            node_remove->prev->next = node_remove->next;
        } else list->head = node_remove->next;

        if (node_remove->next != NULL){
            node_remove->next->prev = node_remove->prev;
        } else list->tail = node_remove->prev;

        list->size--;
        free(node_remove);
    }
}

data_type *get_array_value(list_type *list){
    data_type *array = (data_type *) malloc(sizeof(data_type)* list->size);
    if (!array){
        printf("Ошибка выделения памяти\n");
            exit(1);
    }
    Node *node_now = list->head;

    for (int i = 0; i < list->size; i++) {
        array[i] = node_now->value;
        node_now = node_now->next;
    }
    
    return array;
}

list_type *find_All_el_byAttr(list_type *list, int num_atter, int_or_str value){
    list_type *list_find_el = initialization_void_list();
    Node *node = list->head;
    int_or_str value_attr_now;
    if (num_atter == ID_Num || num_atter == Battery_Num) {
        while (node != NULL) {
            if (!find_value_byAttr(&value_attr_now, num_atter, node)) {
                printf("Ошибка поиска по значению\n");
                return NULL;
            }

            if (value_attr_now.num == value.num) {
                append(list_find_el, node->value);
            }
            node = node->next;
        }
    } else if (ID_Num <= num_atter && num_atter <= Condition_Num){
        while (node != NULL) {
            if (!find_value_byAttr(&value_attr_now, num_atter, node)) {
                printf("Ошибка поиска по значению\n");
                return NULL;
            }

            if (strcmp(value_attr_now.str, value.str) == 0) { //0 - если равны
                append(list_find_el, node->value);
            }
            node = node->next;
        }
    } else {
        printf("Передан неверный номер аттребута\n");
        return NULL;
    }


    return list_find_el;
}

list_type *get_clone_list(list_type *list){
    list_type *clone_list = initialization_void_list();
    Node *node_now = list->head;
    while (node_now != NULL) {
        append(clone_list, node_now->value);
        node_now = node_now->next;
    }
    clone_list->head->prev = NULL;
    return clone_list;
}

void append_id_in_list(list_type *list){
    Node *node = list->head;
    for (int i = 0; i < list->size; i++){
        node->value.id = i + 1;
        node = node->next;
    }
}