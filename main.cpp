#include <cctype>
#include <ctime>
#include <cstdlib>
#include <cstring>

#include "List.h"
#include "static_funcs.h"
#include "prototypes.h"
#include "game_results.h"

List<List<one_device_results> > all_stages_test_results;
//Первый массив - игра, Второй массив - результаты тестов каждой игры

List<string> test_stages;
List<string> games_list;

time_t start_time;
time_t end_time;

unsigned int devices_count;
unsigned int stage_count;

int main(const char *argc, char *argv[]) {
    List<string> args = split_string(get_string_by_chars(*argv), *" ");
    //TODO добавить в аргументы параметр - название игры
    switch (*argc) {
        case 1: {
            printf("Utility usage:");
            printf("First cli argument is <Test stages>");
            printf("Second cli argument is <Devices>");
            printf("Third cli argument is <true / false write results to file>");
            print_next_line();
            return 0;
        }
        case 3: {
            constexpr bool hardcoded_bool_value = false;
            game_tests(args.getElement(1), args.getElement(2));
            get_results(hardcoded_bool_value);
            break;
        }
        case 4: {
            game_tests(args.getElement(1), args.getElement(2));
            get_results(std::stoi(args.getElement(3)));
            break;
        }
        default: {
            printMSG("Given arguments - ", *red);
            printf(argc);
            printMSG("Error in arguments", *red);
            gracefully_exit();
        }
    }
    printf(cReset "Bye");
}

void game_tests(const_string stages_cli, const string devices_cli) {
    ctime(&start_time);
    init_string_arr(test_stages, Atos(stages_cli, false));
    init_string_arr(games_list, Atos(devices_cli, true));
    if (stage_count != 0) {
        init_array_by(all_stages_test_results, get_empty());
    } else {
        printMSG("An error ocurred in making array test_results", *red);
        gracefully_exit();
    }
    for (counter_v device_num = 0; device_num < games_list.getSize(); device_num++) {
        game_stages(device_num);
    }
    ctime(&end_time);
}

void game_stages(const counter_v device_num) {
    List<one_test_result> stages_result;
    print_next_line();
    printMSG(games_list.getElement(device_num), *blue);
    for (counter_v stage = 0; stage < test_stages.getSize(); stage++) {
        std::uppercase(reinterpret_cast<std::ios_base &>(*test_stages.getElement(stage)));
        printMSG("Enter (yes / 1) for success or (no / 0) for failure or skip to skip", *yellow);
        printMSG(">> ", *green);
        const string res = reverse_scan();
        string problems = "";
        stages_result.setElement(stage, one_test_result{});
        if (res == SUCCESS) {
            enter_data(stages_result.getElement(stage), true, (test_stages.getElement(stage).append(SUCCESS)),
                       problems);
        } else if (res == SKIPPED) {
            enter_data(stages_result.getElement(stage), true, (test_stages.getElement(stage).append(SKIPPED)),
                       problems);
        } else if (res == FAILURE) {
            printMSG("Напишите, что было не так в тесте: ", *red);
            const counter_v _ = scanf("%s", &problems); //TODO учитывает только одно слово
            if (_ > 0) {
                printMSG("an error occured in section about something bad in test", *red);
                return;
            }
            enter_data(stages_result.getElement(stage), false, (test_stages.getElement(stage).append(FAILURE)),
                       problems);
        }
        all_stages_test_results.getElement(device_num).getElement(stage).set_stages_res(
                stages_result.getElement(stage), stage);
        //TODO подозрительно
    }
}

void get_help_menu() {
    print_next_line();
    println("This utility provide ability to run testing suites");
    println("Program points:");
    println("\t 1 - Save testing progress");
    println("\t 2 - Print current results");
    println("\t 3 - Print test suit");
    println("\t 4 - Get input cli parameters");
    println("\t 5 - Close menu");
    while (true) {
        counter_v option = 0;
        printf(">> ");
        scanf("%d", &option);
        if (isdigit(option) == true) {
            switch (option) {
                case 1: {
                    println("Saving current testing progress");
                    break;
                }
                case 2: {
                    println("Current results are:");
                    counter_v device_num = 0;
//                    for (one_device_results _1: all_stages_test_results.getElement(device_num)) {
//                        one_device_results outer_array_game = _1;
//                        counter_v inner_array_test = 0; //TODO может быть ошибка из-за двойного массива
                    // for (one_test_result _2: outer_array_game) {
                    //     println(const_cast<string>(_2.name.c_str()));
                    //     println(reinterpret_cast<string>(_2.pass));
                    //     println(const_cast<string>(_2.errors_in_test.c_str()));
                    // }
                    device_num++;
                }
                    break;
                case 3: {
                    println("Utility parameters");
                    println("First param are 'Game stages' (ex. tests that you want to test)");
                    println("Second param are 'Devices' (ex. android, ios, desktop)");
                    println(
                            "Third param is optional, but it point to write tests result to file or not (true \\ false)");
                    continue;
                }
                case 4: {
                    for (int i = 0; i < test_stages.getSize(); i++) {
                        printMSG(test_stages.getElement(i), *blue);
                    }
                    break;
                }
                case 5: {
                    println("Bye");
                    break;
                }
                default:
                    break;
            }
        } else {
            println("Invalid argument type");
        }
    }
}

void get_results(const int is_write_to_file) {
    if (!is_write_to_file) {
        print_results();
    } else {
        printMSG("Saving in file", *green);
        // write_to_file();
    }
}

void print_results() {
    printf("\n");
    for (counter_v game_num = 0; game_num < test_length_2d(all_stages_test_results); game_num++) {
        printf(cReset);
        std::uppercase(reinterpret_cast<std::ios_base &>(*games_list.getElement(game_num)));
        for (counter_v stage = 0; stage < all_stages_test_results.getElement(game_num).getSize(); stage++) {
            const std::string res = all_stages_test_results.getElement(game_num).getElement(stage).get_stages_res().
                    getElement(stage).get_name();

            printf(blue, strcat("\t", res.c_str()));
        }
    }
    const unsigned long dur = start_time - end_time;
    get_time_stat(dur);
}

string reverse_scan() {
    int user_input;
    const unsigned int _ = scanf("%d", &user_input);
    if (_ > 0) {
        printMSG("an error occured in scan game stage result", *red);
        return FAILURE;
    }
    switch (user_input) {
        case 1:
            return SUCCESS;
        case 0:
            return FAILURE;
        case 2:
            return SKIPPED;
//        case switch_stats::EXIT:
//            gracefully_exit();
//        case switch_stats::HELP_short:
//        case switch_stats::HELP_long:
//            get_help_menu();
//            return nullptr;

        default:
            printMSG("Invalid argument", *red);
            printMSG("Please, try again", *red);
            printMSG(">> ", *green);
            if (_ > 0) {
                printMSG("Error in recursion", *red);
            }
            return reverse_scan();
    }
}

/*
Проверка того, что переданный путь это файл
*/
bool check_file(const_string path) {
    std::ifstream in;
    in.open(path);
    if (!in.is_open()) {
        printMSG("Error occured in checking file", *red);
        check_dir(path);
        if (path.find(".") == -1) {
            in.close();
            printMSG("Maybe path is not contains file extension", *red);
        }
        gracefully_exit();
        return false;
    } else {
        in.close();
        printMSG("File exists", *green);
        return true;
    }
}

/*
Проверка того, что переданный путь это директория
*/
bool check_dir(const_string path) {
    std::ifstream in;
    in.open(path);
    if (!in.is_open()) {
        printMSG("Error occured is checking dir", *red);
        in.close();
        gracefully_exit();
        return false;
    } else {
        printMSG("Dir exists", *yellow);
        in.close();
        return true;
    }
}

/*
Перевод строки в массив
*/
List<string> Atos(const_string str, const bool increm) {
    if (check_file(str)) {
        printMSG("Argument is a file", *green);
        return proceed_file(str, increm);
    }
    if (str.find(",") == -1) {
        printMSG("using as separator ','", *green);
        List<string> splitted = split_string(str, *",");
        return splitted;
    } else {
        printMSG("using as separator ' ' *whitespace", *red);
        List<string> splitted = split_string(str, *" ");
        return splitted;
    }
}

/*
Функция выполняет открытие файла и читает его содержимое (построчно)
Возвращается содержимое файла
*/
List<string> proceed_file(const_string path, const bool increm) {
    std::ifstream in;
    in.open(path);
    if (!in.is_open()) {
        printMSG("error ocurred during file open", *red);
        gracefully_exit();
        return List<string>{};
    } else {
        printMSG("Dir exists", *yellow);
        std::string fileLine;
        List<string> fileLines;
        while (std::getline(in, fileLine)) {
            fileLines.addElement(fileLine.data());
        }
        in.close();
        return fileLines;
    }
}
