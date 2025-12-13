#ifndef DBMS_H
#define DBMS_H

#include <stdbool.h>

FILE *initialization_DB(char *name_file);

static bool validation_DB(FILE * file_ptr);

list_type *get_data_DB(FILE * file_ptr);

int work_push_DB(list_type *list, char *name_file);

#endif