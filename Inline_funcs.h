#pragma once

#include <fstream>
#include <iostream>
#include "custom_types.h"
#include "Game_results.h"
#include "Input_statements.h"
#include "termcolor.hpp"
#include <cstring>

counter_v double_size(const List<List<one_device_results> > &double_array);
List<List<string> > proceed_suit(const_string &path);
bool check_dir(const_string &path);
void printMSG(const_string &str);

/*
Функция для ввода строки
*/
inline string input() {
    printMSG(INPUT_CURSOR);
    string input;
    std::istream &is = std::cin;
    std::getline(is, input);
    return input;
}

/*
Функция для ввода целочисленного значения
*/
inline int input_int() {
    printMSG(INPUT_CURSOR);
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
    std::cout << termcolor::bright_white << msg << termcolor::reset;
    std::cout << NEW_LINE;
}

/*
 *Prints new line with bool message
 */
inline void println(const bool &msg) {
    std::cout << termcolor::bright_white << msg << termcolor::reset;
    std::cout << NEW_LINE;
}

/*
 *Print new line
 */
inline void println() {
    std::cout << NEW_LINE;
}

inline void println_important(const_string &msg) {
    std::cout << termcolor::bright_cyan << termcolor::underline << msg << termcolor::reset;
    std::cout << NEW_LINE;
}

/*
 *Print new line and red error message
 */
inline void println_error(const_string &msg) {
    std::cout << termcolor::bright_red << msg << termcolor::reset;
    std::cout << NEW_LINE;
}

/*
 *Print new line and white info message
 */
inline void println_info(const_string &msg) {
    std::cout << termcolor::blue << msg << termcolor::reset;
    std::cout << NEW_LINE;
}

/*
Вывод сообщения независимо от цвета
*/
inline void printMSG(const_string &str) {
    std::cout << termcolor::reset;
    std::cout << str << NEW_LINE;
    std::cout << termcolor::reset;
}
/*
Завершение работы программы
*/
inline void gracefully_exit() {
    println("Exiting...");
    exit(0);
}

inline bool contains(string where, char what) {
    return where.find(what);
}

inline bool starts_with(string where, string what) {
    return where.find_first_of(what) != std::string::npos;
}

/*
Инициализация строкового списка
*/
inline void init_string_arr(List<string> to_which, const List<string> &from_which) {
    for (counter_v i = 0; i < to_which.getSize(); i++) {
        if (string elem = from_which.getElement(i); !elem.empty()) {
            to_which.setElement(i, elem);
        }
    }
}

/*
Инициализация двойного массива
Поддерживается только двойной массив (массив массивов)
@param to_which в какой массив передавать инициализацию
@param by чем заполнять массивы
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

/*
Функция для вывода конечного отчёта по тестированию
@param seconds_time время в секундах
*/
inline void get_time_stat(const unsigned long seconds_time) {
    println_important("Отчет по тестированию");
    println("На тестирование ушло:");
    const counter_v seconds = seconds_time % 60 != 0 ? seconds_time % 60 : 0;
    const counter_v minutes = seconds / 60;
    const counter_v hours = minutes > 60 ? minutes / 60 : 0;
    printf("%d часов ", hours);
    printf("n%d минут ", minutes);
    printf("n%d секунд", seconds);
}

/*
Разделение строки по: 
@param str строка для разделения
@param delimeter для разделения
*/
inline List<string> split_string(const_string &str, const char &delimiter) {
    if (str.empty()) {
        println("Empty string");
        return {};
    }
    if (contains(str, delimiter)) {
        return {};
    }
    if (contains(str, delimiter)) {
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

/*
Подсчёт длины двойного массива
*/
inline counter_v double_size(const List<List<one_device_results> > &double_array) {
    const counter_v rows = double_array.size(); // Количество строк
    const counter_v cols = double_array.empty() ? 0 : double_array.getElement(0).size(); // Количество столбцов
    const counter_v size = rows + cols;
    return size;
}

/*
Перевод массива символов в строку
@param chars массив символов для перевода в строку
*/
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
@param str строка для перевода в массив
*/
inline List<string> Atos(const_string &str) {
    if (check_file(str)) {
        println_info("Argument is a file");
        return proceed_double_array(proceed_suit(str));
    }
    if (str.find(',') == -1) {
        println_info("using as separator ','");
        List<string> strings = split_string(str, ',');
        return strings;
    }
    println_info("using as separator ' ' *whitespace");
    List<string> strings = split_string(str, ' ');
    return strings;
}

List<string> proceed_internal_suit(const_string &path);

/*
Функция выполняет открытие файла набора тестов и читает его содержимое (построчно)
Возвращается содержимое файла
@param path путь к файлу с набором тестов
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
        if (fileLine != NEW_LINE) {
            if (!starts_with(fileLine, IGNORE_TEST)) {
                test_suit.addElement(fileLine);
            }
            if (starts_with(fileLine, ANOTHER_SUIT)) {
                println_info("Another suit detected");
                test_suit = proceed_internal_suit(fileLine);
            }
        }
        all_suit.addElement(test_suit);
    }
    in.close();
    return all_suit;
}

/*
Обработка внутреннего набора тестов
*/
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
        if (fileLine != NEW_LINE) {
            if (!starts_with(fileLine, IGNORE_TEST)) {
                test_suit.addElement(fileLine);
            }
        }
    }
    in.close();
    return test_suit;
}

/*
Обработка двойного массива и переделывание его в одномерный массив
*/
inline List<string> proceed_double_array(const List<List<string> > &d_array) {
    List<string> test_suit;
    for (const auto &outer: d_array) {
        for (const auto &inner: outer) {
            test_suit.addElement(inner);
        }
    }
    return test_suit;
}

/*
 bool value end_testing_save - параметр, обозначающий в какой период будет сохранение (просто для разделения логики в функции)
 @param true для сохранения в основном потоке выполнения 
 @param false для сохранения в функции help_menu
 */
inline void save_results(const bool end_testing_save) {
    time_t timestamp;
    ctime(&timestamp);
    if (end_testing_save) {
        println_info("Saving testing progress");
        const_string save_path = strcat(save_file_name + timestamp, save_ext);
        std::ofstream out(save_path, std::ios::app);
        if (out.is_open()) {
            for(auto res1 : all_stages_test_results) {
                for (auto res2: res1) {
                    out << res2.get_game_name();
                    out << res2.get_platform_name();
                    // out << res2.get_stages_res();
                    println();
                }
            }
        } else {
            println_error("Failed to open save file");
        }
        out.close();
    } else {
        println_info("Saving current testing progress");
        println("Enter stage");
        const_string save_point_stages = input();
        println("Enter device");
        const_string save_point_device = input();
        const_string save_path = strcat(save_file_name + timestamp, save_ext);
        std::ofstream out(save_path, std::ios::trunc);
        if (out.is_open()) {
            out << save_point_stages << std::endl;
            out << save_point_device << std::endl;
        } else {
            println_error("Failed to open save file");
        }
        out.close();
    }
}
