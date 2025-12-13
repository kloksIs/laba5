#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "type_and_struct.h"
#include "list.h"
#include "DBMS.h"

#define BUFFER_SIZE 150
#define NEED_ATTRS 6 


FILE *initialization_DB(char *name_file){
    FILE *file_ptr = fopen(name_file, "r");
    if (!file_ptr){
        printf("Ошибка открытия файла\n");
        return NULL;
    }
    
    //Проверяем точность структуры БД
    if (!validation_DB(file_ptr)){
        printf("Ошибка валидации БД\n");
        return NULL;
    }
    fseek(file_ptr, 0, SEEK_SET);
    return file_ptr;
}

bool validation_DB(FILE * file_ptr){
    //Проверка корректного считывания главных данных из первой строки
    char first_str[BUFFER_SIZE] = {0};
    if (!fgets(first_str, BUFFER_SIZE, file_ptr)){
        return false;
    }

    //БД для городских самокатов. Характеристик: %d Кол-во_самокатов: %d
    int num_attrs = 0, num_scooters = 0;
    short count = sscanf(first_str,"БД для городских самокатов. Характеристик: %d Кол-во_самокатов: %d", &num_attrs, &num_scooters);
    if (count != 2){
        printf("Данные строчке №1 считались не корректно\n");
        return false;
    } else if (num_attrs != NEED_ATTRS) {
        printf("Программа расчитывает на 6 характеристик, а их %d\n", num_attrs);
        return false;
    }

    //Проверка, что есть все элементы и у каждого все характеристики
    int id = 0, attrs = 0;
    char str[BUFFER_SIZE] = {0};
    int c;
    bool in_object;
    fgets(str, BUFFER_SIZE, file_ptr); //Пропускаем вторую строчку

    while (fgets(str, BUFFER_SIZE, file_ptr) != NULL) {
        int num_ch = 0;
        id++;
        attrs = 0;
        if ((c = str[num_ch]) == '\n') {
            printf("Пустая строка №%d\n", id + 2); //Т.к. счёт id со 2 строки
            return false;
        } else if (c < '0' || c > '9') {
            printf("Строка №%d начинается не с ID_Num\n", id + 2);
            return false;
        }
        else {
            in_object = true;
            num_ch++;
         }
        

        while ((c = str[num_ch]) != '\n') {
            if (c == ';'){
                if (!in_object){
                    printf("Два разделяющих символа подрят в строке №%d\n", id + 2);
                    return false;
                }
                in_object = false;
                attrs++;
            } else in_object = true;
            num_ch++;
        }
        if (attrs != num_attrs - 1 && in_object){
            printf("Передано не верное кол-во характеристик в строке №%d\n", id + 2);
            return false;
        }
    }
    if (id != num_scooters){
            printf("Фактическое число самокатов не равно указанному в стрке №1\n");
            return false;
        }
    
    return true;
}

list_type *get_data_DB(FILE * file_ptr){
    list_type *list_DB = initialization_void_list();
    data_type data;
    char str_buffer[BUFFER_SIZE] = {0};
    short count;
    int num_str = 0;

    fgets(str_buffer, BUFFER_SIZE, file_ptr); //Пропускаем первые 2 строчки
    fgets(str_buffer, BUFFER_SIZE, file_ptr);
    num_str = 2;

    //Считываем построчно БД, и добавляем в список
    while (fgets(str_buffer, BUFFER_SIZE, file_ptr) != NULL) {
        num_str++;
        count = sscanf(str_buffer, "%d;%[^;];%[^;];%[^;];%d;%[^\n]",
                                    &data.id, data.serial_number, data.status,
                                    data.location, &data.battery, data.condition);
        if (count != NEED_ATTRS){
            printf("В строке №%d прочитано только %d поля из %d\n", num_str, count, NEED_ATTRS);
            return NULL;
        }
        append(list_DB,data);
    }
    list_DB->head->prev = NULL;
    return list_DB;
}

int work_push_DB(list_type *list, char *name_file){
    FILE *file = fopen(name_file, "w");
    if (!file) {
        perror("Ошибка открытия файла для перезаписи");
        return EXIT_FAILURE;
    }

    Node *node = list->head;
    fprintf(file, "БД для городских самокатов. Характеристик: 6 Кол-во_самокатов: %d\n", list->size);
    fprintf(file, "ID_Num;Серийный_номер;Статус;Локация;Заряд_батареи(%%);Состояние");
    while (node != NULL){
        fprintf(file, "%d;%s;%s;%s;%d;%s\n",
                        node->value.id, node->value.serial_number,
                        node->value.status, node->value.location,
                        node->value.battery, node->value.condition);
        node = node->next;
    }
    
    fclose(file);
    return EXIT_SUCCESS;
}