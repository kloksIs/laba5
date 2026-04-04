#include <list.hpp>
#include <iostream>
#include <string>
#include <stdexcept>
using namespace std;


list::list(const Node::Data* array, int size) : list(){
    if (!array) {
        throw invalid_argument("Передан пустой массив");
    }
    if (size <= 0) {
        throw invalid_argument("Размер массива должен быть больше 0");
    }

    try {
        this->_size = size;
        for (int i = 0; i < size; ++i) {
            Node* newNode = new Node;
            newNode->data = array[i];
            newNode->next = nullptr;
            newNode->prev = _tail;

            if (_tail) {
                _tail->next = newNode;
            } else {
                _head = newNode;
            }
            
            _tail = newNode;
        }
    }
    catch(const std::exception& e){
        clear();
        cerr << e.what() << endl;
        throw;
    }
    
};

list::list(const list& other) : list(){
    try{
        Node* temp = other._head;
        while (temp){
            this->append(temp->data);
            temp = temp->next;
        }
        
    }
    catch(const std::exception& e){
        std::cerr << e.what() << '\n';
        clear();
        throw;
    }
    
};

list::~list(){
    clear();
}

list& list::operator=(list other){
    swap(*this, other);
    return *this;
}

void list::clear() noexcept {
    while (_head){
        Node* nextNode = _head->next;
        delete _head;
        _head = nextNode;
    }
    _tail = nullptr;
    _size = 0;
};

void list::append(const Node::Data& value){
    Node* newNode = new Node;
    newNode->data = value;
    newNode->next = nullptr;
    newNode->prev = _tail;

    if (_tail) {
        _tail->next = newNode;
    } else {
        _head = newNode;
    }

    _tail = newNode;
    _size++;
}

Node::Data* list::get_full_data(){
    std::string line;
    Node::Data* array_data = new Node::Data[_size];
    Node* nowNode = _head;
    //получаем данные
    for (int i = 0; i < _size; i++){
        array_data[i].id = nowNode->data.id;
        array_data[i].serial_number = nowNode->data.serial_number;
        array_data[i].status = nowNode->data.status;
        array_data[i].location = nowNode->data.location;
        array_data[i].battery = nowNode->data.battery;
        array_data[i].condition = nowNode->data.condition;
        nowNode = nowNode->next;
    }
    return array_data;
}

Node::Data* list::get_data(Node* thisNode){
    return &thisNode->data;
}

Node::Data* list::get_data(const std::string& in_id){
    Node* temp = _head;
    while (temp){
        if (temp->data.id == in_id){
            return &temp->data;
        }
        temp = temp->next;
    }
    return nullptr;
};

Node* list::findNode(const std::string& value, int num_field){
    if (num_field < 0 || num_field >= 6) {
        throw out_of_range("Некорректный индекс поля: " + to_string(num_field));
    }

    Node* temp = _head;
    while (temp){
        string* array_field[] = {&temp->data.id, &temp->data.serial_number, &temp->data.status,
                            &temp->data.location, &temp->data.battery, &temp->data.condition};
        string* arg_now = array_field[num_field];
        if (*arg_now == value){
            return temp;
        }
        temp = temp->next;
    }
    return nullptr;
};

bool list::pop(const std::string& in_id){
    Node* del = findNode(in_id);
    if (!del){
        return false;
    }
    if (del->prev){
        del->prev->next = del->next;
    } else {
        _head = del->next;
    }

    if (del->next){
        del->next->prev = del->prev;
    } else {
        _tail = del->prev;
    }
    delete del;
    _size--;
    return true;
};

void swap(list& first, list& second) noexcept{
    std::swap(first._head, second._head);
    std::swap(first._tail, second._tail);
    std::swap(first._size, second._size);
};

void list::edit(const Node::Data& new_data, const std::string& in_id){
    Node* newNode = findNode(in_id);
    if (!newNode){
        throw invalid_argument("Не верный id или такого id не существует");
    }
    newNode->data = new_data;
}

int list::get_size(){
    return _size;
}

void list::refreshIds() {
    Node* current = _head;
    int counter = 1;
    while (current != nullptr) {
        current->data.id = std::to_string(counter);
        current = current->next;
        counter++;
    }
}