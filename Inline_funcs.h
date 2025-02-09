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

using namespace std;

/*
Функция для ввода строки
*/
inline string input() {
    printMSG(INPUT_CURSOR);
    string input;
    istream &is = cin;
    getline(is, input);
    return input;
}

/*
Функция для ввода целочисленного значения
*/
inline int input_int() {
    printMSG(INPUT_CURSOR);
    string input;
    istream &is = cin;
    getline(is, input);
    if (const int integer = stoi(input); isdigit(integer) == true) {
        return integer;
    }
    return -1;
}

/*
 *Prints new line with message
 */
inline void println(const_string &msg) {
    cout << termcolor::bright_white << msg << termcolor::reset;
    cout << NEW_LINE;
}

/*
 *Prints new line with bool message
 */
inline void println(const bool &msg) {
    cout << termcolor::bright_white << msg << termcolor::reset;
    cout << NEW_LINE;
}

/*
 *Print new line
 */
inline void println() {
    cout << NEW_LINE;
}

inline void println_important(const_string &msg) {
    cout << termcolor::bright_cyan << termcolor::underline << msg << termcolor::reset;
    cout << NEW_LINE;
}

/*
 *Print new line and red error message
 */
inline void println_error(const_string &msg) {
    cout << termcolor::bright_red << msg << termcolor::reset;
    cout << NEW_LINE;
}

/*
 *Print new line and white info message
 */
inline void println_info(const_string &msg) {
    cout << termcolor::blue << msg << termcolor::reset;
    cout << NEW_LINE;
}

/*
Вывод сообщения независимо от цвета
*/
inline void printMSG(const_string &str) {
    cout << termcolor::reset;
    cout << str << NEW_LINE;
    cout << termcolor::reset;
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
    return where.find_first_of(what) != string::npos;
}

/*
Инициализация строкового списка
*/
inline void init_string_arr(List<string> to_which, const List<string> &from_which) {
    try {
        for (counter_v i = 0; i < from_which.getSize(); i++) {
            string elem = from_which.getElement(i);
            if (elem.length() != 0) {
                to_which.addElement(elem);
            }
        }
        if (to_which.getSize() == 0) {
            throw ERROR_CODE;
        }
    } catch (int code) {
        println_error("error in init string array " + code);
    }
}

/*
Инициализация двойного массива
Поддерживается только двойной массив (массив массивов)
@param to_which в какой массив передавать инициализацию
@param by чем заполнять массивы
 */
inline void init_array_by(counter_v size, const List<List<one_device_results> > &to_which, const one_test_result &by) {
    counter_v inner_array = 0;
    counter_v outer_array = 0;
    try {
        for (outer_array = 0; outer_array < size; outer_array++) {
            for (inner_array = 0; // Проблема с взятием нулевой длины
                inner_array < to_which.getElement(outer_array).getElement(inner_array).size(); inner_array++) {
                    to_which.getElement(outer_array).getElement(inner_array) = reinterpret_cast<const char *>(&by);
                }
        }
        if (to_which.getSize() == 0) {
            throw ERROR_CODE;
        }
    } catch (int code) {
        println_error("error in init of array with code " + code);
    }
}

/*
Функция для вывода конечного отчёта по тестированию
@param seconds_time время в секундах
*/
inline void get_time_stat(const unsigned long seconds_time) {
    if (seconds_time != 0){
        println_important("Отчет по тестированию");
        println("На тестирование ушло:");
        const counter_v seconds = seconds_time % 60 != 0 ? seconds_time % 60 : 0;
        const counter_v minutes = seconds / 60;
        const counter_v hours = minutes > 60 ? minutes / 60 : 0;
        printf("%d часов ", hours);
        printf("n%d минут ", minutes);
        printf("n%d секунд ", seconds);
    } else {
        println("Tests were not executed");
    }
}

/*
Разделение строки по: 
@param str строка для разделения
@param delimeter для разделения
@return Может вернуть пустой список, при условии пустой, переданной строки
Иначе, возвращает список, который содержит строки
*/
inline List<string> split_string(const_string &str, const char &delimiter) {
    if (str.empty()) {
        println("Empty string");
        return {};
    }
    if (!contains(str, delimiter)) {
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
@return строка, созданная из массива символов
*/
inline string get_string_by_chars(char chars[]) {
    try {
        string tmp;
        for (int i = 0; i < strlen(chars); i++) {
            tmp.append(&chars[i]);
        }   
        if (tmp.length() == 0) {
            throw ERROR_CODE;
        }
        return tmp;
    } catch (int code) {
        println_error("error in get string by chars");
    }
}

/*
Проверка того, что переданный путь это файл
@return true - если файл существует;
false - если файла не существует
*/
inline bool check_file(const_string &path) {
    ifstream in;
    in.open(path);
    if (in.bad() == false) {
        println_info("file is present");
        return true;
    }
    println_error("error in file reading, file not exists");
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
@return true - если директория существует;
false - если директории не существует
*/
inline bool check_dir(const_string &path) {
    ifstream in;
    in.open(path);
    if (in.bad() == false) {
        println_info("Dir exists");
        in.close();
        return true;
    }
    println_error("error occurred is checking dir");
    in.close();
    gracefully_exit();
    return false;
}

List<string> proceed_double_array(const List<List<string> > &d_array);

/**
Перевод строки в массив.
@param str строка для перевода в массив.
@return список строк полученных путём разделения строки по разделителю.
*/
inline List<string> Atos(const_string &str) {
    try {
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
        if (strings.getSize() == 0) {
            throw ERROR_CODE;
        }
        return strings;
    }
     catch (int code) {
        println_error("error in atos func");
    }
}

List<string> proceed_internal_suit(const_string &path);

/*
Функция выполняет открытие файла набора тестов и читает его содержимое (построчно).
Возвращается содержимое файла.
@param path путь к файлу с набором тестов.
@return набор, включающий тесты.
*/
inline List<List<string> > proceed_suit(const_string &path) {
    try {
        ifstream in;
        in.open(path);
        if (in.bad() == true) {
            println_error("error occurred during open suit file");
            gracefully_exit();
            return List<List<string> >{};
        }
        string fileLine;
        List<string> test_suit;
        List<List<string> > all_suit;
        while (getline(in, fileLine)) {
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
        if (all_suit.getSize() == 0) {
            throw ERROR_CODE;
        }
        in.close();
        return all_suit;
        } catch (int code) {
            println_error("error in proceed suit");
        }
        return {{}};
}

/*
Обработка внутреннего набора тестов,
@return набор тестов.
*/
inline List<string> proceed_internal_suit(const_string &path) {
    try {
        ifstream in;
        in.open(path);
        if (in.bad() == true) {
            println_error("error occurred during file open");
            gracefully_exit();
            return List<string>{};
        }
        string fileLine;
        List<string> test_suit = List<string>{};
        while (getline(in, fileLine)) {
            if (fileLine != NEW_LINE) {
                if (!starts_with(fileLine, IGNORE_TEST)) {
                    test_suit.addElement(fileLine);
                }
            }
        }
        if (test_suit.getSize() == 0) {
            throw ERROR_CODE;
        }
        in.close();
        return test_suit;
    } catch (int code) {
        println_error("error in proceed inter suit");
    }
}

/*
Обработка двойного массива и переделывание его в одномерный массив.
@return массив строк,
*/
inline List<string> proceed_double_array(const List<List<string> > &d_array) {
    List<string> test_suit = List<string>{10};
    try {
        for (const auto &outer: d_array) {
            for (const auto &inner: outer) {
                test_suit.addElement(inner);
            }
        }
        if (test_suit.getSize() == 0) { //Не выполнится
            throw ERROR_CODE;
        }
        return test_suit;
    } catch (int code) {
        println("Error in proceed double array");
    }
}

/*
 * Функция для заполнения одномерного массива. 
 */
inline void fill_array(List<string> *array_to_fill, counter_v lenght) {
    try {
        for (int i = 0; i < lenght; i++) {
            array_to_fill->push_back("");
        }
        if (array_to_fill->size() == 0) {
            throw ERROR_CODE;
        }
    } catch (int code) {
        println_error("error in fill array");
    }
    
}

inline void fill_double_array(List<List<string>> *double_array_to_fill, counter_v lenght) {
    try {
        for (int i = 0; i < lenght; i++) {
        double_array_to_fill->push_front(get_empty_list());
            for (int x = 0; x < lenght; x++) {
                double_array_to_fill[i][x].addElement("");
            }
        }
        if (double_array_to_fill->size() == 0) {
            throw ERROR_CODE;
        }
    } catch (int code) {
        println_error("error in filling double array");
    }
}