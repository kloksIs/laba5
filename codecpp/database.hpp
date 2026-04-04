#pragma once
#include <iostream>
#include "list.hpp"
#include <string>
#include <fstream>

class Database{
public:
    Database(const std::string& file);
    ~Database();
    Node::Data* get_data();
    int get_size();
    bool save_changes(Node::Data* arr ,int size);
    
private:
    std::string name_file;
    std::fstream file_;
    Node::Data* array_data;
    int size_;
    bool initialization(std::fstream& file);
};


