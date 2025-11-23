#include <stdio.h>
#include <stdlib.h>
#include "list.h"

typedef struct Node {
    int value;
    struct Node *node_prev;
    struct Node *node_next;
} Node;

typedef struct {
    int size;;
    Node *head;
    Node *tail //указатель на конец
} list_type;

list_type initialization(int *array, int size) {
    //Проверка корректности вводимых данных
    if (!array){
        printf("Передан пустой массив\n");
        exit(1);
    }
    if (size <= 0) {
        printf("Размер массива равен или меньше 0\n");
        exit(1);
    }
    

    //Выделяем память под массив указателей
    Node **heads = (Node**) malloc(sizeof(Node*) * size);
    if (!heads){
        printf("Ошибка выделения памяти\n");
        exit(1);
    }
    
    //Создаём узлы, со значениями
    for (int i = 0; i < size; i++) { 
        heads[i] = (Node*) malloc(sizeof(Node));
        if (!heads[i]) {
            printf("Ошибка выделения памяти для узла %d\n", i);
            exit(1);
        }
        heads[i]->value = array[i];
        heads[i]->node_prev = NULL;
        heads[i]->node_next = NULL;
    }

    //Определяем связи между узлами
    for (int i = 0; i < size; i++) {
        if (i > 0) {
            heads[i]->node_prev = heads[i-1];
        }
        if (i < size - 1) {
            heads[i]->node_next = heads[i+1];
        }
    }
    
    //Инициализируем список и заполняем его
    list_type list;
    list.size = size;
    list.head = heads[0];
    list.tail = heads[size-1];
    free(heads);
    return list;
}