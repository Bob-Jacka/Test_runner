#include <ctype.h>
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#include "static_funcs.h"
#include "prototypes.h"
#include "game_results.h"

one_device_results all_stages_test_results[][];
//Первый массив - игра, Второй массив - результаты тестов каждой игры

string test_stages[];
string games_list[];

time_t start_time;
time_t end_time;

unsigned int devices_count;
unsigned int stage_count;

int main(const int argc, char *argv[]) {
    const string args = split_string(*argv, " ", argc);
    //TODO добавить в аргументы параметр - название игры
    switch (argc) {
        case 1:
            printf("Utility usage:");
            printf("First cli argument is <Test stages>");
            printf("Second cli argument is <Devices>");
            printf("Third cli argument is <true / false write results to file>");
            print_next_line();
            return 0;
        case 3:
            const bool hardcoded_bool_value = false;
            game_tests(args[1], args[2]);
            get_results(hardcoded_bool_value);
        case 4:
            game_tests(args[1], args[2]);
            get_results(Atob(args[3]));
        default:
            printMSG("Given arguments - ", red);
            printf(argc);
            printMSG("Error in arguments", red);
            gracefully_exit();
    }
    printf(cReset "Bye");
}

void game_tests(const string stages_cli, const string devices_cli) {
    ctime(&start_time);
    init_string_arr(test_stages, Atos(stages_cli, false));
    init_string_arr(games_list, Atos(devices_cli, true));
    if (stage_count != 0) {
        init_array_by(all_stages_test_results, empty);
    } else {
        printMSG("An error ocurred in making array test_results", red);
        gracefully_exit();
    }
    for (counter_v device_num = 0; device_num < size(games_list); device_num++) {
        game_stages(device_num);
    }
    ctime(&end_time);
}

void game_stages(const counter_v device_num) {
    one_test_result stages_result[];
    printMSG(games_list[device_num], blue);
    for (counter_v stage = 0; stage < size(test_stages); stage++) {
        print_upper(test_stages[stage]);
        printMSG("Enter (yes / 1) for success or (no / 0) for failure or skip to skip", yellow);
        printMSG(">> ", green);
        const string res = reverse_scan(user_input);
        stages_result[stage] = one_test_result
        {};
        if (res == SUCCESS) {
            enter_data(stages_result[stage], true, "", strcat(test_stages[stage], SUCCESS));
        } else if (res == SKIPPED) {
            enter_data(stages_result[stage], true, "", strcat(test_stages[stage], SKIPPED));
        } else if (res == FAILURE) {
            string problems;
            printMSG("Напишите, что было не так в тесте: ", red);
            const unsigned int _ = scanf("%s", &problems); //TODO учитывает только одно слово
            if (_ > 0) {
                printMSG("an error occured in section about something bad in test", red);
                return;
            }
            enter_data(stages_result[stage], false, strcat(test_stages[stage], FAILURE), problems);
        }
        all_stages_test_results[device_num]->stages_res[stage] = stages_result[stage];
    }
}

void get_help_menu(void) {
    print_next_line();
    println("This utility provide ability to run testing suites");
    println("Program points:");
    println("\t 1 - Save testing progress");
    println("\t 2 - Print current results");
    println("\t 3 - Print test suit");
    println("\t 4 - Get input cli parameters");
    println("\t 5 - Close menu");
    while (true) {
        unsigned int *option;
        printf(">> ");
        scanf(&option);
        if ((isdigit(*option)) == true) {
            switch (option) {
                case 1:
                    println("Saving current testing progress");
                    break;
                case 2:
                    println("Current results are:");
                    for (one_device_results _: all_stages_test_results) {
                        printMSG(_);
                    }
                    break;
                case 3:
                    println("Utility parameters");
                    println("First param are 'Game stages' (ex. tests that you want to test)");
                    println("Second param are 'Devices' (ex. android, ios, desktop)");
                    println("Third param is optional, but it point to write tests result to file or not (true \\ false)");
                    continue;
                case 4:
                    for (string _: test_stages) {
                        printMSG(_);
                    }
                    break;
                case 5:
                    println("Bye");
                    break;
            }
        } else {
            println("Invalid argument type");
        }
    }
}

void get_results(const bool is_write_to_file) {
    if (!is_write_to_file) {
        print_results();
    } else {
        printMSG("Saving in file", green);
        // write_to_file();
    }
}

void print_results(void) {
    printf("\n");
    for (counter_v game_num = 0; game_num < size(all_stages_test_results); game_num++) {
        printf(cReset);
        print_upper(games_list[game_num]); //TODO большие строки
        for (counter_v stage = 0; stage < size(all_stages_test_results[game_num]); stage++) {
            const string res = all_stages_test_results[game_num][stage].stages_res->name;
            printf(blue, strcat("\t", res));
        }
    }
    const unsigned long dur = start_time - end_time;
    get_time_stat(dur);
}

string reverse_scan() {
    string user_input;
    const unsigned int _ = scanf("%s", &user_input);
    if (_ > 0) {
        printMSG("an error occured in scan game stage result", red);
        return;
    }
    switch (*user_input) {
        case "y":
            return SUCCESS;
        case "n":
            return FAILURE;
        case "s":
            return SKIPPED;
        case "^C":
            gracefully_exit();
        case "-h":
        case "--help":
            get_help_menu();
            return false;

        default:
            printMSG("Invalid argument", red);
            printMSG("Please, try again", red);
            printMSG(">> ", green);
            char txt;
            const unsigned int _ = scanf("%c", &txt);
            if (_ > 0) {
                printMSG("Error in recursion", red);
            }
            return reverse_scan(txt);
    }
}

/*
Проверка того, что переданный путь это файл
*/
bool check_file(const string path) {
    const int _ = fopen(path, "r");
    if (_ == -1) {
        printMSG("Error occured in checking file", red);
        check_dir(path);
        if (!strstr(path, ".")) {
            printMSG("Maybe path is not contains file extension", red);
        }
        gracefully_exit();
        return false;
    } else {
        printMSG("File exists", green);
        return true;
    }
}

/*
Проверка того, что переданный путь это директория
*/
bool check_dir(const string path) {
    const int _ = fopen(path, "r");
    if (_ != -1) {
        printMSG("Error occured is checking dir", red);
        gracefully_exit();
        return false;
    } else {
        printMSG("Dir exists", yellow);
        return true;
    }
}

/*
Перевод строки в булево значение
*/
bool Atob(const string str) {
    switch (*str) {
        case "1":
        case "True":
        case "true":
            return true;
        case "0":
        case "False":
        case "false":
            return false;
        default:
            printMSG("Invalid argument", red);
            gracefully_exit();
            return false;
    }
}

/*
Перевод строки в массив
*/
string *Atos(const char str[], const bool increm) {
    if (check_file(str)) {
        printMSG("Argument is a file", green);
        return proceed_file(str, increm);
    }
    if (strstr(str, ",")) {
        printMSG("using as separator ','", green);
        string *splitted = split_string(str, ",", strlen(str));
        return splitted;
    } else {
        printMSG("using as separator ' ' *whitespace", red);
        string *splitted = split_string(str, " ", 1);
        return splitted;
    }
}

/*
Функция выполняет открытие файла и читает его содержимое (построчно)
Возвращается содержимое файла
*/
string *proceed_file(const string path, bool *increm) {
    FILE *file = fopen(path, "r");
    if (file != NULL) {
        printMSG("error ocurred during file open", red);
        fclose(file);
        gracefully_exit();
    }
    char fileLine[100];
    string file;
    while (!feof(file)) {
        fgets(fileLine, 100, file);
        if (!feof(file)) {
            puts(fileLine);
        }
    }
    fclose(file); //TODO возвращается только прочитанная строка
    return fileLine;
}
