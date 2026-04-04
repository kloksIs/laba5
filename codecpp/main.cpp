#include <iostream>
#include "list.hpp"
#include"database.hpp"
#include "menu.hpp"
#include <string>
#include <iomanip>
#include <limits>
#include <regex>

enum MainMenu { SHOW_DB = 1, SEARCH, EDIT, EXIT };
enum EditMenu { PREVIEW = 1, ADD, UPDATE, REMOVE, SAVE, BACK };
void printTable(Node::Data* array, int size);
bool run_EdinMenu(list& list);

int main(int argc, char* argv[]) {
    try {
        if (argc != 2) {
            std::cerr << "Ошибка: Неверное количество аргументов.\n"
                    << "Использование: " << argv[0] << " <имя_файла_бд>" << std::endl;
            return 1;
        }
        std::string name_file = argv[1];
    

        Database db(name_file);
        list db_list(db.get_data(), db.get_size());
        int choice;
        bool choice_flag = true; 
        std::string trash;

        while (true) {
                try {
                if (choice_flag){
                    Menu::main();
                    if (!(std::cin >> choice)) {
                        std::cout << "Ошибка: введите число!" << std::endl;
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        continue;
                    }
                }
                choice_flag = true;

                switch (static_cast<MainMenu>(choice)) {
                    //Вывод
                    case SHOW_DB: {
                        Node::Data* temp_data = db_list.get_full_data();
                        printTable(temp_data, db_list.get_size());
                        delete[] temp_data;
                        std::cout << "\nНажмите Enter для продолжения";
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::getline(std::cin, trash); 
                        break;
                    }
                    //Поиск
                    case SEARCH: {
                        std::string search;
                        std::string value;
                        
                        Menu::find();
                        std::cin >> search;
                        std::cout << "\nВведите данные для поиска: ";
                        std::cin >> value;
                        std::cout << std::endl;

                        Node* found_node = db_list.findNode(value, std::stoi(search) - 1);

                        if (found_node != nullptr) {
                            Node::Data* found_data = db_list.get_data(found_node);
                            
                            std::cout << "\nРезультат поиска:" << std::endl;
                            printTable(found_data, 1); 
                        } else {
                            std::cout << "\nОшибка: Объект со значением" << value << " не найден." << std::endl;
                        }

                        std::cout << "\nНажмите Enter для продолжения";
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::getline(std::cin, trash); 
                        break;
                    }
                    //Редактирование
                    case EDIT:{
                        list new_list = db_list;
                        if (run_EdinMenu(new_list)){ //подменю
                            db_list = new_list;
                        }
                        Node::Data* final_data = db_list.get_full_data();
                        bool saved = db.save_changes(final_data, db_list.get_size());
                        delete[] final_data;
                        if (!saved){
                            throw std::runtime_error("Не получилось сохранить файл " + name_file);
                        }
                                 
                        break;
                    }
                    //Выход
                    case EXIT:
                        std::cout << "Завершение работы..." << std::endl;
                        return 0;
                    default:
                        std::cout << "Неверный пункт!" << std::endl;
                }
            }
            catch(const std::out_of_range& e){
                std::cerr << e.what() << '\n';
                choice_flag = false;
                continue;
            }
            catch(const std::invalid_argument& e){
                std::cerr << e.what() << '\n';
                choice_flag = false;
                continue;
            }

            
        }
    return 0;
    }
    catch(const std::exception& e){
        std::cerr << e.what() << '\n';
        std::cerr << "Глобальная ошибка!. Программа аварийно закрылась\n" << '\n';
        return 1;
    }
    
}
void printTable(Node::Data* array, int size) {
    const int wID = 6;
    const int wSerialNum = 15;
    const int wStatus = 15;
    const int wLoc = 25;
    const int wBatt = 10;
    const int wCond = 20;

    std::cout << std::left 
              << " | " << std::setw(wID)     << "ID"
              << " | " << std::setw(wSerialNum) << "Serial"
              << " | " << std::setw(wStatus) << "Status"
              << " | " << std::setw(wLoc)    << "Location"
              << " | " << std::setw(wBatt)   << "Battery"
              << " | " << std::setw(wCond)   << "Condition" 
              << " | " << std::endl;

    std::cout << std::string(wID + wSerialNum + wStatus + wLoc + wBatt + wCond + 19, '-') << std::endl;
    for (int i = 0; i < size; ++i) {
        std::cout << " | " << std::setw(wID)     << array[i].id
                  << " | " << std::setw(wSerialNum) << array[i].serial_number
                  << " | " << std::setw(wStatus) << array[i].status
                  << " | " << std::setw(wLoc)    << array[i].location
                  << " | " << std::setw(wBatt)   << (array[i].battery + "%")
                  << " | " << std::setw(wCond)   << array[i].condition
                  << " | " << std::endl;
    }
}

bool run_EdinMenu(list& list){
    int choice;
    bool choice_flag = true;
    std::string trash;
    while (true) {
           try{
            if (choice_flag){
                    Menu::edit();
                    if (!(std::cin >> choice)) {
                        std::cout << "Ошибка: введите число!" << std::endl;
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        continue;
                    }
                }
                choice_flag = true;

                switch (static_cast<EditMenu>(choice)) {
                    //Предпоказ
                    case PREVIEW: {
                        Node::Data* temp_data = list.get_full_data();
                        printTable(temp_data, list.get_size());
                        delete[] temp_data;
                        std::cout << "\nНажмите Enter для продолжения";
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::getline(std::cin, trash); 
                        break;
                    }
                    case ADD: {
                        bool isAdded = false;
                        std::regex pattern(R"(\d+;[A-Z0-9]+;[А-Яа-я\s]+;\[[^\]]+\];\d+;[А-Яа-я\s]+;)");

                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        do {
                            std::string userInput;
                            std::string Id = std::to_string(list.get_size() + 1);
                            
                            std::cout << "\n--- Добавление (ID: " << Id << ") ---" << std::endl;
                            std::cout << "Напишите серийный номер. 10 символов (буквы и цифры)" << std::endl;
                            std::cout << "Введите: Serial;Status;Location;Battery;Condition" << std::endl;
                            std::cout << "(Или нажмите Enter для отмены)" << std::endl;
                            std::cout << "Ввод: ";
                            
                            std::getline(std::cin, userInput);
                            //Для выхода
                            if (userInput.empty()){ 
                                isAdded = true; 
                                break; 
                            }


                            //Проверка по шаблону
                            std::string fullLine = Id + ";" + userInput + ";";
                            if (!std::regex_match(fullLine, pattern)) {
                                std::cout << ">> Ошибка: Неверный формат данных!" << std::endl;
                                continue;
                            }

                            //Проверки уникальности номера серийного
                            std::stringstream str_str(userInput);
                            std::string ready_serial;
                            std::getline(str_str, ready_serial, ';');

                            if (list.findNode(ready_serial, Field::SERIAL) != nullptr) {
                                std::cout << ">> Ошибка: Серийный номер " << ready_serial << " уже есть в базе!" << std::endl;
                                continue;
                            }

                            Node::Data newData;
                            newData.id = Id;
                            newData.serial_number = ready_serial;
                            std::getline(str_str, newData.status, ';');
                            std::getline(str_str, newData.location, ';');
                            std::getline(str_str, newData.battery, ';');
                            std::getline(str_str, newData.condition, ';');

                            list.append(newData);
                            std::cout << ">> Успешно добавлено!" << std::endl;
                            isAdded = true;

                        } while (!isAdded);
                        
                        break;
                    }
                    case UPDATE:{
                        if (std::cin.peek() == '\n') std::cin.ignore(); 

                        std::cout << "\n--- Редактирование объекта ---" << std::endl;
                        std::cout << "Введите ID объекта для изменения (или Enter для отмены): ";
                        
                        std::string searchId;
                        std::getline(std::cin, searchId);

                        if (searchId.empty()) break;

                        Node* targetNode = list.findNode(searchId, Field::ID);
                        if (targetNode == nullptr) {
                            std::cout << ">> Ошибка: Объект с ID " << searchId << " не найден!" << std::endl;
                            break; 
                        }

                        std::cout << "Текущие данные объекта:" << std::endl;
                        printTable(list.get_data(targetNode), 1);

                        bool isUpdated = false;
                        std::regex pattern(R"(\d+;[A-Z0-9]+;[А-Яа-я\s]+;\[[^\]]+\];\d+;[А-Яа-я\s]+;)");

                        do {
                            std::cout << "\nВведите НОВЫЕ данные (Serial;Status;Location;Battery;Condition):" << std::endl;
                            std::cout << "(Или нажмите Enter, чтобы оставить всё как есть)\nВвод: ";
                            
                            std::string userInput;
                            std::getline(std::cin, userInput);

                            if (userInput.empty()) {
                                std::cout << ">> Изменения отменены." << std::endl;
                                break; 
                            }

                            std::string fullLine = searchId + ";" + userInput + ";";
                            if (!std::regex_match(fullLine, pattern)) {
                                std::cout << ">> Ошибка: Неверный формат данных!" << std::endl;
                                continue;
                            }

                            std::stringstream str_str(userInput);
                            Node::Data newData;
                            newData.id = searchId; 
                            
                            std::getline(str_str, newData.serial_number, ';');
                            
                            if (newData.serial_number != list.get_data(targetNode)->serial_number) {
                                if (list.findNode(newData.serial_number, Field::SERIAL) != nullptr) {
                                    std::cout << ">> Ошибка: Серийный номер " << newData.serial_number << " уже занят!" << std::endl;
                                    continue;
                                }
                            }

                            std::getline(str_str, newData.status, ';');
                            std::getline(str_str, newData.location, ';');
                            std::getline(str_str, newData.battery, ';');
                            std::getline(str_str, newData.condition, ';');

                            *list.get_data(targetNode) = newData; 
                            
                            std::cout << ">> Данные успешно обновлены!" << std::endl;
                            isUpdated = true;

                        } while (!isUpdated);
                        break;
                    }
                    

                    case REMOVE: {
                        if (std::cin.peek() == '\n') std::cin.ignore();

                        std::cout << "\n--- Удаление объекта ---" << std::endl;
                        std::cout << "Введите ID для удаления (или Enter для отмены): ";
                        
                        std::string searchId;
                        std::getline(std::cin, searchId);

                        if (searchId.empty()) break;

                        if (list.pop(searchId)) {
                            std::cout << ">> Объект с ID " << searchId << " успешно удален." << std::endl;                            
                            list.refreshIds(); 
                            std::cout << ">> Порядковые номера (ID) обновлены." << std::endl;
                        } else {
                            std::cout << ">> Ошибка: Объект с ID " << searchId << " не найден!" << std::endl;
                        }
                        break;
                    }
                    //Сохранение
                    case SAVE:
                        return true;
                    //Выход
                    case BACK:
                        return false;
                    default:
                        std::cout << "Неверный пункт!" << std::endl;
                }
           }
           catch(const std::out_of_range& e){
                std::cerr << e.what() << '\n';
                choice_flag = false;
                continue;
            }
            catch(const std::invalid_argument& e){
                std::cerr << e.what() << '\n';
                choice_flag = false;
                continue;
            }
           
        }
}
