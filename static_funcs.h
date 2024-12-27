#pragma once

#include <fstream>
#include <iostream>
#include "custom_types.h"
#include "Game_results.h"
#include "input_statements.h"
#include "termcolor.hpp"

counter_v double_size(const List<List<one_device_results> > &double_array);

List<List<string> > proceed_suit(const_string &path);

bool check_dir(const_string &path);

void printMSG(const_string &str);

inline string input() {
    printMSG(">> ");
    string input;
    std::istream &is = std::cin;
    std::getline(is, input);
    return input;
}

inline int input_int() {
    printMSG(">> ");
    string input;
    std::istream &is = std::cin;
    std::getline(is, input);
    if (const int integer = std::stoi(input); isdigit(integer) == true) {
        return integer;
    }
    return -1;
}

/*
 *Prints new line with message
 */
inline void println(const_string &msg) {
    std::cout << termcolor::bright_white << msg;
    std::cout << "\n";
}

/*
 *Prints new line with bool message
 */
inline void println(const bool &msg) {
    std::cout << termcolor::bright_white << msg;
    std::cout << "\n";
}

/*
 *Print new line
 */
inline void println() {
    std::cout << "\n";
}

inline void println_important(const_string &msg) {
    std::cout << termcolor::bright_cyan << termcolor::bold << msg;
    std::cout << "\n";
}

/*
 *Print new line and red error message
 */
inline void println_error(const_string &msg) {
    std::cout << termcolor::bright_red << msg;
    std::cout << "\n";
}

/*
 *Print new line and white info message
 */
inline void println_info(const_string &msg) {
    std::cout << termcolor::bright_white << msg;
    std::cout << "\n";
}

/*
Вывод сообщения независимо от цвета
*/
inline void printMSG(const_string &str) {
    std::cout << termcolor::reset;
    std::cout << str << "\n";
    std::cout << termcolor::reset;
}

inline void gracefully_exit() {
    println("Exiting...");
    exit(0);
}

inline void init_string_arr(List<string> to_which, const List<string> &from_which) {
    for (counter_v i = 0; i < to_which.getSize(); i++) {
        if (string elem = from_which.getElement(i); !elem.empty()) {
            to_which.setElement(i, elem);
        }
    }
}

/*
 * Поддерживается только двойной массив (массив массивов)
 */
inline void init_array_by(const List<List<one_device_results> > &to_which, const one_test_result &by) {
    counter_v inner_array = 0;
    counter_v outer_array = 0;
    for (outer_array = 0; outer_array < double_size(to_which); outer_array++) {
        for (inner_array = 0;
             inner_array < to_which.getElement(outer_array).getElement(inner_array).size(); inner_array++) {
            to_which.getElement(outer_array).getElement(inner_array) = reinterpret_cast<const char *>(&by);
        }
    }
}

inline void get_time_stat(const unsigned long seconds_time) {
    println_important("Отчет по тестированию");
    println("На тестирование ушло:");
    const counter_v seconds = seconds_time % 60 != 0 ? seconds_time % 60 : 0;
    const counter_v minutes = seconds / 60;
    const counter_v hours = minutes > 60 ? minutes / 60 : 0;
    printf("%d часов\n%d минут\n%d секунд\n", hours, minutes, seconds);
}

inline List<string> split_string(const_string &str, const char &delimiter) {
    if (str.empty()) {
        println("Empty string");
        return {};
    }
    if (str.find_first_of(delimiter) == string::npos) {
        return {};
    }
    if (!str.contains(delimiter)) {
        println("string contains invalid characters");
        return {};
    }
    List<string> splitted;
    const string &words = str;
    counter_v index_start = 0;
    counter_v index_end = 0;
    for (counter_v cur_pos = 0; cur_pos < words.length(); cur_pos++) {
        char current_char = words[cur_pos];
        if (current_char == delimiter) {
            index_start = cur_pos + 1;
            index_end = words.find(delimiter, cur_pos + 1);

            const string word = words.substr(index_start, index_end);
            splitted.addElement(word);
        }
    }
    return splitted;
}

inline counter_v double_size(const List<List<one_device_results> > &double_array) {
    const counter_v rows = double_array.size(); // Количество строк
    const counter_v cols = double_array.empty() ? 0 : double_array.getElement(0).size(); // Количество столбцов
    const counter_v size = rows + cols;
    return size;
}

inline string get_string_by_chars(char chars[]) {
    string tmp;
    for (int i = 0; i < strlen(chars); i++) {
        tmp.append(&chars[i]);
    }
    return tmp;
}

/*
Проверка того, что переданный путь это файл
*/
inline bool check_file(const_string &path) {
    std::ifstream in;
    in.open(path);
    if (in.is_open()) {
        in.close();
        println_info("File exists");
        return true;
    }
    println_error("Error occurred in checking file");
    check_dir(path);
    if (path.find('.') == -1) {
        in.close();
        println_error("Maybe path is not contains file extension");
    }
    gracefully_exit();
    return false;
}

/*
Проверка того, что переданный путь это директория
*/
inline bool check_dir(const_string &path) {
    std::ifstream in;
    in.open(path);
    if (in.is_open()) {
        println_info("Dir exists");
        in.close();
        return true;
    }
    println_error("Error occurred is checking dir");
    in.close();
    gracefully_exit();
    return false;
}

List<string> proceed_double_array(const List<List<string> > &d_array);

/*
Перевод строки в массив
*/
inline List<string> Atos(const_string &str) {
    if (check_file(str)) {
        println_info("Argument is a file");
        return proceed_double_array(proceed_suit(str));
    }
    if (str.find(',') == -1) {
        println_info("using as separator ','");
        List<string> strings = split_string(str, *",");
        return strings;
    }
    println_info("using as separator ' ' *whitespace");
    List<string> strings = split_string(str, *" ");
    return strings;
}

List<string> proceed_internal_suit(const_string &path);

/*
Функция выполняет открытие файла набора тестов и читает его содержимое (построчно)
Возвращается содержимое файла
*/
inline List<List<string> > proceed_suit(const_string &path) {
    std::ifstream in;
    in.open(path);
    if (!in.is_open()) {
        println_error("error occurred during open suit file");
        gracefully_exit();
        return List<List<string> >{};
    }
    std::string fileLine;
    List<string> test_suit;
    List<List<string> > all_suit;
    while (std::getline(in, fileLine)) {
        if (fileLine != "\n") {
            if (!fileLine.starts_with(ignore_test)) {
                test_suit.addElement(fileLine);
            }
            if (fileLine.starts_with(another_suit)) {
                println_info("Another suit detected");
                test_suit = proceed_internal_suit(fileLine);
            }
        }
        all_suit.addElement(test_suit);
    }
    in.close();
    return all_suit;
}

inline List<string> proceed_internal_suit(const_string &path) {
    std::ifstream in;
    in.open(path);
    if (!in.is_open()) {
        println_error("error occurred during file open");
        gracefully_exit();
        return List<string>{};
    }
    std::string fileLine;
    List<string> test_suit;
    while (std::getline(in, fileLine)) {
        if (fileLine != "\n") {
            if (!fileLine.starts_with(ignore_test)) {
                test_suit.addElement(fileLine);
            }
        }
    }
    in.close();
    return test_suit;
}

inline List<string> proceed_double_array(const List<List<string> > &d_array) {
    List<string> test_suit;
    for (const auto &outer: d_array) {
        for (const auto &inner: outer) {
            test_suit.addElement(inner);
        }
    }
    return test_suit;
}
