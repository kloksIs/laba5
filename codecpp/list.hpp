#ifndef CLASS_H
#define CLASS_H

#include <iostream>
#include <string>

struct Node {
    struct Data{
        std::string id;
        std::string serial_number;
        std::string status;
        std::string location;
        std::string battery;
        std::string condition;
    } data;
    Node *prev;
    Node *next;
};

enum Field { ID = 0, SERIAL, STATUS, LOCATION, BATTERY, CONDITION };

class list{
private:
    int _size = 0;
    Node *_head = nullptr; //начало
    Node *_tail = nullptr; //конец
public:
    list() = default;
    list(const Node::Data* array, int size);
    list(const list& other);
    ~list();
    list& operator=(list other);
    void clear() noexcept ;
    void append(const Node::Data& value);
    Node::Data* get_full_data();
    Node::Data* get_data(Node*);
    Node::Data* get_data(const std::string& in_id);
    Node* findNode(const std::string& value, int num_arg = Field::ID);
    bool pop(const std::string& in_id);
    friend void swap(list& first, list& second) noexcept;
    void edit(const Node::Data& new_data, const std::string& in_id);
    int get_size();
    void refreshIds();
};


#endif