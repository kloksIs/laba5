#include <stdio.h>
#include <stdlib.h>
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
    del_last_el(list);
    return list;
}

list_type *initialization_ready_list(int *array, int size) {
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

void append(list_type *list, int value){
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

void del_last_el(list_type *list){
    list->tail->prev = NULL;
    list->size--;
    free(list->tail);
}

Node *find_node_byValue(list_type *list, int value){
    Node *node = list->head;
    while (node != NULL && node->value != value) {
        node = node->next;
    }
    
    return node;
}

void remove_el_byValue(list_type *list, int value){
    Node *node_remove = find_node_byValue(list, value);
    if (!node_remove){
        //
        printf("Элемент для удаления не найден\n");
        //return
    }

    //Меняем узлы, отчищаем
    node_remove->prev->next = node_remove->next;
    node_remove->next->prev = node_remove->prev;
    free(node_remove);
}

int *get_array_value(list_type *list){
    int *array = (int *) malloc(sizeof(int)* list->size);
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