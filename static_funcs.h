#pragma once

#include <stdio.h>
#include "custom_types.h"
#include "game_results.h"

inline void println(const string msg) {
    printf(msg, "\n");
}

inline void print_next_line() {
    printf("\n");
}

inline char **create_arr(counter_v size) {
    char **array = malloc(size * sizeof(char *));
    for (auto int i = 0; i < size; i++) {
        array[i] = malloc(50 * sizeof(char));
        if (array[i] == NULL) {
            println("Failed to allocate memory");
            exit(1);
        }
        sprintf(array[i], "String %d", i);
    }
    return array;
}

inline void gracefully_exit(void) {
    println("Exiting...");
    exit(1);
}

inline ssize_t get_line(char **lineptr, size_t *n, FILE *stream) {
    // Проверка на NULL
    if (lineptr == NULL || stream == NULL) {
        return -1; // Возвращаем -1 в случае ошибки
    }
    // Если размер буфера равен 0, выделяем память
    if (*lineptr == NULL) {
        *n = 128; // Начальный размер
        *lineptr = malloc(*n);
        if (*lineptr == NULL) {
            return -1; // Ошибка выделения памяти
        }
    }
    ssize_t total_read = 0; // Общее количество считанных символов
    int c; // Переменная для хранения считанного символа
    while (1) {
        c = fgetc(stream); // Считываем символ из файла

        if (c == EOF) {
            if (total_read == 0) {
                return -1; // Если ничего не считано, возвращаем -1
            }
            break; // Конец файла
        }

        // Проверяем, нужно ли увеличить размер буфера
        if (total_read + 1 >= *n) {
            *n *= 2; // Увеличиваем размер буфера
            char *new_lineptr = realloc(*lineptr, *n);
            if (new_lineptr == NULL) {
                free(*lineptr); // Освобождаем память в случае ошибки
                return -1;
            }
            *lineptr = new_lineptr;
        }

        (*lineptr)[total_read++] = c; // Сохраняем считанный символ

        // Проверяем, не достигли ли конца строки
        if (c == '\n') {
            break; // Конец строки
        }
    }
    (*lineptr)[total_read] = '\0'; // Завершаем строку нулевым символом
    return total_read; // Возвращаем количество считанных символов
}

inline void init_string_arr(string to_which[], string from_which[]) {
    counter_v counter = 0;
    for (counter_v i = 0; i < sizeof(to_which) / sizeof(to_which[0]); i++) {
        string elem = from_which[i];
        if (elem != NULL) {
            to_which[counter] = elem;
        }
        counter++;
    }
}

/*
 * Поддерживается только двойной массив (массив массивов)
 */
inline void init_array_by(string to_which[][], const one_test_result by) {
    unsigned int inner_array = 0;
    unsigned int outer_array = 0;
    for (outer_array = 0;
         outer_array < size(to_which); outer_array++) { //TODO сделать функцию для длинны массива массивов
        for (inner_array = 0; inner_array < size(to_which[outer_array]); inner_array++) {
            to_which[outer_array][inner_array] = &by;
        }
    }
}

inline void get_time_stat(long seconds_time) {
    println("Отчет по тестированию");
    println("На тестирование ушло");
    counter_v minutes = seconds_time / 60;
    counter_v hours = minutes > 60 ? minutes / 60 : 0;
    counter_v seconds = 0; //TODO дописать секунды
    printf("%d часов\n %d минут\n %d секунд\n", hours, minutes, seconds);
}

inline int double_array_len(string array[][]) {
    size_t outer_length = sizeof(array) / sizeof(array[0]);
    size_t inner_length = sizeof(array[0]) / sizeof(array[0][0]);
}

/*
 * Печать строки в верхнем регистре
 */
inline void print_upper(string str) {
    int j = 0;
    char ch;
    while (str[j]) {
        ch = str[j];
        putchar(toupper(ch));
        j++;
    }
}

inline char *split_string(const char *str, const char delimiter, int *count) {
    char *temp_str = strdup(str);
    if (!temp_str) {
        return NULL;
    }
    *count = 0;
    for (const char *p = temp_str; *p; p++) {
        if (*p == delimiter) {
            (*count)++;
        }
    }
    (*count)++;
    char *result = malloc(*count * sizeof(char *));
    if (!result) {
        free(temp_str);
        return NULL;
    }
    const char *token = strtok(temp_str, &delimiter);
    int index = 0;
    while (token) {
        result[index++] = strdup(*token);
        token = strtok(NULL, &delimiter);
    }
    free(temp_str);
    return result;
}

/*
Вывод сообщения независимо от цвета
*/
void printMSG(const string str, const Color clr) {
    printf("\n"cReset);
    printf(clr, str, "\n");
    printf("\n"cReset);
}