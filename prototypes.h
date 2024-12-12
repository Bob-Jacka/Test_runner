#pragma once

void game_stages(int device_num);

void game_tests(string stages_cli, string devices_cli);

bool reverse_scan(string scan_val);

void get_results(bool is_write_to_file);

void print_results(void);

void printMSG(string str, Color clr);

string *proceed_file(string path, bool increm);

bool check_dir(string path);

string *Atos(string str, bool increm);

bool Atob(string str);

char *split_string(const char *str, char delimiter, int *count);

void get_help_menu(void);
