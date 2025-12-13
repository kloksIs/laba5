#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "type_and_struct.h"
#include "list.h"
#include "DBMS.h"
#include "display_menu.h"
#include "work_menu.h"

#define CHOICE_IN_MAIN 4
#define CHOICE_IN_FAID 7


int main(){
    FILE *file = initialization_DB("database.txt");
    if (!file){
        printf("Файл не получилось инициализировать\n");
        exit(EXIT_FAILURE);
    }
    list_type *list = get_data_DB(file);
    fclose(file);

    short location = main_menu;
    list_type *list_with_found;
    list_type *list_edit;
    while (location != stop_program){
        switch (location) {
        case main_menu:
            text_menu_main();
            location = input_verification(CHOICE_IN_MAIN);
            location = transform(location, menu_DB_show, menu_find, menu_edit, stop_program);
            break;
        case menu_DB_show:
            work_DB_show(get_array_value(list), list->size, false);
            work_continue_menu();
            location = main_menu;
            break;
        case menu_find:
            text_find_menu();
            location = input_verification(CHOICE_IN_FAID);
            if (location == 7){
                location = main_menu;
                break;
            }
            list_with_found = find_All_el_byAttr(list, (int)location, get_search_value(&location));
            if (location == menu_find){;
                break;
            }
            work_find(get_array_value(list_with_found),list_with_found->size);
            work_continue_menu();
            location = menu_find;
            break;
        case menu_edit:
            list_edit = get_clone_list(list);
            location = work_edit(list_edit);
            if (location){
                append_id_in_list(list_edit);
                free_list(list);
                list = list_edit;
                list_edit = NULL;
                if (work_push_DB(list, "t")){
                    exit(EXIT_FAILURE);
                }
                printf("База Данных успешно обновлена.\n");
                work_continue_menu();
            } else free_list(list_edit);

            location = main_menu;
            break;
        }
    }
    return 0;
}