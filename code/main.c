#include <stdio.h>
#include "list.h"
int main(){
    int arr[3] = {1,2, 3};
    list_type *list = initialization_list(arr,3);
    del_last_el(list);
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