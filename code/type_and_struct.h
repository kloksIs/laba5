#ifndef TYPE_AND_STRUCT
#define TYPE_AND_STRUCT

typedef struct {
    int id;
    char serial_number[20];
    char status[30];
    char location[50];
    int battery;
    char condition[50];
} data_type;

typedef struct Node {
    data_type value;
    struct Node *prev;
    struct Node *next;
} Node;

typedef struct {
    int size;
    Node *head;
    Node *tail; //указатель на конец
} list_type;

//ID_Num = 1, Serial_Number_Num = 2, ..., Condition_Num = 6;
typedef enum{
    ID_Num = 1,
    Serial_Number_Num,
    Status_Num,
    Location_Num,
    Battery_Num,
    Condition_Num
} tupe_attr;


typedef union {
    int num;
    char str[50];
} int_or_str ;



#endif