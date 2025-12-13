#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
#include "list.h"
#include "display_menu.h"
#include "work_menu.h"

#define ERROR_GLOBAL -1
#define ERROR_OVERFLOW -2
#define BUFFER_SIZE 50
#define NEED_ATTRS 6
#define LEN_LOCATION 18 


short transform(short num, ...){
    va_list args;
    va_start(args, num);

    short i = num;
    while (i > 0) {
        num = (short)va_arg(args, int); 
        i--;
    }
    va_end(args);
    
    return num;
}

void work_continue_menu(){
    int c;
    printf("\n");
    printf("Для продолжения нажмите Enter: ");
    while ((c = getchar()) != '\n'){
        if (c == EOF){
            printf("\nОшибка ввода.");
            exit(EXIT_FAILURE);
        }
        
    }
    printf("\033[2J\033[H");
    printf("\n");
}

int get_str_and_len(char *str, int size_buffer){
    int len = 0;
    if(!fgets(str, size_buffer, stdin)){
        printf("\nОшибка ввода.\n");
        return ERROR_GLOBAL;
    }
    for (int i = 0; i < size_buffer; i++){
        if (str[i] == '\n'){
            str[i] = '\0';
            return len;
        }
        len++;
    }
}

short input_verification(short num_choice){
    int c;
    short number;
    short count = 0;

    while (!count) {
        count = scanf(" %hd", &number);
        while ((c = getchar()) != '\n'){
            if (c == EOF){
            printf("\nОшибка ввода.");
            exit(EXIT_FAILURE);
            }
        }

        if (1 > number || number > num_choice) {
            count = 0;
        }
        if (!count) {
            printf("\033[1F\033[2K");
            printf("Неверный ввод. Введите один из представленных пунктов: ");
        }
    }

    printf("\033[2J\033[H");
    printf("\n");
    return number;
}

void work_DB_show(data_type *array, int size, bool count_id){
    int ID;

    printf("База данных для городских самокатов. Характеристик: 6 Количество самокатов: %d\n",size);
    printf("-------------------------------------------------------------------------------------------\n");
    printf("%s%-2s    %-14s      %-6s\t     %-7s\t     %-3s\t   %-15s\n%s",
        "\033[1m",
        "ID_Num", "Серийный номер", "Статус", "Локация", "Батарея(%)", "Состояние",
        "\033[0m");
    printf("-------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < size; i++){
        if (!count_id){
            ID = array[i].id;
        } else ID = i + 1;
        
        printf("%-4d    %-10s        %-s\t%-15s\t%-3d\t   %-15s\n",
            ID,
            array[i].serial_number, 
            array[i].status, 
            array[i].location, 
            array[i].battery, 
            array[i].condition);

        printf("-------------------------------------------------------------------------------------------\n");
    }

    free(array);
    array = NULL;
}

int_or_str get_search_value(short *point_ptr){
    int point = (int) *point_ptr;
    int_or_str value;
    char char_value[BUFFER_SIZE];

    printf("\033[2J\033[H");
    printf("Помните: ");
    switch (point) {
    case ID_Num:
        printf("ID - числа от 1 до 9999.\n");
        break;
    case Serial_Number_Num:
        printf("Серийный номер - набор латинских букв и цифр состоящий из 10 символов.\n");
        break;
    case Status_Num:
        printf("Статус - \"Арендован\" либо \"Свободен\" либо \"Обслуживание\" либо \"Заблокирован\", без \"\".\n");
        break;
    case Location_Num:
        printf("Локация - координаты записанные в форме [??.????, ??.????], где ? - цифры, [] - учитываются.\n");
        break;
    case Battery_Num:
        printf("Заряд батареи - показатель 1 до 100 в %% (символ %% писать не нужно).\n");
        break;
    case Condition_Num:
        printf("Состояние - Либо \"Исправен\" либо \"Требуется ремонт\", без \"\".\n");
        break;
    }
    printf("Для отмены нажмите Enter.\n");

    printf("Введите значение для поиска: ");
    int len = get_str_and_len(char_value, BUFFER_SIZE);
    if (len < 1){
        if (len == ERROR_GLOBAL){
            exit(EXIT_FAILURE);
        }
        *point_ptr = menu_find;
    }
    if (point == ID_Num || point == Battery_Num){
        value.num = atoi(char_value);
    } else {
        strcpy(value.str,char_value);
    }
    printf("\033[2J\033[H");
    printf("\n");
    
    return value;
}

void work_find(data_type *array, int size){
    if (size == 0){
        printf("Найдено объектов: %d\n",size);
        return;
    }
    
    printf("Найдено объектов: %d\n",size);
    printf("-------------------------------------------------------------------------------------------\n");
    printf("%s%-2s    %-14s      %-6s\t     %-7s\t     %-3s\t   %-15s\n%s",
        "\033[1m",
        "ID_Num", "Серийный номер", "Статус", "Локация", "Батарея(%)", "Состояние",
        "\033[0m");
    printf("-------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < size; i++){
        printf("%-4d    %-10s        %-s\t%-15s\t%-3d\t   %-15s\n",
            array[i].id,
            array[i].serial_number, 
            array[i].status, 
            array[i].location, 
            array[i].battery, 
            array[i].condition);

        printf("-------------------------------------------------------------------------------------------\n");
    }

    free(array);
    array = NULL;
}

Node *serial_number_node(list_type *list, char *ser_num, short *point, short now_point){
    Node *node;
    int_or_str box;
    int len;

    printf("Серийный номер - набор латинских букв и цифр состоящий из 10 символов.\n");
    printf("Для отмены нажмите Enter.\n");
    printf("Введите серийный номер: ");
    len = get_str_and_len(box.str, BUFFER_SIZE);
    if (len <= 0){
        if (len == ERROR_GLOBAL) exit(EXIT_FAILURE);
        *point = main_edit_menu;
        return NULL;
    } else if (len != 10){
        printf("Неверный размер серийного номера. Попробуйте ещё раз.\n");
        *point = now_point;
        return NULL;
    }
    node = find_node_byValue(list, box, 2);
    strcpy(ser_num, box.str);
    *point = go;
    return node;
}
    
data_type *work_add(list_type *list, short *point, short now_point){
    data_type *data =(data_type*) malloc(sizeof(data_type));
    if (!data){
        printf("Ошибка выделения памяти, что-то пошло не так\n");
        *point = main_edit_menu;
        return NULL;
    }
    
    char str[BUFFER_SIZE * 3];
    short count = 0;

    int len = get_str_and_len(str, BUFFER_SIZE * 3);
    if (len <= 0){
        if (len == ERROR_GLOBAL) exit(EXIT_FAILURE);
        *point = main_edit_menu;
        return NULL;
    }

    count = sscanf(str, "%[^|]|%[^|]|%d|%[^\n]",
                                    data->status, data->location,
                                    &data->battery, data->condition);
    if (count != NEED_ATTRS - 2) {
        printf("\n");
            printf("Удалось прочитано только %d данных из %d. Проверьте ввод и попробуйте ещё раз.\n", count, NEED_ATTRS - 2);
            *point = now_point;
            return NULL;
    }

    strcpy(str, data->status);
    if (!(strcmp(str, "Арендован") == 0 || 
            strcmp(str, "Свободен") == 0 || 
            strcmp(str, "Обслуживание") == 0 || 
            strcmp(str, "Заблокирован") == 0)) {
        printf("Введён неверный статус.\n");
        *point = now_point;
        return NULL;
    }

    count = sscanf(data->location, "[%[^]]]", str);
    if (!(count == 1 && strlen(data->location) == 18)){
        printf("Введёна неверная локация.\n");
        *point = now_point;
        return NULL;
    }
    
    if (data->battery < 0 || data->battery > 100){
        printf("Введённ неверный заряд батареи.\n");
        *point = now_point;
        return NULL;
    }

    strcpy(str, data->condition);
    if (!(strcmp(str, "Исправен") == 0||strcmp(str, "Требуется ремонт") == 0)){ 
        printf("Введено не верное состояние.\n");
        *point = now_point;
        return NULL;
    }
    
    *point = go;
    return data;


}

short work_edit(list_type *list_clone){
    short location_in_edit = main_edit_menu;
    short choice = 7;
    char serial_num[BUFFER_SIZE];
    Node *node_now;
    data_type *Data;
    int_or_str temp;

    while (location_in_edit != exit_back) {
        switch (location_in_edit) {
        case main_edit_menu:
            text_edit_menu();
            location_in_edit = input_verification(choice);
            break;
        
        case menu_edit_DB_show:
            work_DB_show(get_array_value(list_clone), list_clone->size, true);
            work_continue_menu();
            location_in_edit = main_edit_menu;
            break;

        case menu_add:
            node_now = serial_number_node(list_clone, serial_num, &location_in_edit, menu_add);
            if (location_in_edit != go){
                if (location_in_edit == menu_add){
                    work_continue_menu();
                }
                printf("\033[2J\033[H");
                break;
            }
            if (node_now != NULL){
                printf("Объект с таким номером уже существует\n");
                work_continue_menu();
                location_in_edit = menu_add;
                break;
            }
            text_add();
            Data = work_add(list_clone, &location_in_edit, menu_add);
            if (location_in_edit != go){
                printf("\033[2J\033[H");
                break;
            }
            strcpy(Data->serial_number,serial_num);
            append(list_clone, *Data);
            free(Data);
            printf("Объект успешно добавлен.\n");
            work_continue_menu();
            location_in_edit = main_edit_menu;
            break;

        case menu_do_edit:
            node_now = serial_number_node(list_clone, serial_num, &location_in_edit, menu_do_edit);
            if (location_in_edit != go){
                if (location_in_edit == menu_do_edit){
                    work_continue_menu();
                }
                printf("\033[2J\033[H");
                break;
            }
            if (node_now == NULL){
                printf("Объект с таким номером не существует, нечего редактировать.\n");
                work_continue_menu();
                location_in_edit = menu_do_edit;
                break;
            }
            text_add();
            Data = work_add(list_clone, &location_in_edit, menu_do_edit);
            if (location_in_edit != go){
                work_continue_menu();
                break;
            }
            strcpy(node_now->value.status, Data->status);
            strcpy(node_now->value.location, Data->location);
            strcpy(node_now->value.condition, Data->condition);
            node_now->value.battery = Data->battery;
            free(Data);
            printf("Объект успешно обновлён.\n");
            work_continue_menu();
            location_in_edit = main_edit_menu;
            break;

        case menu_remove:
            node_now = serial_number_node(list_clone, serial_num, &location_in_edit, menu_remove);
            if (location_in_edit != go){
                if (location_in_edit == menu_remove){
                    work_continue_menu();
                }
                printf("\033[2J\033[H");
                break;
            }
            if (node_now == NULL){
                printf("Объект с таким номером не существует, нечего удалять.\n");
                work_continue_menu();
                location_in_edit = menu_remove;
                break;
            }
            strcpy(temp.str, node_now->value.serial_number);
            remove_el_byValue(list_clone, temp,2);
            printf("Объект успешно удалён.\n");
            work_continue_menu();
            location_in_edit = main_edit_menu;
            break;

        case menu_save:
            return 1;
            break;
        }
    }
    return 0;
}