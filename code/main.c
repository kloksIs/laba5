#include <stdio.h>
#include "list.h"
int main(){
    int arr[1] = {1};
    // list_type *list = initialization_ready_list(arr,1);
    list_type *list = initialization_void_list();
    // del_last_el(list);
    append(list, 10);
    append(list, 30);
    append(list, 10);
    append(list, 10);
    append(list, 30);
    del_last_el(list);
    int *values = get_array_value(list);
    for (int i = 0; i < list->size; i++) {
        printf("%d ", values[i]);
    }
    
    printf("\n%d\n", list->size);
    return 0;
}