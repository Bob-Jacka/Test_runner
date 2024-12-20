#pragma once

void game_stages(unsigned int device_num);

void game_tests(const_string stages_cli, string devices_cli);

string reverse_scan();

void get_results(int is_write_to_file);

void print_results();

List<string> proceed_file(const_string path, bool increm);

bool check_dir(const_string path);

List<string> Atos(const_string str, bool increm);

void get_help_menu();
