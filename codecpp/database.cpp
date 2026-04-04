#include "database.hpp"
#include <iostream>
#include "list.hpp"
#include <string>
#include <fstream>
#include <stdexcept>
#include <regex>
#include <limits>

Database::Database(const std::string& file_str) : name_file(file_str), file_(name_file, std::ios::in), array_data(nullptr), size_(0){
    try {
        if (!initialization(file_)){
            throw std::runtime_error("Не получилось открыть файл " + name_file);
        }

        std::string line;
        array_data = new Node::Data[size_];
        std::getline(file_,line); // пропускаем две первые строки
        std::getline(file_,line);
        //получаем данные
        for (int i = 0; i < size_; i++){
            std::getline(file_, line, ';'); array_data[i].id = line;

            std::getline(file_, line, ';'); array_data[i].serial_number = line;

            std::getline(file_, line, ';'); array_data[i].status = line;

            std::getline(file_, line, ';'); array_data[i].location = line;

            std::getline(file_, line, ';'); array_data[i].battery = line;

            std::getline(file_, line, ';'); array_data[i].condition = line;

            file_.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        file_.clear(); 
        file_.seekg(0, std::ios::beg);
    }
    catch(const std::exception& e){
        delete[] array_data;
        array_data = nullptr; 
        std::cerr << e.what() << '\n';
        throw;
    }
}

Database::~Database(){
    delete[] array_data;
    array_data = nullptr; 
}

Node::Data* Database::get_data(){
    return array_data;
}

int Database::get_size(){
    return size_;
}

bool Database::initialization(std::fstream& file){
        size_ = 0;
        
        if (!file.is_open()){
            throw std::runtime_error("Не получилось открыть файл " + name_file);
        }

        std::string line;
        std::getline(file,line); // пропускаем две первые строки
        std::getline(file,line);
        std::regex pattern(R"(\d+;[^;]+;[^;]+;\[[^\]]+\];\d+;[^;]+;)");

        while (std::getline(file, line)){
            if (!std::regex_match(line, pattern)) {
                throw std::runtime_error("Ошибка формата у объекта " + std::to_string(size_+1) + ": " + line);
            }
            size_++;
        }
        if (size_ == 0){
            throw std::runtime_error("Файл с некорректными данными или пуст");
        }

        file.clear(); 
        file.seekg(0, std::ios::beg);
        return true;
}

bool Database::save_changes(Node::Data* arr ,int size) {
    if (file_.is_open()) {
        file_.close();
    }

    file_.open(name_file, std::ios::out | std::ios::trunc);
    if (!file_.is_open()) {
        return false; 
    }

    file_ << "--- Database Table --- objects: " + std::to_string(size) << "\n";
    file_ << "ID;SERIAL;STATUS;LOCATION;BATTERY;CONDITION;" << "\n";

    for (int i = 0; i < size; i++){
        file_ << arr[i].id << ";"
              << arr[i].serial_number << ";"
              << arr[i].status << ";"
              << arr[i].location << ";"
              << arr[i].battery << ";"
              << arr[i].condition << ";" // Тот самый ';' в конце
              << "\n";
    }
    file_.close();
    file_.open(name_file, std::ios::in); 
    
    return true;
}
