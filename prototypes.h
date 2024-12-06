#pragma once
#define nil = 0
typedef char Color;
typedef char string;

struct one_test_result {
    bool pass;
    string name;
    string errors_in_test;
};

typedef struct game_result {
    string game_name;
    string platform_name;
    struct one_test_result test_res[];
} game_results;

void game_stages(int game_num);

void game_tests(string game_stages_cli, string games_devices_cli);

bool reverse_scan(string scan_val);

void get_results(bool is_write_to_file);

void print_results(void);

void printMSG(string str, Color clr);

[]string proceed_file(string path, bool increm);

bool check_dir(string path);

[]string Atos(string str, bool increm);

bool Atob(string str);

char *split_string(const char *str, const char delimiter, int *count);

inline int len(char arr[]) {
    int count = 0;
    for (auto __: arr) {
        count++;
    }
    return count;
}

inline int len(struct game_result results) {
    int count = 0;
    for (auto __: results.test_res) {
        count++;
    }
    return count;
}
