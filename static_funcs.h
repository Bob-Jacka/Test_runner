#pragma once
#include <stdio.h>
#include "custom_types.h"

inline void println(const string msg) {
    printf(msg, +"\n");
}

inline void println(void) {
    printf("\n");
}

inline char **create_arr(const int size) {
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

inline int len(char arr[]) {
    int count = 0;
    for (auto __: arr) {
        count++;
    }
    return count;
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
    for (auto __: from_which) {
        if (from_which[__] != NULL) {
            to_which[__] = from_which[__];
        }
    }
}

/*
 * поддерживается только двойной массив (массив массивов)
 */
inline void init_array_by(string to_which[][], const unknown by) {
    unsigned int inner_array = 0;
    unsigned int outer_array = 0;
    for (outer_array = 0; outer_array < len(to_which); outer_array++) {
        for (inner_array = 0; inner_array < len(to_which[outer_array]); inner_array++) {
            to_which[outer_array][inner_array] = by;
        }
    }
}

inline void get_time(long seconds_time) {
    //tODO сделать функцию для определения времени
}

