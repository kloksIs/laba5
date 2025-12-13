#ifndef WORK_MENU_H
#define WORK_MENU_H

typedef enum {
    main_menu,
    menu_DB_show,
    menu_find,
    menu_edit,
    push_DB,
    stop_program,
} points;

typedef enum {
    main_edit_menu,
    menu_edit_DB_show,
    menu_add,
    menu_do_edit,
    menu_remove,
    menu_save,
    exit_back,
    go,
} points_edit;

short transform(short num, ...);

void work_continue_menu();

int get_str_and_len(char *str, int size_buffer);

short input_verification(short num_choice);

void work_DB_show(data_type *array, int size, bool count_id);

int_or_str get_search_value(short *point_ptr);

void work_find(data_type *array, int size);

Node *serial_number_node(list_type *list, char *ser_num, short *point, short now_point);

data_type *work_add(list_type *list, short *point, short now_point);

//1 - если надо сохранить изменения, 0 - если не надо
short work_edit(list_type *list_clone);

#endif