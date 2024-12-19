#pragma once

#include <iostream>
#include "custom_types.h"
#include "game_results.h"
#include <sstream>
#include <vector>
#include <list>

counter_v test_length_2d(List<List<one_device_results> > array);
counter_v test_length_1d(List<List<one_device_results> > array);

inline void println(const_string msg) {
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

inline void init_string_arr(List<string> to_which, List<string> from_which[]) {
    counter_v counter = 0;
    for (counter_v i = 0; i < to_which.getSize(); i++) {
        string elem = from_which->getElement(i);
        if (elem != nullptr) {
            to_which.setElement(counter, elem);
        }
        counter++;
    }
}

/*
 * Поддерживается только двойной массив (массив массивов)
 */
inline void init_array_by(List<List<one_device_results> > to_which, const one_test_result &by) {
    counter_v inner_array = 0;
    counter_v outer_array = 0;
    for (outer_array = 0;
         outer_array < test_length_2d(to_which); outer_array++) {
        for (inner_array = 0; inner_array < to_which.getElement(outer_array).getElement(inner_array).size(); inner_array++) {
            //TODO возможно беда из -за индекса
            to_which.getElement(outer_array).getElement(inner_array) = reinterpret_cast<const char *>(&by);
        }
    }
}

inline void get_time_stat(const long seconds_time) {
    println("Отчет по тестированию");
    println("На тестирование ушло");
    counter_v minutes = seconds_time / 60;
    counter_v hours = minutes > 60 ? minutes / 60 : 0;
    counter_v seconds = 0; //TODO дописать секунды
    printf("%d часов\n %d минут\n %d секунд\n", hours, minutes, seconds);
}

inline List<string> split_string(const char *str, const char delimiter) {
    List<string> splitted;
    std::istringstream iss(str);
    std::vector<string> words{std::istringstream<std::string>(iss), std::istringstream<std::string>()};
    for (string i: words) {
        splitted.addElement(i);
    }
    return splitted;
}

/*
Вывод сообщения независимо от цвета
*/
inline void printMSG(const const_string str, const Color clr) {
    printf("\n", *cReset);
    printf(&clr, str, "\n");
    printf("\n", *cReset);
}

inline counter_v str_length_1d(string *array) {
    counter_v counter = 0;
    for ([[maybe_unused]] auto &str: array) {
        counter++;
    }
    return counter;
}

inline counter_v str_length_2d(List<List<string> > array) {
    counter_v counter = 0;
    for (string str1: array.getElement(0)) {
        //TODO возможно проблема с индексом листа
        //TODO обработка двойного массива
        for (string str2: array.getElement(0).getElement(0)) {
            //TODO возможно проблема с индексом листа
            counter++;
        }
        counter++;
    }
    return counter;
}

inline counter_v test_length_1d(List<List<one_device_results> > array) {
    counter_v counter = 0;
    for (one_device_results str: array.getElement(0)) {
        //TODO возможно проблема с индексом листа
        counter++;
    }
    return counter;
}

inline counter_v test_length_2d(List<List<one_device_results> > array) {
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
