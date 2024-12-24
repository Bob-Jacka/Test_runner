#pragma once

#include <iostream>
#include "custom_types.h"
#include "game_results.h"
#include <fstream>
#include <vector>
#include <list>

counter_v test_length_2d(List<List<one_device_results> > &array);

inline void println(const_string &msg) {
    std::cout << msg;
    std::cout << "\n";
}

inline void print_next_line() {
    std::cout << "\n";
}

inline void gracefully_exit() {
    println("Exiting...");
    exit(1);
}

inline void init_string_arr(List<string> to_which, const List<string>& from_which) {
    for (counter_v i = 0; i < to_which.getSize(); i++) {
        if (string elem = from_which.getElement(i); !elem.empty()) {
            to_which.setElement(i, elem);
        }
    }
}

/*
 * Поддерживается только двойной массив (массив массивов)
 */
inline void init_array_by(List<List<one_device_results> > to_which, const one_test_result &by) {
    counter_v inner_array = 0;
    counter_v outer_array = 0;
    //TODO возможно из за этого ошибка компиляции
    for (outer_array = 0; outer_array < test_length_2d(to_which); outer_array++) {
        for (inner_array = 0;
             inner_array < to_which.getElement(outer_array).getElement(inner_array).size(); inner_array++) {
            //TODO возможно беда из -за индекса
            to_which.getElement(outer_array).getElement(inner_array) = reinterpret_cast<const char *>(&by);
        }
    }
}

inline void get_time_stat(const unsigned long seconds_time) {
    println("Отчет по тестированию");
    println("На тестирование ушло");
    counter_v minutes = seconds_time / 60;
    counter_v hours = minutes > 60 ? minutes / 60 : 0;
    counter_v seconds = 0; //TODO дописать секунды
    printf("%d часов\n %d минут\n %d секунд\n", hours, minutes, seconds);
}

inline List<string> split_string(const_string &str, const char delimiter) {
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

            string word = words.substr(index_start, index_end);
            splitted.addElement(word);
            word = "";
        }
    }
    return splitted;
}

/*
Вывод сообщения независимо от цвета
*/
inline void printMSG(const_string &str, const Color clr) {
    printf("\n" cReset);
    printf(&clr, str, "\n");
    printf("\n" cReset);
}

inline counter_v test_length_2d(List<List<one_device_results> > &array) {
    counter_v counter = 0;
    for (one_device_results str1: array.getElement(0)) {
        //TODO проблема с индексами
        for (one_device_results str2: array.getElement(0)) {
            counter++;
        }
        counter++;
    }
    return counter;
}

inline List<one_test_result> init_test_array() {
    List<one_test_result> device_results;
    for (counter_v i = 0; i < device_results.getSize(); i++) {
        device_results.setElement(i, get_empty());
    }
    return device_results;
}

inline std::string get_string_by_chars(char chars[]) {
    std::string tmp;
    for (int i = 0; i < strlen(chars); i++) {
        tmp.append(&chars[i]);
    }
    return tmp;
}
