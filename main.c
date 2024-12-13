#include <ctype.h>
#include <time.h>
#include <stdbool.h>
#include <fileapi.h>
#include <stdlib.h>
#include <string.h>

#include "static_funcs.h"
#include "prototypes.h"
#include "game_results.h"

#define  red  "\x1b[31m"
#define   blue  "\x1b[34m"
#define   green  "\x1b[32m"
#define   yellow  "\x1b[33m"
#define   cReset  "\x1b[0m"

one_device_results all_stages_test_results[][];
//Первый массив - игра, Второй массив - результаты тестов каждой игры

string test_stages[];
string games_list[];

time_t start_time;
time_t end_time;

unsigned int devices_count;
unsigned int stage_count;

int main(const int argc, char *argv[]) {
    const unsigned int args_len = argc;
    const string *args = split_string(&argv, " ", args_len);
    //TODO добавить в аргументы параметр - название игры
    //TODO сделать передачу через параметры по типу: -h, --help
    switch (args_len) {
        case 1:
            printf("Utility usage:");
            printf("First cli argument is <Game stages>");
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
            printf(args_len);
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

void game_stages(const unsigned int device_num) {
    one_test_result stages_result[];
    printMSG(games_list[device_num], blue);
    for (counter_v stage = 0; stage < size(test_stages); stage++) {
        printMSG(test_stages[stage], blue);
        printMSG("Enter 1 for yes if success or 0 for no if not or skip to skip", yellow);
        printMSG(">> ", green);
        string user_input;
        const unsigned int _ = scanf("%c", &user_input);
        if (_ != 1) {
            printMSG("an error occured in scan game stage result", red);
            return;
        }
        const bool res = reverse_scan(user_input);
        stages_result[stage] = one_test_result{};
        if (res == true) {
            enter_data(stages_result[stage], true, "", strcat(test_stages[stage], SUCCESS));
        } else if (res == false && strcmp(user_input, param_to_skip) != 0) {
            enter_data(stages_result[stage], true, "", strcat(test_stages[stage], SKIPPED));
        } else if (res == false) {
            string problems;
            printMSG("Напишите, что было не так в тесте: ", red);
            const unsigned int _ = scanf("%s", &problems);
            if (_ != 1) {
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
        scanf(&option);
        if (sizeof(option) == 4) {
            switch (option) {
                case 1:
                    println("Saving current testing progress");
                    break;
                case 2:
                    println("Current results are:");
                    for (unknown _: all_stages_test_results) {
                        printMSG(_);
                    }
                    break;
                case 3:
                    println("Utility parameters");
                    println("First param are 'Game stages' (ex. tests that you want to test)");
                    println("Second param are 'Devices' (ex. android, ios, desktop)");
                    println(
                        "Third param is optional, but it point to write tests result to file or not (true \\ false)");
                    continue;
                case 4:
                    for (unknown _: test_stages) {
                        printMSG(_);
                    }
                    break;
                case 5:
                    println("Bye");
                    break;
                default:
                    println("Invalid argument");
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
        printf(games_list[game_num]); //TODO большие строки
        for (counter_v stage = 0; stage < size(all_stages_test_results[game_num]); stage++) {
            const string res = all_stages_test_results[game_num][stage].stages_res->name;
            printf(blue, strcat("\t", res));
        }
    }
    println("Отчет по тестированию");
    const unsigned long dur = start_time - end_time;
    get_time(dur);
}

bool reverse_scan(const char scan_val) {
    switch (&scan_val) {
        case 'y':
            return true;
        case 'n':
            return false;
        case 's':
            return false;
        case '^C':
            gracefully_exit();
        // case '-h':
        // case "--help":
        //     get_help_menu();
        //     return false;

        default:
            printMSG("Invalid argument", red);
            printMSG("Please, try again", red);
            printMSG(">> ", green);
            char txt;
            const unsigned int _ = scanf("%c", &txt);
            if (_ != 1) {
                printMSG("Error in recursion", red);
            }
            return reverse_scan(txt);
    }
}

/*
Проверка того, что переданный путь это файл
*/
bool check_file(const string path) {
    const int _ = fopen(path, 'r');
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
    const int _ = fopen(path, 'r');
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
    switch (str) {
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

char *read_line_from_file(FILE *file) {
    size_t size = 0;
    char *line = NULL;
    // Читаем строку из файла
    if (get_line(&line, &size, file) == -1) {
        free(line);
        return NULL;
    }
    return line;
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
    string to_return[];
    counter_v counter = 0;
    char *readed_line = NULL; // Указатель на строку
    size_t size = 0; // Размер буфера
    while (get_line(&readed_line, &size, file) != -1) {
        if (!strcmp(&readed_line[0], not_include)) {
            to_return[counter] = readed_line;
            counter++;
        }
    }
    return to_return;
    fclose(file);
}

/*
Вывод сообщения независимо от цвета
*/
void printMSG(const string str, const Color clr) {
    printf("\n" cReset);
    printf(clr, str, "\n");
    printf("\n" cReset);
}

char *split_string(const char *str, const char delimiter, int *count) {
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
        result[index++] = strdup(&token);
        token = strtok(NULL, &delimiter);
    }
    free(temp_str);
    return result;
}
