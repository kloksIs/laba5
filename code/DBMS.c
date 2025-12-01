#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "list.h"

#define BUFFER_SIZE 150
#define NEED_ATTRS 6 


typedef struct {
    int id;
    char serial_number[20];
    char status[20];
    char location[50];
    short battery;
    char condition[20];
} Data_type;


FILE *initialization_DB(){
    FILE *file_ptr = fopen("database.txt", "r");
    if (!file_ptr){
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
    char first_str[BUFFER_SIZE];
    if (fgets(first_str, BUFFER_SIZE, file_ptr) != NULL){
        return false;
    }

    int num_attrs = 0, num_scooters = 0;
    short count = sscanf("БД для городских самокатов. Характеристик: %d Кол-во_самокатов: %d", &num_attrs, &num_scooters);
    if (count != 2){
        printf("Данные строчке №1 считались не корректно\n");
        return false;
    } else if (num_attrs != NEED_ATTRS) {
        printf("Программа расчитывает на 7 характеристик, а их %d\n", num_attrs);
        return false;
    }

    //Проверка, что есть все элементы и у каждого все характеристики
    int id = 0, attrs = 0;
    char str[BUFFER_SIZE];
    int c;
    bool in_object;
    fgets(str, BUFFER_SIZE, file_ptr); //Пропускаем вторую строчку

    while (fgets(str, BUFFER_SIZE, file_ptr) != NULL) {
        id++;
        attrs = 0;
        if ((c = fgetc(str)) == '\n') {
            printf("Пустая строка №%d\n", id + 2); //Т.к. счёт id со 2 строки
            return false;
        } else if (c < '0' || c > '9') {
            printf("Строка №%d начинается не с ID\n", id + 2);
            return false;
        }
         else in_object = true;
        

        while ((c = fgetc(str)) != '\n') {
            if (c == ';'){
                if (!in_object){
                    printf("Два разделяющих символа подрят в строке №%d\n", id + 2);
                    return false;
                }
                in_object = false;
                attrs++;
            } else in_object = true;
            
        }
        if (attrs != num_attrs){
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

get_data_DB(FILE * file_ptr){
    list_type *list_DB = initialization_void_list();
    Data_type data;
    char str_buffer[BUFFER_SIZE];
    short count;
    int num_str = 0;

    fgets(str_buffer, BUFFER_SIZE, file_ptr); //Пропускаем первые 2 строчки
    fgets(str_buffer, BUFFER_SIZE, file_ptr);
    num_str = 2;

    while (fgets(str_buffer, BUFFER_SIZE, file_ptr) != NULL) {
        num_str++;
        count = sscanf(str_buffer, "%d;%[^;];%[^;];%[^;];%d;%s",
                                    &data.id, data.serial_number, data.status,
                                    data.location, &data.battery, data.condition);
        if (count != NEED_ATTRS){
            printf("В строке №%d прочитано только %d поля из %d\n", num_str, count, NEED_ATTRS);
            return NULL;
        }
        


    }
    


}