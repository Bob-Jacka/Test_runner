#pragma once
#define nil = 0

#include <stdio.h>

void game_stages(int game_num);

void game_tests(string game_stages_cli, string games_devices_cli);

bool reverse_scan(string scan_val);

void get_results(bool is_write_to_file);

void print_results(void);

void printMSG(string str, Color clr);

[]

string proceed_file(string path, bool increm);

bool check_dir(string path);

[]

string Atos(string str, bool increm);

bool Atob(string str);

char *split_string(const char *str, const char delimiter, int *count);

void get_help_menu(void);

inline int len(char arr[]) {
    int count = 0;
    for (auto __: arr) {
        count++;
    }
    return count;
}

inline void println(string msg) {
    printf(msg + "\n")
}

inline void println(void) {
    printf("\n")
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
