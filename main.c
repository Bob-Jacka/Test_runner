#include <ctype.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <fileapi.h>
#include "prototypes.h"

#include <stdlib.h>
#include <string.h>

Color red = "\033[31m";
Color blue = "\033[34m";
Color green = "\033[32m";
Color yellow = "\033[33m";
Color cReset = "\033[0m";

game_results test_results[][]; //первый массив - игра, второй массив - результаты тестов каждой игры

string test_stages[];
string games_list[];

time_t start_time;
time_t end_time;

int stage_count;

int main(int argc, char *argv[]) {
    const unsigned int args_len = argc;
    []char args = &argv;
    if (args_len == 1) {
        printf("Utility usage:");
        printf("First cli argument is <Game stages>");
        printf("Second cli argument is <Devices>");
        printf("Third cli argument is <true / false write results to file>");
        printf();
        return 0;
    }

    if (args_len == 3) {
        string hardcodeed_bool_value = "false";
        game_tests(os.Args[1], os.Args[2]);
        get_results(Atob(hardcodeed_bool_value));
    } else if (args_len == 4) {
        game_tests(os.Args[1], os.Args[2]);
        get_results(Atob(os.Args[3]));
    } else {
        printf(red, "Given arguments - ");
        printf(+len(os.Args));
        printf(red, "Error in arguments");
        exit(1);
    }
    printf(cReset, "Bye");
}

void game_tests(const string game_stages_cli, string games_devices_cli) {
    ctime(&start_time);

    test_stages = Atos(game_stages_cli, false);
    games_list = Atos(games_devices_cli, true);
    if (stage_count != 0) {
        test_results = make([][]string, len(games_devices_cli) + 1);
    } else {
        printMSG("An error ocurred in making array test_results", red);
        exit(1);
    }

    for (auto int game_num = 0; game_num < len(games_list); game_num++) {
        printMSG(toupper(games_list[game_num]), blue);
        game_stages(game_num);
    }
    ctime(&end_time);
}

void game_stages(const int game_num) {
    []string stages_result;
    for (auto int stage = 0; stage < len(test_stages); stage++) {
        printMSG(toupper(test_stages[stage]), blue);
        printMSG("Enter 1 for yes if success or 0 for no if not or skip to skip", yellow);
        printf(green, ">> ");
        string txt;
        string _, error1 = scanf(&txt);
        if (error1 != nil) {
            printMSG("an error occured in scan game stage result", red);
            return;
        }
        const string res = reverse_scan(txt);
        []

        string newSlice = make([]string, len(stages_result) + 1, cap(stages_result) + 1 * 2);
        stages_result = newSlice;
        if (res == true) {
            stages_result[stage] = test_stages[stage] + " No errors";
        } else if (res == false && strcmp(txt, "skip") == 1) {
            stages_result[stage] = test_stages[stage] + " TEST SKIPPED";
        } else if (res == false) {
            string problems;
            printMSG("Напишите, что было не так в тесте: ", red);
            string _, error2 = scanf(&problems); //TODO сделать массивом строк
            if (error2 != nil) {
                printMSG("an error occured in section about something bad in test", red);
                return;
            } //TODO сделать сочетание -h для меню, где можно будет сохранить результат посередине пробега
            stages_result[stage] = test_stages[stage] + problems;
        }
    }
    test_results[game_num] = append(stages_result);
}

void get_results(const bool is_write_to_file) {
    if (!is_write_to_file) {
        print_results();
    } else {
        printMSG("Saving in file", green);
        // write_to_file();
    }
}

void print_results() {
    printf("\n");
    for (int game_num = 0; game_num < len(test_results); game_num++) {
        printf(cReset);
        printf(toupper(games_list[game_num]));
        for (unsigned int stage = 0; stage < len(test_results[game_num]); stage++) {
            struct game_result res = test_results[game_num][stage];
            if (res != nil) {
                printf(blue, "\t" + res);
            }
        }
    }
    unsigned long dur = end_time.Sub(start_time);
    printf("\n");
    printf("Времени на тестирование ушло: \n");
    printf(green, "\t Часов - ");
    printf(dur);
    printf(green, "\t Минут - ");
    printf(dur);
    printf(green, "\t Секунд - ");
    printf(dur);
    printf("\n");
    printf(yellow, "Other info");
    printf(yellow, "Go compiler is " + runtime.Compiler);
    printf(yellow, "Computer arch at time is " + runtime.GOARCH);
    printf(yellow, "Max available processors are ");
    printf(runtime.GOMAXPROCS(runtime.NumCPU()));
}

bool reverse_scan(const string scan_val) {
    switch (scan_val) {
        case "1":
        case "ye":
        case "y":
        case "yes":
            return true;

        case "0":
        case "n":
        case "no":
            return false;
        case "skip":
            return false;

        default:
            printMSG("Invalid argument", red);
            printMSG("Please, try again", red);
            printMSG(">> ", green);
            string txt;
            string _, err = scanf(&txt);
            if (err != nil) {
                printMSG("Error in recursion", red);
            }
            return reverse_scan(txt);
    }
}

/*
Проверка того, что переданный путь это файл
*/
bool check_file(string path) {
    string _, err = os.Stat(path);
    if (err != nil) {
        printMSG("error occured in checking file", red)
        check_dir(path);
        if (!strstr(path, ".")) {
            printMSG("Maybe path is not contains file extension", red)
        }
        exit(1);
        return false;
    } else {
        printMSG("File exists", green);
        return true;
    }
}

/*
Проверка того, что переданный путь это директория
*/
bool check_dir(string path) {
    FILE _, smth = os.ReadDir(path);
    if (smth != nil) {
        printMSG("error occured is checking dir", red);
        exit(1);
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
        case "true": return true;
        case "0":
        case "False":
        case "false": return false;
        default:
            printMSG("Invalid argument", red);
            exit(1);
            return false;
    }
}

/*
Перевод строки в массив
*/
[]string Atos(string str, bool increm) {
    if (check_file(str)) {
        printMSG("Argument is a file", green);
        return proceed_file(str, increm);
    } else if (strstr(str, ",")) {
        printMSG("using as separator ','", green);
        string splitted = split_string(str, ",");
        return splitted;
    } else {
        printMSG("using as separator ' ' *whitespace", red);
        string splitted = split_string(str, " ", 1);
        return splitted;
    }
}

/*
Функция выполняет открытие файла и читает его содержимое (построчно)
Возвращается содержимое файла
*/
[]string proceed_file(string path, bool *increm) {
    FILE file, err = open(path);
    if (err != nil) {
        printMSG("error ocurred during file open", red);
        file.Close();
        exit(1);
    }
    []string readed;
    var reader = bufio.NewReader(file);
    var scanner = bufio.NewScanner(reader);
    for (scanner.Scan()) {
        string line = scanner.Text(); //строка
        if (len(line) > 0) {
            readed = append(readed, line);
            if (increm) {
                stage_count++;
            }
        }
    }
    file.Close();
    return readed;
}

/*
Вывод сообщения независимо от цвета
*/
void printMSG(const string str, const Color clr) {
    printf(cReset + "\n");
    printf(clr, str + "\n");
    printf(cReset + "\n");
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
        result[index++] = strdup(token);
        token = strtok(NULL, &delimiter);
    }
    free(temp_str);
    return result;
}
